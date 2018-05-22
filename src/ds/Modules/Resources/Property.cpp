/*
 * Property.cpp
 *
 *  Created on: Jan 24, 2018
 *      Author: marvin
 */

#include "Property.h"

namespace Ogre {

Property::Property(reflist l):DSResource(l) {
	//nothing necessary
}

int32 Property::getValue() const {
	return mValue;
}

DSResource::ShaderPiece::shaderList Property::genShaderPieces() {
	//No shader necessary
	return DSResource::ShaderPiece::shaderList();
}

DSResource::PropertyList Property::genProperties() {
	PropertyList l;
	l[this->getName()]=this->mValue;
	return l;
}



int Property::getBufferSize() {
	//No data => adds nothing to the Buffer
	return 0;
}







void Property::init() {
	//Nothing really
}

void Property::upload(GPUPointer* ptr) {
	//Nothing necessary
}

void Property::setValue(int32 value) {
	//Can only be set before lock, since the shader has already been compiled then
	checkLock();
	mValue = value;
}

Property::~Property() {
	// TODO Cleanup
}

} /* namespace Ogre */
