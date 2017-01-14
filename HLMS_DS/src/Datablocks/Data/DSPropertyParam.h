/*
 * DSPropertyParam.h
 *
 *  Created on: 24.03.2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSPROPERTYPARAM_H_
#define SRC_DATABLOCKS_DATA_DSPROPERTYPARAM_H_
#include "Ogre.h"
#include "OgreIdString.h"
#include "DSPropertyParamType.h"

namespace Ogre {

class DSPropertyParam {
	DSPropertyParamType * type;
	IdString 	key;
	int32 		value;
public:



	DSPropertyParam(Ogre::String,int32,DSPropertyParamType *);
	DSPropertyParam(IdString,int32,DSPropertyParamType *);

	DSPropertyParam(Ogre::String,int32);
	DSPropertyParam(IdString,int32);
	DSPropertyParam();

	virtual ~DSPropertyParam();
	const IdString& getKey() const;
	void setKey(const IdString& key);
	void setKey(Ogre::String key);
	int32 getValue() const;
	void setValue(int32 value);
};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSPROPERTYPARAM_H_ */
