/*
 * DSPieceParam.cpp
 *
 *  Created on: 26.03.2016
 *      Author: marvin
 */


#include "OgreString.h"


#include "Datablocks/Data/DSPieceParam.h"
#include "Datablocks/Data/DSPieceParamType.h"

namespace Ogre {


DSPieceParam::DSPieceParam(String a, String b,DSPieceParamType * type) {
	this->id=IdString(a);
	this->piece=b;
	this->parent=type;
	this->shader=type->shader;
}
DSPieceParam::DSPieceParam(String id, String piece,int type) {
	this->id=IdString(id);
	this->piece=piece;
	this->shader=type;

	this->parent=NULL;
}
DSPieceParam::~DSPieceParam() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */

 Ogre::IdString& Ogre::DSPieceParam::getId()  {
	return id;
}

void Ogre::DSPieceParam::setId( IdString& id) {
	this->id = id;
}

 Ogre::DSPieceParamType*& Ogre::DSPieceParam::getParent()  {
	return parent;
}

void Ogre::DSPieceParam::setParent( DSPieceParamType*& parent) {
	this->parent = parent;
}

 Ogre::String& Ogre::DSPieceParam::getPiece()  {
	return piece;
}

void Ogre::DSPieceParam::setPiece( String& piece) {
	this->piece = piece;
}

int Ogre::DSPieceParam::getShader() const {
	return shader;
}

void Ogre::DSPieceParam::setShader(int shader) {
	this->shader = shader;
}
