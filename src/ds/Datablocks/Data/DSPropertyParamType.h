/*
 * DSPropertyParamType.h
 *
 *  Created on: 24.03.2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSPROPERTYPARAMTYPE_H_
#define SRC_DATABLOCKS_DATA_DSPROPERTYPARAMTYPE_H_
#include "OgreString.h"
namespace Ogre {

class DSPropertyParamType {


public:

	String paramName;
	String propertyName;
	DSPropertyParamType(String);
	DSPropertyParamType(String,String);
	virtual ~DSPropertyParamType();
};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSPROPERTYPARAMTYPE_H_ */
