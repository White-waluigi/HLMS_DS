/*
 * DSDatablock.cpp
 *
 *  Created on: 24.11.2015
 *      Author: marvin
 */
#include "../Datablocks/Data/DSMaterialParamType.h"
#include "OgreStableHeaders.h"

#include "DSDatablock.h"
#include "../OgreHlmsDS.h"
#include "OgreHlmsManager.h"
#include "OgreHlmsTextureManager.h"
#include "OgreTexture.h"
#include "OgreLogManager.h"
#include "Data/DSTextureParam.h"
#include "Data/DSTextureParamType.h"
#include "Vao/OgreVaoManager.h"
#include "Vao/OgreConstBufferPacked.h"
#include "Vao/OgreTexBufferPacked.h"

#include "Data/DSMaterialParam.h"

#include "../Light/StringValueUtils.h"

#include "../DBParser/DBParser.h"
#include "../DBParser/ParamVecParser.h"

namespace Ogre {
//const size_t DSDatablock::MaterialSizeInGpu          = 4 * sizeof(float) + DSBakedTexture::NUM_DS_TEXTURE_TYPES * sizeof(uint16);

const size_t DSDatablock::MaterialSizeInGpu = 52 * 4 + 14 * 2;
const size_t DSDatablock::MaterialSizeInGpuAligned = alignToNextMultiple(
		DSDatablock::MaterialSizeInGpu, 4 * 4);

DSDatablock::DSDatablock(IdString name, HlmsDS *creator,
		const HlmsMacroblock *macroblock, const HlmsBlendblock *blendblock,
		const HlmsParamVec &params, bool derived) :
		HlmsDatablock(name, creator, macroblock, blendblock, params) {


	this->creator=creator;

	dbtype = DS_DATABLOCK_BASE;
	UpdateAllowed = true;

	this->textureParamTypes = new std::vector<DSTextureParamType>();
	this->vec4ParamTypes = new std::vector<DSMaterialParamType>();
	this->propertyParamTypes = new std::vector<DSPropertyParamType>();
	this->pieceParamTypes = new std::vector<DSPieceParamType>();

	propertyParams = new std::vector<DSPropertyParam *>();
	materialParams = new std::vector<DSMaterialParam *>();
	textureParams = new std::vector<DSTextureParam *>();
	pieceParams = new std::vector<DSPieceParam *>();

	//autoParams = new std::vector<DSAutoParam*>();

	APparser = new DSAutoParamParser();

	MaterialBuffer = NULL;
	// TextureHash=mTextureHash;

	float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	static float incr = 0;
	incr += 0.1;
	IDColour = new Ogre::ColourValue(r, g, b);

	this->status = DIRTY;
	ShadowMaterialBuffer = NULL;
	TextureSize = 0;

	Mparser=new ParamVecParser();
	JMparser=NULL;
	safeForRewrite=false;
	mDSCreator=creator;

	generateControl(true);
}
void DSDatablock::initializeMaterial(const HlmsParamVec &params,
		HlmsDS *creator,DBParser * customparser) {

	initializeParamTypes();

	initializeProperties();

	HlmsParamVec vec=params;
	if(customparser!=NULL){
		customparser->setData(vec,this);
		customparser->parse(mCreator->getHlmsManager());
	}else{
		Mparser->setData(vec,this);
		Mparser->parse(mCreator->getHlmsManager());
	}


//	for (size_t i = 0; i < NUM_DS_TEXTURE_TYPES; ++i)
//		textures[i].samplerBlock = mSamplerblocks[i];


	if(this->materialParams->size()==0&&this->textureParams->size()==0){
		safeForRewrite=true;

	}
	createMaterial();
}
//-----------------------------------------------------------------------------------
void DSDatablock::calculateHash() {
	IdString hash;

	DSBakedTexturesArray::const_iterator itor = mBakedTextures.begin();
	DSBakedTexturesArray::const_iterator end = mBakedTextures.end();
	String dope = "";
	while (itor != end) {
		hash += IdString(itor->texture->getName());
		hash += IdString(itor->samplerBlock->mId);
		dope = itor->texture->getName();
		++itor;

	}

	if (mTextureHash != hash.mHash) {
		mTextureHash = hash.mHash;

	}

}
HlmsTextureManager::TextureLocation DSDatablock::setTexture(const String &name,
		DSTextureType textureType, DSTextureParam * paramptr) {
	const HlmsTextureManager::TextureMapType texMapTypes[NUM_DS_TEXTURE_TYPES] =
			{ HlmsTextureManager::TEXTURE_TYPE_DIFFUSE,
					HlmsTextureManager::TEXTURE_TYPE_NORMALS,
					//specular
					HlmsTextureManager::TEXTURE_TYPE_DIFFUSE,
					//glow
					HlmsTextureManager::TEXTURE_TYPE_DIFFUSE, };

	HlmsManager *hlmsManager = mCreator->getHlmsManager();
	HlmsTextureManager *hlmsTextureManager = hlmsManager->getTextureManager();




	HlmsTextureManager::TextureLocation texLocation =
			hlmsTextureManager->createOrRetrieveTexture(name,
					texMapTypes[textureType]);

	//assert( texLocation.texture->isTextureTypeArray() || textureType == PBSM_REFLECTION );
	//HlmsTextureManager::TextureLocation texLocation;
	return texLocation;
}
DSDatablock::~DSDatablock() {
	// TODO Auto-generated destructor stub
}

//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
void DSDatablock::markForUpdate() {
	this->status = DIRTY;
}
void DSDatablock::syncWithGPU() {
	for (uint i = 0; i < materialParams->size(); i++) {
		//update only for dynamic Params, like AutoParams
		materialParams->at(i)->update();
	}

	if (!UpdateAllowed) {
		//todo assign datablocks to Pass
		//std::cout <<"Warning, only 1 update per frame allowed";

		return;
	}
	if (this->status == CLEAN && staticMaterial) {
		return;
	}
	for (uint i = 0; i < textureParams->size(); i++) {
		textureParams->at(i)->update();
	}

	GPUPointer *g = new GPUPointer();

	g->passBufferPtr = reinterpret_cast<float*>(this->MaterialBuffer->map(0,
			getMaterialBufferSize()));
//	*g->passBufferPtr++ = IDColour->r;
//	*g->passBufferPtr++ = IDColour->g;
//	*g->passBufferPtr++ = IDColour->b;
//	*g->passBufferPtr++ = IDColour->a;
//
	g->map(IDColour->r, 1);
	g->map(IDColour->g, 1);
	g->map(IDColour->b, 1);
	g->map(IDColour->a, 1);
	float * start = g->passBufferPtr;

	//************************Vec4*****************************************************

	for (uint i = 0; i < materialParams->size(); i++) {
//		*g->passBufferPtr++ = vec4Params->at(i).data[0];
//		*g->passBufferPtr++ = vec4Params->at(i).data[1];
//		*g->passBufferPtr++ = vec4Params->at(i).data[2];
//		*g->passBufferPtr++ = vec4Params->at(i).data[3];
		//g->map(vec4Params->at(i).data);
		materialParams->at(i)->map(g);
	}

	//*******************Texture **************************************************
	for (uint i = 0; i < textureParams->size(); i++) {

		uint val = textureParams->at(i)->getActiveLocation()->y;

		float valf = reinterpret_cast<float &>(val);

		g->map(valf, 4);

		//*g->passBufferPtr+=16;

		g->map(textureParams->at(i)->getTextMat());
		//for(int i=0;i<4;i++){

		//}

		//g->map(textureParams->at(i).getTextMat());

		if (textureParams->at(i)->isMultiTex()) {
			for (int ii = 0; ii < textureParams->at(i)->getNumTextures(); ii++) {
				val = textureParams->at(i)->getLocation(ii)->y;
				valf = reinterpret_cast<float &>(val);

				//*g->passBufferPtr++ = valf;
				g->map(valf, 4);

			}
		}
	}

	//deprecated
	//*******************Autoparam **************************************************
//	for (uint i = 0; i < autoParams->size(); i++) {
//		autoParams->at(i)->uploadParamData(g);
//	}

	this->MaterialBuffer->unmap(UO_KEEP_PERSISTENT);

	this->UpdateAllowed = false;
	this->status = CLEAN;

}
float DSDatablock::randf() {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}
float DSDatablock::randf2() {
	float ret = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	if (ret < 0.25) {
		return 0;
	}
	if (ret < 0.5) {
		return 0.25;
	}
	if (ret < 0.75) {
		return 0.5;
	}
	if (ret < 1) {
		return 0.75;
	}

//	}
	return 1;
}
Ogre::Vector3 DSDatablock::randv() {
	Ogre::Vector3 retv;
	float ret = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	retv.x = ret;

	ret = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	retv.z = ret;

	ret = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	retv.y = ret;

	retv.normalise();

	return retv;
}

Ogre::Vector3 DSDatablock::randv2() {
	Ogre::Vector3 retv;
	retv.x = randf2();
	retv.y = randf2();
	retv.z = randf2();
	return retv;
}

Ogre::Vector3 DSDatablock::randv3() {
	Ogre::Vector3 retv;
	if ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) < 0.1) {
		retv.x = 0;
		retv.y = 0;
		retv.z = 0;
	} else {
		retv.x = 0.6;
		retv.y = 0.3;
		retv.z = 0.3;

	}
	return retv;
}
void DSDatablock::bakeTextures(
		) {
//void DSDatablock::bakeTextures() {
	//The shader might need to be recompiled (mTexToBakedTextureIdx changed).
	//We'll need to flush.
	//Most likely mTexIndices also changed, so we need to update the const buffers as well
	mBakedTextures.clear();

	for (size_t i = 0; i < textureParams->size(); ++i) {
		for (size_t ii = 0; ii < textureParams->at(i)->getNumTextures(); ++ii) {

			if (!textureParams->at(i)->getTexture(ii)->texture.isNull()) {



				DSBakedTexturesArray::const_iterator itor = std::find(
						mBakedTextures.begin(), mBakedTextures.end(),
						*textureParams->at(i)->getTexture(ii));

				if (itor == mBakedTextures.end()) {
					textureParams->at(i)->getLocation(ii)->x =
							mBakedTextures.size();
					mBakedTextures.push_back(
							 *textureParams->at(i)->getTexture(ii));
				} else {
					textureParams->at(i)->getLocation(ii)->x = itor
							- mBakedTextures.begin();
				}
			} else {
				textureParams->at(i)->getLocation(ii)->x = NUM_DS_TEXTURE_TYPES;
			}

		}
	}

	calculateHash();
	flushRenderables();
	markForUpdate();

}

void DSDatablock::initializeParamTypes() {

}
void DSDatablock::createConstBuffer(HlmsDS *creator) {

	this->MaterialBuffer = creator->mVaoManager->createConstBuffer(
			getMaterialBufferSize(), BT_DYNAMIC_DEFAULT, 0, false);

}

ConstBufferPacked *DSDatablock::getMaterialBuffer() {
	return MaterialBuffer;
}
size_t DSDatablock::getMaterialBufferSize() {
	size_t size = materialParams->size() * sizeof(float) * 4;
	size += sizeof(float) * 4;
	for (int i = 0; i < materialParams->size(); i++) {
		size += materialParams->at(i)->getSize();
	}

	size += materialParams->size() * sizeof(uint);
	size = 16 * 1024;
	return size;
}

void DSDatablock::setData(const HlmsParamVec& params) {

	String paramVal;

	for (std::vector<DSMaterialParam *>::iterator it = materialParams->begin();
			it != materialParams->end(); ++it) {

		if (Hlms::findParamInVec(params, IdString((*it)->getParamName()),
				paramVal)) {

			DSMaterialParam* param = (*it);
			Mparser->initParam(paramVal, param);
		}

	}
	markForUpdate();

}

void DSDatablock::frameEnded() {
	if (!staticMaterial) {
		status = DIRTY;
	}
	this->UpdateAllowed = true;
}
void DSDatablock::createTexIndexPropertys(bool caster) {

	int index = 0;

	DSPropertyParam *param =new DSPropertyParam(DSProperty::MaxTextureParams,
			MaxTextureParams, NULL);

	propertyParams->push_back(param);
	param =new DSPropertyParam(DSProperty::NumTextureParams, textureParams->size(),
	NULL);
	propertyParams->push_back(param);
	int size=textureParams->size();
	assert(size>-1);
	for (uint i = 0; i < textureParams->size(); i++) {

//			String cast;          // string which will contain the result
//			stringstream convert;   // stream used for the conversion
//			convert << ii; // insert the textual representation of 'Number' in the characters in the stream
//
//			cast = convert.str();

		DSTextureParam * tex = textureParams->at(i);
		DSPropertyParam * param = new DSPropertyParam(
				DSProperty::TextureLoc + tex->getPostFix(),
				tex->getLocation(0)->x + 1, NULL);
		propertyParams->push_back(param);

		param = new DSPropertyParam(DSProperty::TextureValLoc + tex->getPostFix(),
				tex->getLocation(0)->x, NULL);
		propertyParams->push_back(param);

		param = new DSPropertyParam(DSProperty::TextureDefines + tex->getPostFix(),
				i,
				NULL);
		propertyParams->push_back(param);

		if (textureParams->at(i)->isMultiTex()) {

			stringstream s;
			s << DSProperty::NumSubTextures << i;

			DSPropertyParam *param = new DSPropertyParam(s.str(),
					textureParams->at(i)->getNumTextures(), NULL);
			propertyParams->push_back(param);

		}
	}

}
void DSDatablock::createVec4IndexPropertys(bool Caster) {

	int index = 0;

	//************************Might be obsolete**********************************************
	DSPropertyParam *param =new  DSPropertyParam(DSProperty::MaxVec4Params,
			MaxVec4Params, NULL);
	propertyParams->push_back(param);

	param =new  DSPropertyParam(DSProperty::NumVec4Params, materialParams->size(),
	NULL);
	propertyParams->push_back(param);

	for (int i = 0; i < materialParams->size(); i++) {

		DSMaterialParam * vec = materialParams->at(i);
		String prefix = "";
		if (!vec->isNoPreFix()) {
			prefix = DSProperty::Vec4Defines;
		}

		DSPropertyParam* param = new DSPropertyParam(prefix + vec->getPostFix(),
				i + 1, NULL);
		propertyParams->push_back(param);
		param = new DSPropertyParam(DSProperty::Vec4ValDefines + vec->getPostFix(),
				i, NULL);
		propertyParams->push_back(param);
	}

	//******************************************************************************************
	stringstream s;

	String value;

	for (int i = 0; i < materialParams->size(); i++) {
		DSMaterialParam * vec = materialParams->at(i);
		String prefix = "";
		if (!vec->isNoPreFix()) {
			prefix = DSProperty::Vec4Defines;
		}
		s << "\t "<<vec->getTypeString(0)<<" " << prefix << vec->getPostFix() <<vec->getArrayString(0)<< ";\n";

	}
	this->propertyParams->push_back(param);

	value = Ogre::String(s.str());
	DSPieceParamType * type = new DSPieceParamType(
			(DSProperty::MaterialVec4Params), PixelShader);
	DSPieceParam * piece = new DSPieceParam(DSProperty::MaterialVec4Params, value,
			type);

	pieceParams->push_back(piece);

}

void DSDatablock::setProperties() {
}
void DSDatablock::initializeProperties() {
}
void DSDatablock::setParam(Ogre::String allocator, Ogre::String allocator1) {
	Ogre::HlmsParamVec params = Ogre::HlmsParamVec();
	params.push_back(
			std::pair<IdString, String>(IdString(allocator), allocator1));
	std::sort(params.begin(), params.end());
	setData(params);
}
void DSDatablock::setParam(Ogre::String allocator, Ogre::Vector4 retv) {
	char * str = new char[100];
	int numvec=0;

	for (std::vector<DSMaterialParam *>::iterator it = materialParams->begin();
			it != materialParams->end(); ++it) {
		DSMaterialParam* param = (*it);

		if (param->getParamName().compare(allocator)==0) {
			numvec=param->getNumVec();
		}

	}
	if(numvec==0){
		return;
	}
	if(numvec==1)
		sprintf(str, "%f", retv.x);

	if(numvec==2)
		sprintf(str, "%f %f", retv.x, retv.y);

	if(numvec==3)
		sprintf(str, "%f %f %f", retv.x, retv.y, retv.z);

	if(numvec==4)
		sprintf(str, "%f %f %f %f", retv.x, retv.y, retv.z, retv.w);

	setParam(allocator, str);
}

void DSDatablock::setParam(Ogre::String allocator, float float1) {
	char * str = new char[100];

	sprintf(str, "%f", float1);

	setParam(allocator, str);
}

float* DSDatablock::getAutoParam(uint id) {
	//if (id < autoParams->size()) {
	//	return autoParams->at(id)->data;
	//}
	return 0;
}

const Ogre::Vector4 * const DSDatablock::getParam(Ogre::String allocator) {
	for (std::vector<DSMaterialParam *>::iterator it = materialParams->begin();
			it != materialParams->end(); ++it) {
		if ((*it)->getParamName().compare(allocator) == 0) {
			return new Vector4((*it)->data);
		}

	}
	return NULL;
}

DSDatablock::Status DSDatablock::getStatus() const {
	return status;
}

void DSDatablock::setStatus(DSDatablock::Status status) {
	this->status = status;
}
bool DSDatablock::isDirty() {
	return (getStatus() == DIRTY);
}
void DSDatablock::clear() {
	assert(this->safeForRewrite);
	mDSCreator->mVaoManager->destroyConstBuffer(this->MaterialBuffer);
	this->MaterialBuffer=NULL;
//	this->MaterialBuffer = creator->mVaoManager->createConstBuffer(
//				getMaterialBufferSize(), BT_DYNAMIC_DEFAULT, 0, false);
}
ParamVecParser*& DSDatablock::getMparser() {
	return Mparser;
}

void DSDatablock::setMparser(ParamVecParser* mparser) {
	Mparser = mparser;
}


void DSDatablock::createMaterial() {
	if(this->materialParams->size()==0&&this->textureParams->size()==0){
		safeForRewrite=true;

	}else{
		safeForRewrite=false;
	}
	createConstBuffer(creator);

	bakeTextures();

	calculateHash();
	for (uint i = 0; i < textureParams->size(); i++) {
		if( textureParams->at(i)->isDynamic()){
			staticMaterial=false;
		}

	}
	createTexIndexPropertys(false);
	createVec4IndexPropertys(false);
	//createTexIndexPropertys(true);
	//createVec4IndexPropertys(true);
	TextureSize = mBakedTextures.size();

	setProperties();

}
Ogre::String DSDatablock::getDName() {
#ifdef OGRE_DEBUG_MODE
	return getName().getFriendlyText();
#endif
	return getName().getReleaseText();
}
void DSDatablock::generateControl(bool staticd) {
	for(int i=0;i<16;i++){
		dynamic_control[i]= (char)('a' + rand()%26);
	}
	if(staticd){
		for(int i=0;i<16;i++){
			static_control[i]=dynamic_control[i];
		}
	}

}
const HlmsSamplerblock* DSDatablock::getSamplerBlock(HlmsSamplerblock sb) {
	return mCreator->getHlmsManager()->getSamplerblock(sb);
}

const HlmsSamplerblock* DSDatablock::getSamplerBlock() {
	HlmsSamplerblock sb;
	return mCreator->getHlmsManager()->getSamplerblock(sb);
}
 HlmsDS* DSDatablock::getDsCreator()  {
	return mDSCreator;
}
 bool DSDatablock::isJsonMaterial() const {
 	return JsonMaterial;
 }

 void DSDatablock::setJsonMaterial(bool jsonMaterial) {
 	JsonMaterial = jsonMaterial;
 }


} /* namespace Ogre */

