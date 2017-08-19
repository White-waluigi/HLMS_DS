/*
 * DSVec4Param.cpp
 *
 *  Created on: 03.03.2016
 *      Author: marvin
 */

#include "Datablocks/Data/DSMaterialParamType.h"
#include "Datablocks/Data/DSVec4Param.h"

namespace Ogre {

 String& DSVec4Param::getPostFix()  {
	return postFix;
}

int DSVec4Param::getNumVec() const {
	return numVec;
}

const Ogre::String& DSVec4Param::getParamName() const {
	return paramName;
}

void DSVec4Param::setParamName(const Ogre::String& paramName) {
	this->paramName = paramName;
}

void DSVec4Param::setNumVec(int numVec) {
	this->numVec = numVec;
}

void DSVec4Param::setPostFix( String& postFix) {
	this->postFix = postFix;
}

DSVec4Param::~DSVec4Param() {
	// TODO Auto-generated destructor stub
}
Ogre::DSVec4Param::DSVec4Param(DSMaterialParamType* type) {
	this->postFix=type->postFix;
	this->numVec=type->numVec;
	this->paramName=type->paramName;
	this->defaultVal=type->defaultVal;
	this->type=NULL;
}

} /* namespace Ogre */

const Ogre::Vector4& Ogre::DSVec4Param::getDefaultVal() const {
	return defaultVal;
}

void Ogre::DSVec4Param::setDefaultVal(const Ogre::Vector4& defaultVal) {
	this->defaultVal = defaultVal;
}
