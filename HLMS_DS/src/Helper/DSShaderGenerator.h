/*
 * DSShaderGenerator.h
 *
 *  Created on: Jan 3, 2017
 *      Author: marvin
 */

#ifndef SRC_HELPER_DSSHADERGENERATOR_H_
#define SRC_HELPER_DSSHADERGENERATOR_H_
#include "Ogre.h"

namespace Ogre {

class DSShaderGenerator {


public:
	class ShaderCode{
	public:
		std::map<Ogre::String,Ogre::String> code;
		static ShaderCode getDefault(Ogre::String);
		static ShaderCode merge(ShaderCode,ShaderCode);

	};
	static ShaderCode generateTextureHelperCode(Ogre::String varname,Ogre::String texname,Ogre::String datatype,Ogre::String modulator="" );
	static ShaderCode generateVarHelperCode(Ogre::String varname,Ogre::String datatype);
	static ShaderCode generatePairedHelperCode(Ogre::String varname,Ogre::String texname,Ogre::String datatype,Ogre::String modulator);



	DSShaderGenerator();
	virtual ~DSShaderGenerator();
};

} /* namespace Ogre */

#endif /* SRC_HELPER_DSSHADERGENERATOR_H_ */
