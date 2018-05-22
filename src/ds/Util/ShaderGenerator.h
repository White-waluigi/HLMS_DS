/*
 * ShaderGenerator.h
 *
 *  Created on: Jan 24, 2018
 *      Author: marvin
 *
 *  Toolbox for generating Shader Code in multiple Languages
 *  based on paramters
 */

#ifndef SRC_DS_UTIL_SHADERGENERATOR_H_
#define SRC_DS_UTIL_SHADERGENERATOR_H_
#include "../Modules/Resources/DSResource.h"


namespace Ogre {

class ShaderGenerator {
public:

	//generate datatype definition for shader ex: float test[23];
	static DSResource::ShaderPiece* getDataTypeShader(DSResource::dataType,String name, String prefix,IdString keyword,int ArraySize=-1);
	//simply writes out datatype
	static String toString(DSResource::dataType);

	//generates a simple function, used to test shader pieces
	static DSResource::ShaderPiece* genTestFunction(IdString keyword,Vector4 color,String offset);

	//generates a helper function that can be called in the shader and returns the right for the selected texture texelcolor for the current pixel
	static DSResource::ShaderPiece* genTextureFunction();
	//add texture to the texture function
	static DSResource::ShaderPiece* AddTextureFunction(String texturename,int resid, int x);

	//do nothing
	ShaderGenerator();
	virtual ~ShaderGenerator();
};

} /* namespace Ogre */

#endif /* SRC_DS_UTIL_SHADERGENERATOR_H_ */
