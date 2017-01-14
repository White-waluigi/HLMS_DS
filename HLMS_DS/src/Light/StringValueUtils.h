/*
 * StringValueUtils.h
 *
 *  Created on: 22.10.2015
 *      Author: marvin
 */

#ifndef PBSHLMS_DEFERREDSHADING_STRINGVALUEUTILS_H_
#define PBSHLMS_DEFERREDSHADING_STRINGVALUEUTILS_H_
#include "OgreColourValue.h"
#include "OgreVector4.h"
#include "OgreException.h"
#include "OgreArchive.h"
#include "stddef.h"
#include "OgreHlmsCommon.h"
namespace Ogre {

class StringValueUtils {
public:
	StringValueUtils();
	virtual ~StringValueUtils();

	static  String getColorStr(Ogre::ColourValue);
	static String getVectorStr(Ogre::Vector3);
	static String getVectorStr(Ogre::Vector4);
	static String getMatrixStr(Ogre::Matrix4);

	static float* getVectorV3(Ogre::String);
	static float* getVectorV4(Ogre::String);
	static float* getMatrixMat4x4(Ogre::String);



	static HlmsParamVec stringToParamVec(Ogre::String s);

};

} /* namespace Ogre */

#endif /* PBSHLMS_DEFERREDSHADING_STRINGVALUEUTILS_H_ */
