/*
 * Piece.h
 *
 *  Created on: Jan 24, 2018
 *      Author: marvin
 *
 * Is used to manage Shader Pieces for Procedural Shader Code,
 * Addressed by keyword
 *
 * !!!!!!!!!! NOTE !!!!!!!!!!!!!!!!
 * Shader Piece Source Code cannot include
 * further Shader Code using @piece
 * please add Code to the main Shader Codebase
 * to do that
 *
 *
 */

#ifndef SRC_DS_MODULES_RESOURCES_PIECE_H_
#define SRC_DS_MODULES_RESOURCES_PIECE_H_

#include "DSResource.h"

namespace Ogre {

class Piece: public DSResource {
	//Shader Piece to be Managed
	DSResource::ShaderPiece* mCode=NULL;
public:
	//No Construction necessary, Shader Piece is added afterwards
	Piece(reflist l);

	
	//Simply return mCode
	virtual DSResource::ShaderPiece::shaderList genShaderPieces();
	//Do Nothing
	virtual DSResource::PropertyList genProperties();

	//No data uploaded
	virtual int getBufferSize();

	//Check if Shader Code was uploaded
	virtual void init();
	//No upload of data
	virtual void upload(GPUPointer * ptr);
	//Get the code
	DSResource::ShaderPiece* getCode();
	//Set the code
	void setCode( DSResource::ShaderPiece* code);

	//Clean up, delete Shader Piece
	virtual ~Piece();
};

} /* namespace Ogre */

#endif /* SRC_DS_MODULES_RESOURCES_PIECE_H_ */
