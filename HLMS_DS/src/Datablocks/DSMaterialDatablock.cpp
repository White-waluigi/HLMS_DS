/*
 * DSMaterialDatablock.cpp
 *
 *  Created on: 23.03.2016
 *      Author: marvin
 */


#include "../Datablocks/Data/DSMaterialParamType.h"
#include "OgreStableHeaders.h"

#include "DSMaterialDatablock.h"
#include "../OgreHlmsDS.h"
#include "OgreHlmsManager.h"
#include "OgreHlmsTextureManager.h"
#include "OgreTexture.h"
#include "OgreLogManager.h"
#include "Data/DSTextureParam.h"
#include "Data/DSTextureParamType.h"
#include "Data/DSVec4Param.h"
#include "Vao/OgreVaoManager.h"
#include "Vao/OgreConstBufferPacked.h"
#include "Vao/OgreTexBufferPacked.h"

namespace Ogre {

DSMaterialDatablock::DSMaterialDatablock(IdString name, HlmsDS *creator,
		const HlmsMacroblock *macroblock, const HlmsBlendblock *blendblock,
		const HlmsParamVec &params) :
		DSDatablock(name, creator, macroblock, blendblock, params, true) {

	dbtype = DS_DATABLOCK_MATERIAL;

	initializeMaterial(params, creator);


}
void DSMaterialDatablock::initializeParamTypes() {


	{
		DSTextureParamType * type;

		type = new DSTextureParamType();
		type->paramName = "diffuse_map";
		type->postFix = "diffuse_map";
		type->textureType = DS_TEXTURE_DIFFUSE;
		type->textureMapType = HlmsTextureManager::TEXTURE_TYPE_DIFFUSE;
		textureParamTypes->push_back(*type);

		type = new DSTextureParamType();

		type->paramName = "normal_map";
		type->postFix = "normal_map";

		type->textureType = DS_TEXTURE_NORMAL;
		type->textureMapType = HlmsTextureManager::TEXTURE_TYPE_DIFFUSE;
		textureParamTypes->push_back(*type);

		type = new DSTextureParamType();

		type->paramName = "specular_map";
		type->postFix = "specular_map";

		type->textureType = DS_TEXTURE_SPECULAR;
		type->textureMapType = HlmsTextureManager::TEXTURE_TYPE_DIFFUSE;
		textureParamTypes->push_back(*type);

		type->paramName = "glow_map";
		type->postFix = "glow_map";

		type->textureType = DS_TEXTURE_GLOW;
		type->textureMapType = HlmsTextureManager::TEXTURE_TYPE_DIFFUSE;
		textureParamTypes->push_back(*type);

		type->paramName = "opacity_map";
		type->postFix = "opacity_map";

		type->textureType = DS_TEXTURE_OPACITY;
		type->textureMapType = HlmsTextureManager::TEXTURE_TYPE_DIFFUSE;
		textureParamTypes->push_back(*type);

		type->paramName = "reflection_map";
		type->postFix = "reflection_map";

		type->textureType = DS_TEXTURE_REFLECTION;
		type->textureMapType = HlmsTextureManager::TEXTURE_TYPE_DIFFUSE;
		textureParamTypes->push_back(*type);
	}

	{
		DSMaterialParamType* type;

		type = new DSMaterialParamType();
		type->paramName = "diffuse";
		type->postFix = "diffuse";
		type->numVec = 4;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "specular";
		type->postFix = "specular";
		type->numVec =4;
		type->defaultVal =Vector4(0,0,0,32);
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "glow";
		type->postFix = "glow";
		type->numVec = 4;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "opacity";
		type->postFix = "opacity";
		type->numVec = 1;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "test";
		type->postFix = "test";
		type->numVec = 4;
		vec4ParamTypes->push_back(*type);

		type = new DSMaterialParamType();
		type->paramName = "reflection";
		type->postFix = "reflection";
		type->numVec = 1;
		vec4ParamTypes->push_back(*type);
	}
	{
		DSPropertyParamType type= DSPropertyParamType("rainbow");
		propertyParamTypes->push_back(type);

		type= DSPropertyParamType("opacity-diffuse");
		propertyParamTypes->push_back(type);

		type= DSPropertyParamType("opacity-cutoff");
		propertyParamTypes->push_back(type);

		type= DSPropertyParamType("opacity-sharp");
		propertyParamTypes->push_back(type);
	}
	{
		DSPieceParamType type= DSPieceParamType("custom_post_material",PixelShader);
		pieceParamTypes->push_back(type);


		type= DSPieceParamType("custom_pre_material",PixelShader);
		pieceParamTypes->push_back(type);
	}
}

DSMaterialDatablock::~DSMaterialDatablock() {
	// TODO Auto-generated destructor stub
}

void DSMaterialDatablock::initializeProperties() {


}
void DSMaterialDatablock::setProperties() {


}
} /* namespace Ogre */
