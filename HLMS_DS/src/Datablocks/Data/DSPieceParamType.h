/*
 * DSPieceParamType.h
 *
 *  Created on: 26.03.2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSPIECEPARAMTYPE_H_
#define SRC_DATABLOCKS_DATA_DSPIECEPARAMTYPE_H_
#include "OgreString.h"
namespace Ogre {

class DSPieceParamType {
public:

	String paramName;
	String propertyName;
	int shader;

	DSPieceParamType(String, String,int shader);
	DSPieceParamType(String,int shader);
	virtual ~DSPieceParamType();
};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSPIECEPARAMTYPE_H_ */
