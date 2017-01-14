/*
 * JSONMaterialCreator.cpp
 *
 *  Created on: Dec 25, 2016
 *      Author: marvin
 */

#include "JSONMaterialCreator.h"
#include "../../Datablocks/Data/DSMaterialParam.h"
#include "../../Datablocks/Data/DSTextureParam.h"
#include "../../Datablocks/Data/DSTextureParamType.h"

#include "../../Datablocks/Data/DSPropertyParam.h"

#include "../../Datablocks/Data/DSPieceParam.h"
#include "../../Datablocks/Data/DSBlocks/MappingBlocks/DSMappingBlock.h"
#include "../../OgreHlmsDS.h"

namespace Ogre {

JSONMaterialCreator::JSONMaterialCreator() {
	// TODO Auto-generated constructor stub
	this->helper=DSShaderGenerator::ShaderCode::getDefault("");
}

JSONMaterialCreator::~JSONMaterialCreator() {
	// TODO Auto-generated destructor stub
}

void JSONMaterialCreator::initializeMaterial(DSDatablock* db,
		MaterialTemplate* materialTemplate, const HlmsJson::NamedBlocks& blocks) {
	this->blocks=blocks;
	this->db=db;
	db->clear();

	DSPropertyParam *test=new DSPropertyParam();
	test->setKey(Ogre::String("IS_JSON"));
	test->setValue(1);
	db->propertyParams->push_back(test);



	try{
		initializeMeta(materialTemplate->data->getObject("meta").o);
		initializeSettings(materialTemplate->data->getObject("settings").o);
		initializeParams(materialTemplate->data->getObject("params").o);

	}catch(Ogre::Exception * e){
		//Ogre::LogManager::getSingleton().logMessage("Can't create Json-Material"+db->getDName()+", because "+e->getFullDescription(),Ogre::LML_TRIVIAL,LML_CRITICAL);
		std::cout<<"Can't create Json-Material"+db->getDName()+", because "+e->getFullDescription()<<"\n";
	}

	DSPieceParam* param=new DSPieceParam("param_helper_custom",helper.code[db->getDsCreator()->getShaderProfile()],PixelShader);

	db->pieceParams->push_back(param);

	db->createMaterial();



	db->generateControl();
	db->setJsonMaterial(true);


}

void JSONMaterialCreator::initializeMeta(MT_MultiData* mt) {
	//******************************************************************************************************************

}

void JSONMaterialCreator::initializeSettings(MT_MultiData* mt) {
	if(mt->getBool("opacity")){
		quickSet("manual-opacity");
	}
	if(mt->getBool("opacity-sharp")){
		quickSet("opacity-sharp");
	}


	String macroname=mt->getDataD("macroblock","").s;
	if(mt->has("macroblock")&&blocks.macroblocks.find(LwConstString::FromUnsafeCStr(macroname.c_str()))!=blocks.macroblocks.end()){

		const Ogre::HlmsMacroblock mb=*blocks.macroblocks[LwConstString::FromUnsafeCStr(macroname.c_str())];
		db->setMacroblock(mb);
	}
	String blendblockname=mt->getDataD("blendblock","").s;
	if(mt->has("blendblock")&&blocks.blendblocks.find(LwConstString::FromUnsafeCStr(blendblockname.c_str()))!=blocks.blendblocks.end()){

		const Ogre::HlmsBlendblock bl=*blocks.blendblocks[LwConstString::FromUnsafeCStr(blendblockname.c_str())];
		db->setBlendblock(bl);
	}
	//******************************************************************************************************************

}

void JSONMaterialCreator::initializeParams(MT_MultiData* mts) {
	for(int i=0;i<mts->size();i++){
		MT_MultiData * mt= mts->getData(i).o;
		String type=mt->getData("type").s;
		if(type.compare("mappingblock")==0){initializeMappingBlock(mt);}
	}
	for(int i=0;i<mts->size();i++){

		MT_MultiData * mt= mts->getData(i).o;

		String type=mt->getData("type").s;
		if(type.compare("paired")==0){initializePaired(mt);}
		if(type.compare("variable")==0){initializeVariable(mt);}
		if(type.compare("texture")==0){initializeTexture(mt);}

		if(type.compare("piece")==0){initializePiece(mt);}
		if(type.compare("property")==0){initializeProperty(mt);}



	}
}

void JSONMaterialCreator::initializeMappingBlock(MT_MultiData* mt) {

	DSMappingBlock * mb=new DSMappingBlock(mt);

	db->MBs[mt->key]=mb;
	//******************************************************************************************************************
}

void JSONMaterialCreator::initializePaired(MT_MultiData* mt) {
	Ogre::String dtype=mt->getDataD("datatype","vec4").s;
	Ogre::String modulator=mt->getDataD("modulator","+").s;



	//******************************************************************************************************************

	HlmsSamplerblock sb;

	DSTextureParam * tparam=new DSTextureParam(sb,db);

	initTexData(tparam,mt->getObject("texData").o,mt->key,dtype);

	db->textureParams->push_back(tparam);

	//******************************************************************************************************************
	DSMaterialParam* param=new DSMaterialParam();

	initVarData(param,mt->getObject("varData").o,dtype,mt->key);

	//param.type = &*it;

	db->materialParams->push_back(param);
	if(mt->getBool("helper")){
		helper=DSShaderGenerator::ShaderCode::merge(helper,DSShaderGenerator::generatePairedHelperCode(param->getPostFix(),mt->key,dtype,modulator));
	}
}

void JSONMaterialCreator::initializeTexture(MT_MultiData* mt) {
	Ogre::String dtype="vec4";
	bool multi=false;


	if(mt->has("datatype")){
		dtype=mt->getData("datatype").s;
	}


	//******************************************************************************************************************
	HlmsSamplerblock sb;

	DSTextureParam * param=new DSTextureParam(sb,db);

	initTexData(param,mt->getObject("texData").o,mt->key,dtype);

	db->textureParams->push_back(param);

	if(mt->getBool("helper")){
		helper=DSShaderGenerator::ShaderCode::merge(helper,DSShaderGenerator::generateTextureHelperCode(param->getPostFix(),mt->key,"vec4"));
	}

}

void JSONMaterialCreator::initializeVariable(MT_MultiData* mt) {
	Ogre::String dtype="vec4";

	Ogre::String type="manual";


	if(mt->has("datatype")){
		dtype=mt->getData("datatype").s;
	}


	if(mt->has("varData")&&mt->getObject("varData").o->has("datatype")){
			type=mt->getObject("varData").o->getData("type").s;
	}
//******************************************************************************************************************


	DSMaterialParam* param=new DSMaterialParam();

	initVarData(param,mt->getObject("varData").o,dtype,mt->key);

	//param.type = &*it;

	db->materialParams->push_back(param);

	if(mt->getBool("helper")){
		helper=DSShaderGenerator::ShaderCode::merge(helper,DSShaderGenerator::generateVarHelperCode(param->postFix,param->getTypeString(0)));
	}

}

void JSONMaterialCreator::initializePiece(MT_MultiData* mt) {
	Ogre::String id=mt->getData("id").s;
	Ogre::String stype=mt->getData("shadertype").s;
	int stypei=0;
	Ogre::String name=mt->key;
	Ogre::String shadermap[5]={
		"vertex",
        "pixel",
        "geometry",
        "hull",
        "domain"};


	std::map<Ogre::String,Ogre::String> shader;
	MT_MultiData * shaderob=mt->getObject("shader").o;
	for(int i=0;i<shaderob->size();i++){
		Ogre::String s=shaderob->getData(i).o->getDataArrayAsString("code").s;
		shader[shaderob->getData(i).o->key]=s;
	}

	for(int i=0;i<5;i++){
		if(stype.compare(shadermap[i])==0){
			stypei=i;
		}
	}
	//******************************************************************************************************************
	DSPieceParam *param=new DSPieceParam(id,shader[db->getDsCreator()->getShaderProfile()],stypei);

	db->pieceParams->push_back(param);


}

void JSONMaterialCreator::initializeProperty(MT_MultiData* mt) {
	Ogre::String id=mt->key;
	int value=(mt->getData("value").f);

	//******************************************************************************************************************
	DSPropertyParam *param=new DSPropertyParam(id,value);

	db->propertyParams->push_back(param);

}

void JSONMaterialCreator::initVarData(DSMaterialParam* param, MT_MultiData* mt,Ogre::String datatype,Ogre::String key) {
	Ogre::String type=mt->getDataD("type","manual").s;

	MT_MultiData::retValMD* rvmd=NULL;
	float * data=0;

	int size=0;

	if(type.compare("manual")==0){
		if(datatype.compare("vec4")==0){
			size=4;
			param->array=false;

			param->type=DSMaterialParam::VEC4;
			param->paramName=key;
			param->postFix=key;


			rvmd= mt->getDataArray("value");


		}
	}
	assert(rvmd!=NULL);
	data=new float[size];
	for(int i=0;i<size;i++){
		data[i]=rvmd[i].f;
	}

	param->setData(data);
}

void JSONMaterialCreator::initTexData(DSTextureParam* param, MT_MultiData* mt,Ogre::String key,Ogre::String datatype) {
	Ogre::String type=mt->getDataD("type","file").s;
	Ogre::String file=mt->getDataD("file","error.png").s;
	assert(mt->has("file"));

	param->setName(file);

	Ogre::String mb=mt->getDataD("mappingblock","").s;

	if( mt->has("mappingblock") && db->MBs.find(mb) !=db->MBs.end()){
		param->setMappingBlock(db->MBs.at(mb));


	}

	String samplername=mt->getDataD("sampler","").s;



	if(mt->has("sampler")&&blocks.samplerblocks.find(LwConstString::FromUnsafeCStr(samplername.c_str()))!=blocks.samplerblocks.end()){


		HlmsSamplerblock sb=*db->getSamplerBlock(*blocks.samplerblocks[LwConstString::FromUnsafeCStr(samplername.c_str())]);
		param->setSamplerBlock(sb);
	}else{
		HlmsSamplerblock sb;
		sb.mU=Ogre::TAM_WRAP;
		sb.mV=Ogre::TAM_WRAP;
		sb.mW=Ogre::TAM_WRAP;
		sb.mCompareFunction=CMPF_LESS_EQUAL;
		param->setSamplerBlock(sb);

	}



	if(type.compare("file")==0){
		if(datatype.compare("vec4")==0){
			//TODO
			param->setPostFix(key);
			HlmsTextureManager::TextureLocation texloc=db->setTexture(file,DSDatablock::DS_TEXTURE_DIFFUSE);
			param->addTexture(&texloc);

		}else{
			assert(false&&" only vec4 supported at the moment");
		}
	}else{
		assert(false&&" only file supported at the moment");
	}



}

void JSONMaterialCreator::quickSet(Ogre::String allocator, int i) {
	DSPropertyParam* test=new DSPropertyParam();
	test->setKey(Ogre::String(allocator));
	test->setValue(i);
	db->propertyParams->push_back(test);
}

void JSONMaterialCreator::quickSet(Ogre::String allocator) {
	quickSet(allocator,1);
}

} /* namespace Ogre */
