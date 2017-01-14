/*
 * DSVec4Param.h
 *
 *  Created on: 03.03.2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSVEC4PARAM_H_
#define SRC_DATABLOCKS_DATA_DSVEC4PARAM_H_

#include <OgreString.h>
#include "OgreVector4.h"
namespace Ogre {
class DSMaterialParamType;
class DSVec4Param {
	DSMaterialParamType* type;

	Ogre::String postFix;
	int numVec;
	Ogre::String paramName;
	Vector4 defaultVal;

public:
	float data[4];

	DSVec4Param(	DSMaterialParamType* type);
	virtual ~DSVec4Param();
	 String& getPostFix() ;
	void setPostFix(String& postFix);
	int getNumVec() const;
	void setNumVec(int numVec);
	const Ogre::String& getParamName() const;
	void setParamName(const Ogre::String& paramName);
	const Vector4& getDefaultVal() const;
	void setDefaultVal(const Vector4& defaultVal);
};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSVEC4PARAM_H_ */
