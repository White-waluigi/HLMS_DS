/*
 * ShaderKeyWordBank.h
 *
 *  Created on: Jan 24, 2018
 *      Author: marvin
 *
 *  Just a static class for storing names of
 *  predefine ShaderKeyWords
 */

#ifndef SRC_DS_UTIL_SHADERKEYWORDBANK_H_
#define SRC_DS_UTIL_SHADERKEYWORDBANK_H_
#include "Ogre.h"
namespace Ogre {

class ShaderKeyWordBank {
public:
	static const String MATERIALDATAPARAM;
	static const String ADDITIONALFUNCTIONS;

	static const String MAINTEXARRAY;
	static const String NUMTEXTURES;
	static const String TEXTUREID;
	static const String TEXTUREFUNCTION;
};

} /* namespace Ogre */

#endif /* SRC_DS_UTIL_SHADERKEYWORDBANK_H_ */
