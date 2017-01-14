/*
 * DSPieceParam.h
 *
 *  Created on: 26.03.2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSPIECEPARAM_H_
#define SRC_DATABLOCKS_DATA_DSPIECEPARAM_H_
#include "OgreString.h"
#include "OgreIdString.h"
#include "DSPieceParamType.h"

namespace Ogre {

class DSPieceParam {
	DSPieceParamType * parent;
	IdString id;
	String piece;
	int shader;

public:




	DSPieceParam(String,String,DSPieceParamType * );
	DSPieceParam(String,String,int );

	virtual ~DSPieceParam();
	 IdString& getId() ;
	void setId( IdString& id);
	 DSPieceParamType*& getParent() ;
	void setParent( DSPieceParamType*& parent);
	 String& getPiece() ;
	void setPiece( String& piece);
	int getShader() const;
	void setShader(int shader);
};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSPIECEPARAM_H_ */
