/*
 * DSPropertyParamType.cpp
 *
 *  Created on: 24.03.2016
 *      Author: marvin
 */

#include "DSPropertyParamType.h"

namespace Ogre {



DSPropertyParamType::DSPropertyParamType(String a) {
	this->paramName=a;
	this->propertyName=a;
}

DSPropertyParamType::DSPropertyParamType(String a,String b) {
	this->paramName=a;
	this->propertyName=b;
}

DSPropertyParamType::~DSPropertyParamType() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */
