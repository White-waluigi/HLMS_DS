/*
 * DSShadowTexture.h
 *
 *  Created on: Jul 9, 2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSSHADOWTEXTURE_H_
#define SRC_DATABLOCKS_DATA_DSSHADOWTEXTURE_H_

#include "OgreString.h"

namespace Ogre {

class DSShadowTexture {
public:

	String name;

	DSShadowTexture(String name);
	virtual ~DSShadowTexture();
};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSSHADOWTEXTURE_H_ */
