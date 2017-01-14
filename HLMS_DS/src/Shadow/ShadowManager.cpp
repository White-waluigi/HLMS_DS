/*
 * ShadowManager.cpp
 *
 *  Created on: Jul 14, 2016
 *      Author: marvin
 */

#include "ShadowManager.h"
namespace Ogre {

ShadowManager::ShadowManager(Config * config, HlmsDS * hlmsman) {
	// TODO Auto-generated constructor stub
	this->config = config;
	this->hlmsman = hlmsman;

	textureParams=new std::vector<DSTextureParamType>();
	vec4Params=new std::vector<DSMaterialParamType>();
	propertyParams=new std::vector<DSPropertyParam>();
	pieceParams=new std::vector<DSPieceParam>();

	DSMaterialParamType vParam;
	vParam.paramName = "diffuse";
	vParam.postFix = "diffuse";
	vec4Params->push_back(vParam);

	DSTextureParamType tParam;

	tParam.paramName = "opacity_map";
	tParam.postFix = "opacity_map";

	tParam.textureType = DSDatablock::DS_TEXTURE_OPACITY;
	tParam.textureMapType = HlmsTextureManager::TEXTURE_TYPE_DIFFUSE;
	textureParams->push_back(tParam);

	tParam.paramName = "diffuse_map";
	tParam.postFix = "diffuse_map";

	tParam.textureType = DSDatablock::DS_TEXTURE_OPACITY;
	tParam.textureMapType = HlmsTextureManager::TEXTURE_TYPE_DIFFUSE;
	textureParams->push_back(tParam);

}

void ShadowManager::generateProperties() {
	{
		//wont work until shadow UVs are fixed
		return;
		int index = 0;

		//************************Might be obsolete**********************************************
		DSPropertyParam param = DSPropertyParam(DSProperty::MaxVec4Params,
				DSDatablock::MaxVec4Params, NULL);
		propertyParams->push_back(param);

		param = DSPropertyParam(DSProperty::NumVec4Params, vec4Params->size(),
		NULL);
		propertyParams->push_back(param);

		for (int i = 0; i < vec4Params->size(); i++) {
			DSMaterialParamType * vec = &vec4Params->at(i);
			DSPropertyParam param = DSPropertyParam(
					DSProperty::Vec4Defines + vec->postFix, i + 1, NULL);
			propertyParams->push_back(param);
			param = DSPropertyParam(
					DSProperty::Vec4ValDefines + vec->postFix, i, NULL);
			propertyParams->push_back(param);
		}

		//******************************************************************************************
		stringstream s;

		String value;

		for (int i = 0; i < vec4Params->size(); i++) {
			DSMaterialParamType * vec = &vec4Params->at(i);

			s << "\t vec4 " << DSProperty::Vec4Defines << vec->postFix
					<< ";\n";

		}
		this->propertyParams->push_back(param);

		value = Ogre::String(s.str());
		DSPieceParamType * type = new DSPieceParamType(
				(DSProperty::MaterialVec4Params), PixelShader);
		DSPieceParam piece = DSPieceParam(DSProperty::MaterialVec4Params, value,
				type);

		pieceParams->push_back(piece);

	}

	{
		int index = 0;

		DSPropertyParam param = DSPropertyParam(DSProperty::MaxTextureParams,
				DSDatablock::MaxTextureParams, NULL);
		propertyParams->push_back(param);
		param = DSPropertyParam(DSProperty::NumTextureParams,
				textureParams->size(),
				NULL);
		propertyParams->push_back(param);

		for (uint i = 0; i < textureParams->size(); i++) {

//			String cast;          // string which will contain the result
//			stringstream convert;   // stream used for the conversion
//			convert << ii; // insert the textual representation of 'Number' in the characters in the stream
//
//			cast = convert.str();

			DSTextureParamType * tex = &textureParams->at(i);
			DSPropertyParam param = DSPropertyParam(
					DSProperty::TextureLoc + tex->postFix,
					0 + 1, NULL);
			propertyParams->push_back(param);

			param = DSPropertyParam(
					DSProperty::TextureValLoc + tex->postFix,
					0, NULL);
			propertyParams->push_back(param);

			param = DSPropertyParam(
					DSProperty::TextureDefines + tex->postFix, i,
					NULL);
			propertyParams->push_back(param);
//		cant use multitextures in shadows, becuase only one shader for shadows is generated
//		if (textureParams->at(i).isMultiTex()) {
//
//			stringstream s;
//			s << DSProperty::NumSubTextures << i;
//
//			DSPropertyParam param = DSPropertyParam(s.str(),
//					textureParams->at(i).getNumTextures(), NULL);
//			propertyParams->push_back(param);
//
//		}
		}
	}
}

ConstBufferPacked* ShadowManager::generateProperties(DSDatablock* db) {
}

void ShadowManager::bindTextures(CommandBuffer* cb, int texUnit) {
}

ShadowManager::~ShadowManager() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */
