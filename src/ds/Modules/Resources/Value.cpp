/*
 * Value.cpp
 *
 *  Created on: Jan 21, 2018
 *      Author: marvin
 */

#include "Value.h"
#include "../../Util/ShaderGenerator.h"
#include "../../Util/ShaderKeyWordBank.h"
namespace Ogre {

Value::Value(reflist l):DSResource(l) {
	//Reserve space for value data
	initData();
}

Value::~Value() {
	// TODO Cleanup
}

void Value::set(float* data) {

	//Set the data to be uploaded to GPU
	int size=getBufferSize();

	//Check size, then copy to to mValue
	for(int i=0;i<size;i++){
		if((data)==NULL){
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,"cannot update value, float array not big enough","void Value::update(float* data) {");
		}
		this->mValue[i]=data[i];


	}
	//Data has changed, mark as unsyncd
	soil();
}



int Value::getCustomSize() {
	OGRE_EXCEPT(Exception::ERR_INVALID_CALL,"Can't use custom type using baseclass Value","int Value::getCustomSize() {");
}



int Value::getArraysize() const {
	return mArraysize;
}

void Value::setArraysize(int arraysize) {

	//Can only be changed before the Datablocks ConstBuffer has been created to keep its size static
	checkLock();
	initData();
	mArraysize = arraysize;
}

Value::dataType Value::getType() const {
	return mType;
}

void Value::initData() {
	//
	this->mValue=new float[getBufferSize()];
}

void Value::setType(dataType type) {
	//Can only be changed before the Datablocks ConstBuffer has been created to keep its size static
	checkLock();
	initData();
	mType = type;
}

void Value::upload(GPUPointer* ptr) {
	ptr->map(mValue, getBufferSize());
	clean();
}
int Ogre::Value::getBufferSize() {

	//Get Buffersize depending on datatype

	size_t size;
	if (this->mType == SINGLEFLOAT) {
		//Can't be smaller than 4
		size = 4;
	} else {
	}
	switch (this->mType) {
	case SINGLEFLOAT:
		size = 4;
		break;
	case UINT:
		size = 4;
		break;
	case VEC4:
		size = 4;
		break;
	case UVEC4:
		size = 4;
		break;

	case MAT4:
		size = 16;
		break;

	case CUSTOM:
		//In case of custom implemenation of Value, get custom made size
		//todo doesn't work, just calls baseclass
		size = getCustomSize();
		break;

	}
	return size * mArraysize;
}
DSResource::ShaderPiece::shaderList Value::genShaderPieces() {
	ShaderPiece::shaderList l;

	l.push_back(ShaderGenerator::getDataTypeShader(this->getType(),this->getName(),"val_","MaterialDataParams",this->mArraysize));
	return l;
}

Value::PropertyList Value::genProperties() {
	PropertyList l;

	return l;
}

} /* namespace Ogre */


