/*
 * Piece.cpp
 *
 *  Created on: Jan 24, 2018
 *      Author: marvin
 */

#include "Piece.h"

namespace Ogre {

Piece::Piece(reflist l):DSResource(l) {
	//Nothing necessary
}

DSResource::ShaderPiece* Piece::getCode() {
	return mCode;
}

DSResource::ShaderPiece::shaderList Piece::genShaderPieces() {
	//return all our ShaderPieces
	DSResource::ShaderPiece::shaderList l;
	l.push_back(mCode);
	return l;
}

DSResource::PropertyList Piece::genProperties() {
	return PropertyList();
}

void Piece::setCode(DSResource::ShaderPiece* code) {
	//Can only be edited before locked
	checkLock();


	this->mCode = code;
	//Resource identcal to Keyword
	mCode->mShaderKeyWord=getName();
}

Piece::~Piece() {
	// TODO cleanup
}

int Piece::getBufferSize() {
	return 0;
}



void Piece::init() {
	//Needs Code to work
	if(this->mCode==NULL){
		OGRE_EXCEPT(Exception::ERR_INVALID_CALL,"No Shadercode for Shader Resource defined!","void Piece::init() {");
	}
}

void Piece::upload(GPUPointer* ptr) {
}



} /* namespace Ogre */
