/*
 * ParamVecParser.cpp
 *
 *  Created on: Nov 17, 2016
 *      Author: marvin
 */

#include <Vao/OgreVaoManager.h>
#include <Vao/OgreConstBufferPacked.h>
#include <Vao/OgreTexBufferPacked.h>
#include <OgreHlms.h>
#include <OgreHlmsDS.h>
#include <OgreHlmsManager.h>
#include <OgreHlmsTextureManager.h>
#include <OgreTexture.h>
#include <OgreLogManager.h>

#include "DBParser/ParamVecParser.h"
#include "Datablocks/DSDatablock.h"
#include "DBParser/DBParser.h"
#include "Datablocks/DSDatablock.h"
#include "Datablocks/Data/DSTextureParam.h"
#include "Datablocks/Data/DSTextureParamType.h"


#include "Datablocks/Data/DSMaterialParam.h"

#include "Light/StringValueUtils.h"

#include "Datablocks/Data/DSMaterialParam.h"
#include "Datablocks/Data/DSMaterialParamType.h"
#include "Datablocks/Data/DSBlocks/MappingBlocks/DSMappingBlock.h"
namespace Ogre {

ParamVecParser::ParamVecParser() {
	// TODO Auto-generated constructor stub

}

void ParamVecParser::parse(HlmsManager *hlmsManager) {

	String paramVal;
    assert( dynamic_cast<DSDatablock*>(hdb) );
    DSDatablock *db = static_cast<DSDatablock*>(hdb);


	HlmsParamVec::const_iterator iter = this->params.begin();

	for (std::vector<DSMaterialParamType>::iterator it =
			db->vec4ParamTypes->begin(); it != db->vec4ParamTypes->end(); ++it) {
		if (Hlms::findParamInVec(params, IdString(it->paramName), paramVal)) {
			//every Value is always a 4 float vector, to keep the padding simple (Since the few material values take barely any space, the overhead seams negliable)
			DSMaterialParamType type = (*it);
			DSMaterialParam* param = new DSMaterialParam(&type);
			//param.type = &*it;

			initParam(paramVal, param);
			db->materialParams->push_back(param);
		}

	}

	iter = params.begin();

	for (std::vector<DSPropertyParamType>::iterator it =
			db->propertyParamTypes->begin(); it != db->propertyParamTypes->end();
			++it) {
		if (Hlms::findParamInVec(params, IdString(it->paramName), paramVal)) {
			DSPropertyParam* param = new DSPropertyParam(it->propertyName,
					StringConverter::parseInt(paramVal), &*it);

			db->propertyParams->push_back(param);
		}

	}

	//DSPieceParam namep=DSPieceParam("db_name",this->getName().mDebugString,new DSPieceParamType("db_name",PixelShader));

	//this->pieceParams->push_back(namep);
//	DSPieceParam namev=DSPieceParam("db_name",this->getName().mDebugString,new DSPieceParamType("db_name",VertexShader));
//
//	this->pieceParams->push_back(namev);

	iter = params.begin();

	for (std::vector<DSPieceParamType>::iterator it = db->pieceParamTypes->begin();
			it != db->pieceParamTypes->end(); ++it) {
		if (Hlms::findParamInVec(params, IdString(it->paramName), paramVal)) {

			String s = paramVal;
			s.erase(remove(s.begin(), s.end(), '\''), s.end());
			DSPieceParam *param = new DSPieceParam(it->propertyName, s, &*it);

			db->pieceParams->push_back(param);
		}

	}

	int index = 0;
	stringstream s;
	String key = "autoparam0";

	while (Hlms::findParamInVec(params, IdString(key), paramVal)) {

		stringstream keystream;

		DSMaterialParam * param = db->APparser->getAutoParam(paramVal);
		param->setPostFix(key);
		param->setNoPreFix(true);
		s << "\t" << param->getParamString(index++) << "\n";
		//autoParams->push_back(param);
		db->materialParams->push_back(param);

		keystream << "autoparam" << index;
		key = Ogre::String(keystream.str());
		db->staticMaterial = false;
	}
	String materialVars(s.str());

	iter = params.begin();

	DSPieceParam* param =new DSPieceParam("AutoParams", materialVars,
			new DSPieceParamType("AutoParams", PixelShader));

	db->pieceParams->push_back(param);

	for (std::vector<DSTextureParamType>::iterator it =
			db->textureParamTypes->begin(); it != db->textureParamTypes->end(); ++it) {
		if (Hlms::findParamInVec(params, IdString(it->paramName), paramVal)) {

			initTextureUnit(paramVal, &*it, hlmsManager);

		}
	}
}
void ParamVecParser::initParam(String paramVal, DSMaterialParam* param) {
    assert( dynamic_cast<DSDatablock*>(hdb) );
    DSDatablock *db = static_cast<DSDatablock*>(hdb);
	int iters = 1;
	String CparamVal=paramVal;
	StringVector splitstr;




		if (param->getNumVec() == 1) {
			Real val = StringConverter::parseReal(CparamVal, 0);
			param->data[0] = val;
			param->data[1] = 0;
			param->data[2] = 0;
			param->data[3] = 0;

		}
		if (param->getNumVec() == 2) {
			Vector2 val = StringConverter::parseVector2(CparamVal,
					Vector2::UNIT_SCALE);
			param->data[0] = val.x;
			param->data[1] = val.y;
			param->data[2] = 0;
			param->data[3] = 0;

		}
		if (param->getNumVec() == 3) {
			Vector3 val = StringConverter::parseVector3(CparamVal,
					Vector3::UNIT_SCALE);
			param->data[0] = val.x;
			param->data[1] = val.y;
			param->data[2] = val.z;
			param->data[3] = 0;
		}
		if (param->getNumVec() == 4) {
			Vector4 val = StringConverter::parseVector4(CparamVal,
					Vector4::ZERO);
			param->data[0] = val.x;
			param->data[1] = val.y;
			param->data[2] = val.z;
			param->data[3] = val.w;

		}
		if(param->array || param->type!=DSMaterialParam::VEC4){



			param->setData(paramVal,-1);

		}

}
void ParamVecParser::initTextureUnit(String paramValD, DSTextureParamType * type,
		HlmsManager *hlmsManager) {
	HlmsParamVec vec = StringValueUtils::stringToParamVec(paramValD);
    assert( dynamic_cast<DSDatablock*>(hdb) );
    DSDatablock *db = static_cast<DSDatablock*>(hdb);
	String paramVal;
	DSTextureParam* param = new DSTextureParam(type,db);

	if (!Hlms::findParamInVec(vec, IdString("file"), paramVal))
		return;


	Ogre::String file_ = paramVal;

	param->setName(paramVal);

	int loops = 1;
	bool isMultitexture = false;
	if (Hlms::findParamInVec(vec, IdString("anim-texture"), paramVal)) {
		loops = StringConverter::parseReal(paramVal, 0);
		isMultitexture = true;
	}
	float speed = -1;
	if (Hlms::findParamInVec(vec, IdString("anim-uv"), paramVal)) {
		speed = StringConverter::parseReal(paramVal, 0);
		param->setAnimatedUv(true);
		param->anim.trans.x = speed;
		param->anim.trans.y = speed / 3.0;
		param->anim.transt = 100.0;
	}

	if (Hlms::findParamInVec(vec, IdString("scale-uv"), paramVal)) {
		Vector3 scale = StringConverter::parseVector3(paramVal,
				Vector3::UNIT_SCALE);
		Matrix4 mat = Matrix4::IDENTITY;
		mat.setScale(Vector3(scale));
		param->setTextMat(mat);
	}

	for (int i = 0; i < loops; i++) {
		Ogre::String file = file_;

		if (isMultitexture) {
			stringstream ss;
			ss << i;
			file = StringUtil::replaceAll(file, "@i", ss.str());
		}
		HlmsTextureManager::TextureLocation texloc = db->setTexture(file,
				param->getTextureType(), param);

		param->addTexture(&texloc);

		//mSamplerblocks[it->textureType] = hlmsManager->getSamplerblock( sb );
		//param->type =;
		//param->texture.samplerBlock = &param->getSamplerBlock();

	}

	HlmsSamplerblock sb = HlmsSamplerblock();
	sb.mU = param->getSamplerBlock().mU;
	sb.mV = param->getSamplerBlock().mV;
	sb.mW = param->getSamplerBlock().mW;



	if (Hlms::findParamInVec(vec, IdString("filter"), paramVal)) {
		if (paramVal.compare("none") == 0) {
			sb.setFiltering(TFO_NONE);
		} else if (paramVal.compare("trilinear") == 0) {
			sb.setFiltering(TFO_TRILINEAR);
		} else if (paramVal.compare("bilinear") == 0) {
			sb.setFiltering(TFO_BILINEAR);
		} else if (paramVal.compare("anistropic") == 0) {
			sb.setFiltering(TFO_ANISOTROPIC);
		}
	}

	param->setSamplerBlock(*hlmsManager->getSamplerblock(sb));

	if (Hlms::findParamInVec(vec, IdString("UVindex"), paramVal)) {
		param->setUVindex(StringConverter::parseReal(paramVal, 0));

	} else {
		param->setUVindex(0);
	}

	stringstream s;
	String key;
	s << DSProperty::TextureUVindex << param->getPostFix();

	key = Ogre::String(s.str());

	DSPropertyParam pparam = DSPropertyParam(key, param->getUVindex());
	//this->propertyParams->push_back(pparam);
	if (param->isAnimatedUv()) {
//		param->setAnimatedTexture(true);
//		param->setMaxAnim(loops);
//		param->setAnimInterval(200);
		MT_MultiData * mt=new MT_MultiData("ParamVecMB");

		mt->ao(new MT_MultiData("type","mappingblock"));

		MT_MultiData * mta=new MT_MultiData("animation");
		MT_MultiData * mta1=new MT_MultiData("PScroll");

		mta1->ao(new MT_MultiData("type","scroll"));
		mta1->ao(new MT_MultiData("speedx",param->anim.trans.x));
		mta1->ao(new MT_MultiData("speedy",param->anim.trans.y/3.0f));
		mta1->ao(new MT_MultiData("timescale",1));


		mta->ao(mta1);
		mt->ao(mta);


		param->setMappingBlock(new DSMappingBlock(mt));

	}
	if (isMultitexture&&loops>1) {
//		param->setAnimatedTexture(true);
//		param->setMaxAnim(loops);
//		param->setAnimInterval(200);
		MT_MultiData * mt=new MT_MultiData("ParamVecMB");

		mt->ao(new MT_MultiData("type","mappingblock"));

		MT_MultiData * mta=new MT_MultiData("animation");
		MT_MultiData * mta1=new MT_MultiData("PFrame");

		mta1->ao(new MT_MultiData("type","frame"));
		mta1->ao(new MT_MultiData("numUV",loops));
		mta1->ao(new MT_MultiData("fps",6));
		mta1->ao(new MT_MultiData("pulse",1));
		mta1->ao(new MT_MultiData("timescale",1));


		mta->ao(mta1);
		mt->ao(mta);


		param->setMappingBlock(new DSMappingBlock(mt));

	}
	if (param->isDynamic()) {
		db->staticMaterial = false;
	}
	db->textureParams->push_back(param);
}
ParamVecParser::~ParamVecParser() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */
