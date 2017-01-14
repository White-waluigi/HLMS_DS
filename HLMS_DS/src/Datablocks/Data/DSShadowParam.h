/*
 * DSShadowParam.h
 *
 *  Created on: Jul 9, 2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSSHADOWPARAM_H_
#define SRC_DATABLOCKS_DATA_DSSHADOWPARAM_H_

#include "OgreString.h"

namespace Ogre {

class DSShadowParam {
public:
	String name;

	DSShadowParam(String name);
	virtual ~DSShadowParam();
};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSSHADOWPARAM_H_ */
