/*
 * DSPropertyParam.cpp
 *
 *  Created on: 24.03.2016
 *      Author: marvin
 */

#include "DSPropertyParam.h"

namespace Ogre {

DSPropertyParam::DSPropertyParam(String key, int32 value ,DSPropertyParamType * type) {
	this->key=IdString( key);
	this->value=value;
	this->type=type;

}
DSPropertyParam::DSPropertyParam(IdString key, int32 value ,DSPropertyParamType * type) {
	this->key= key;
	this->value=value;
	this->type=type;

}
DSPropertyParam::~DSPropertyParam() {
	// TODO Auto-generated destructor stub
}
const IdString& Ogre::DSPropertyParam::getKey() const {
	return key;
}
void Ogre::DSPropertyParam::setKey(Ogre::String key) {
	this->key = IdString(key);
}
void Ogre::DSPropertyParam::setKey(const IdString& key) {
	this->key = key;
}

int32 Ogre::DSPropertyParam::getValue() const {
	return value;
}

void Ogre::DSPropertyParam::setValue(int32 value) {
	this->value = value;
}
Ogre::DSPropertyParam::DSPropertyParam(String key, int32 value) {
	this->key=IdString( key);
	this->value=value;
	this->type=NULL;
}

Ogre::DSPropertyParam::DSPropertyParam(IdString key, int32 value) {
	this->key= key;
	this->value=value;
	this->type=NULL;

}
Ogre::DSPropertyParam::DSPropertyParam() {
}
} /* namespace Ogre */


