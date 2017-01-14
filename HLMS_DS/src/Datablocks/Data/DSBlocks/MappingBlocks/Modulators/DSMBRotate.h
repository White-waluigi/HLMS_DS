/*
 * DSMBRotate.h
 *
 *  Created on: Jan 4, 2017
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_MODULATORS_DSMBROTATE_H_
#define SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_MODULATORS_DSMBROTATE_H_
#include "../DSMBModulator.h"
namespace Ogre {

class DSMBRotate: public DSMBModulator {
	Ogre::Vector2 center=Vector2::ZERO;
	float speed=1.0;

public:
	DSMBRotate(DSMappingBlock * parent,MT_MultiData* params);
	virtual ~DSMBRotate();

	virtual  void modulate();

};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_MODULATORS_DSMBROTATE_H_ */
