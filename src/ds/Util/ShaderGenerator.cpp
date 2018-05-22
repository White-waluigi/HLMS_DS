/*
 * ShaderGenerator.cpp
 *
 *  Created on: Jan 24, 2018
 *      Author: marvin
 */

#include "ShaderGenerator.h"
#include "ShaderKeyWordBank.h"
namespace Ogre {

ShaderGenerator::ShaderGenerator() {
	// TODO Auto-generated constructor stub

}

DSResource::ShaderPiece * ShaderGenerator::getDataTypeShader(DSResource::dataType dataType, String name,String prefix,IdString keyword, int ArraySize) {
	DSResource::ShaderPiece * res=new DSResource::ShaderPiece();

	//todo hlsl shaders
	//Add new Shaderpiece to the bottom of existing Shaderpiece;

	res->mConflict=DSResource::ShaderPiece::APPEND;
	Ogre::String array= ArraySize==-1?"":Ogre::String("[")+Ogre::StringConverter::toString(ArraySize)+Ogre::String("]");
	res->mCode["glsl"]=toString(dataType)+" "+prefix+name+array+";\n";
	res->mShaderKeyWord=keyword;
	return res;
}

String ShaderGenerator::toString(DSResource::dataType dataType) {
	//Turn datatype into String
	if(dataType==DSResource::SINGLEFLOAT){
		return "float";
	}else if(dataType==DSResource::VEC4){
		return "vec4";
	}else if(dataType==DSResource::MAT4){
		return "mat4";
	}else if(dataType==DSResource::UINT){
		return "uint";
	}else if(dataType==DSResource::UVEC4){
		return "uvec4";
	}else{}

	return "error Custom is an invalid Datatype";

}

ShaderGenerator::~ShaderGenerator() {
	// TODO Auto-generated destructor stub
}
DSResource::ShaderPiece* Ogre::ShaderGenerator::genTestFunction(IdString keyword,Vector4 color,String offset) {
		DSResource::ShaderPiece * res=new DSResource::ShaderPiece();

		//todo hlsl shaders
		//Add new Shaderpiece to the bottom of existing Shaderpiece;

		res->mConflict=DSResource::ShaderPiece::INSERT;
		res->mCode["glsl"]="\tvec4 getTestFunc"+offset+"(){\n"+"return vec4("+StringConverter::toString(color.x) +","+StringConverter::toString(color.y) +","+StringConverter::toString(color.z) +","+StringConverter::toString(color.w) +");"    +"\n}";
		res->mShaderKeyWord=keyword;
		return res;
}
DSResource::ShaderPiece* Ogre::ShaderGenerator::genTextureFunction() {
	DSResource::ShaderPiece * res=new DSResource::ShaderPiece();

	//Insert into predefined place (is recursive)
	res->mConflict=DSResource::ShaderPiece::INSERT;

	res->mCode["glsl"]="vec4 getDSTexture(int id,vec2 uv){\n${@REPLACE} \n\treturn vec4(1,0,0,0);\n}";
	res->mShaderKeyWord=ShaderKeyWordBank::TEXTUREFUNCTION;
	return res;

}

DSResource::ShaderPiece* Ogre::ShaderGenerator::AddTextureFunction(
		String texturename,int resid, int x) {
	DSResource::ShaderPiece * res=new DSResource::ShaderPiece();
	res->mConflict=DSResource::ShaderPiece::INSERT;

	res->mCode["glsl"]="\n\tif(id=="+StringConverter::toString(resid)+"){\n\t\treturn texture(textureMapsArray["+StringConverter::toString(x)+"],vec3(uv,material.texloc_"+texturename+".x));\n\t}\n${@REPLACE}";
	res->mShaderKeyWord=ShaderKeyWordBank::TEXTUREFUNCTION;
	return res;

}
} /* namespace Ogre */


