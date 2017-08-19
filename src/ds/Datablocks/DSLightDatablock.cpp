/*
 * DSLightDatablock.cpp
 *
 *  Created on: 20.03.2016
 *      Author: marvin
 */

#include <OgreStableHeaders.h>
#include <OgreHlmsManager.h>
#include <OgreHlmsTextureManager.h>
#include <OgreTexture.h>
#include <OgreLogManager.h>
#include <Vao/OgreVaoManager.h>
#include <Vao/OgreConstBufferPacked.h>
#include <Vao/OgreTexBufferPacked.h>


#include "Datablocks/DSLightDatablock.h"
#include "OgreHlmsDS.h"
#include "Datablocks/Data/DSTextureParam.h"
#include "Datablocks/Data/DSTextureParamType.h"
#include "Datablocks/Data/DSVec4Param.h"
#include "Datablocks/Data/DSMaterialParamType.h"
#include "Light/DSLight.h"
#include "Datablocks/Data/DSMaterialParam.h"


namespace Ogre {

DSLightDatablock::DSLightDatablock(IdString name, HlmsDS *creator,
		const HlmsMacroblock *macroblock, const HlmsBlendblock *blendblock,
		const HlmsParamVec &params) :
		DSDatablock(name, creator, macroblock, blendblock, params, true) {

	dbtype = DS_DATABLOCK_LIGHT;
	initializeMaterial(params, creator);


	HlmsManager *hlmsManager = mCreator->getHlmsManager();

	this->updateable=true;
}

DSLightDatablock::~DSLightDatablock() {
	// TODO Auto-generated destructor stub
}

void DSLightDatablock::initializeParamTypes() {


	{
		DSTextureParamType * type;

		type = new DSTextureParamType();
		type->paramName = "gobo_map";
		type->postFix = "gobo_map";
		type->textureType = DS_TEXTURE_DIFFUSE;
		type->textureMapType = HlmsTextureManager::TEXTURE_TYPE_DIFFUSE;
		textureParamTypes->push_back(*type);
	}
	{
		DSMaterialParamType* type;

		type = new DSMaterialParamType();
		type->paramName = "position";
		type->postFix = "position";
		type->numVec = 4;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "diffuse";
		type->postFix = "diffuse";
		type->numVec = 4;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "specular";
		type->postFix = "specular";
		type->numVec = 4;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "attenuation";
		type->postFix = "attenuation";
		type->numVec = 4;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "spotdirection";
		type->postFix = "spotdirection";
		type->numVec = 4;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "spotparams";
		type->postFix = "spotparams";
		type->numVec = 4;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "lightparams";
		type->postFix = "lightparams";
		type->numVec = 4;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "lightsettings";
		type->postFix = "lightsettings";
		type->numVec = 4;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "shadowParams";
		type->postFix = "shadowParams";
		type->numVec = 4;
		vec4ParamTypes->push_back(*type);


		type = new DSMaterialParamType();
		type->paramName = "shadowQualityParams";
		type->postFix = "shadowQualityParams";
		type->numVec = 4;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "shadowRes";
		type->postFix = "shadowRes";
		type->array=true;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "shadowDr";
		type->postFix = "shadowDr";
		type->array=true;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "shadowMat";
		type->postFix = "shadowMat";
		type->array=true;
		type->type=DSMaterialParam::MAT4;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "goboMat";
		type->postFix = "goboMat";
		type->array=false;
		type->type=DSMaterialParam::MAT4;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "shadowPssmSplits";
		type->postFix = "shadowPssmSplits";
		type->type = DSMaterialParam::SINGLEFLOAT;
		type->array=true;

		vec4ParamTypes->push_back(*type);


		type = new DSMaterialParamType();
		type->paramName = "test";
		type->postFix = "test";
		type->numVec = 4;
		vec4ParamTypes->push_back(*type);

//		type = new DSMaterialParamType();
//		type->paramName = "campos0";
//		type->postFix = "campos0";
//		type->numVec = 4;
//		vec4ParamTypes->push_back(*type);
//
//		type = new DSMaterialParamType();
//		type->paramName = "campos1";
//		type->postFix = "campos1";
//		type->numVec = 4;
//		vec4ParamTypes->push_back(*type);
//
//		type = new DSMaterialParamType();
//		type->paramName = "campos2";
//		type->postFix = "campos2";
//		type->numVec = 4;
//		vec4ParamTypes->push_back(*type);



	}
	{
		DSPropertyParamType type= DSPropertyParamType("Shadow_ID");
		propertyParamTypes->push_back(type);

		type= DSPropertyParamType("Shadow_Atlas_ID");
		propertyParamTypes->push_back(type);

	}
	{
		DSPieceParamType type= DSPieceParamType("custom_post_material",PixelShader);
		pieceParamTypes->push_back(type);



		type= DSPieceParamType("Shadow_ID",PixelShader);
		pieceParamTypes->push_back(type);
	}
}

Ogre::Light::LightTypes DSLightDatablock::getType() {
	String paramVal;
	for (uint i = 0; i < materialParams->size(); i++) {

		if (materialParams->at(i)->getParamName().compare("lightparams") == 0) {

			typedef union {
				uint i;
				float f;
			} type_;

			type_ type;
			type.f = materialParams->at(i)->data[0];

			return (Ogre::Light::LightTypes) type.i;

		}

	}
	return Ogre::Light::LT_SPOTLIGHT;

}
void DSLightDatablock::setProperties() {
	 Ogre::String lightNames[5]={"Direc", "Point", "Spot","Rim","Ambient"};




		bool notransf=false;
		if (getType()== Ogre::Light::LT_DIRECTIONAL) {
			notransf=true;
		}
		if (getType()== 4) {
			notransf=true;
		}
		if(notransf){
			DSPropertyParamType * type=new DSPropertyParamType("LIGHTTYPE");
			DSPropertyParam *param=new DSPropertyParam(DSProperty::noTransf,1,type);
			this->propertyParams->push_back(param);

		}

	DSPropertyParamType * type=new DSPropertyParamType("LIGHTTYPE");



	  char buff[100];

	Ogre::String key="LIGHTTYPE_"+lightNames[getType()];

	DSPropertyParam *param=new DSPropertyParam(key,1,type);

	this->propertyParams->push_back(param);
}

void DSLightDatablock::initializeProperties() {
}

} /* namespace Ogre */
