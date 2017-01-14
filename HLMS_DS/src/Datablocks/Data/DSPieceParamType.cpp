/*
 * DSPieceParamType.cpp
 *
 *  Created on: 26.03.2016
 *      Author: marvin
 */

#include "DSPieceParamType.h"

namespace Ogre {

DSPieceParamType::DSPieceParamType(String a,int shader) {
	this->paramName=a;
	this->propertyName=a;
	this->shader=shader;

}
DSPieceParamType::DSPieceParamType(String a, String b, int shader) {
	this->paramName=a;
	this->propertyName=b;
	this->shader=shader;
}
DSPieceParamType::~DSPieceParamType() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */
