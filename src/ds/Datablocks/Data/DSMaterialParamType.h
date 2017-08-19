/*
 * DSVec4ParamType.h
 *
 *  Created on: 03.03.2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSMATERIALPARAMTYPE_H_
#define SRC_DATABLOCKS_DATA_DSMATERIALPARAMTYPE_H_
#include "OgreString.h"
#include "OgreVector4.h"

#include "Datablocks/Data/DSMaterialParam.h"
namespace Ogre {

class DSMaterialParamType {
public:
	Ogre::String paramName;
	int numVec;
	DSMaterialParam::DSMType  type=DSMaterialParam::VEC4;
	bool array=false;

	Ogre::String postFix;
	Ogre::Vector4 defaultVal=Vector4(0,0,0,0);
	DSMaterialParamType();
	virtual ~DSMaterialParamType();

};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSMATERIALPARAMTYPE_H_ */
