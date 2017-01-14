/*
 * DSMBScroll.h
 *
 *  Created on: Jan 4, 2017
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_MODULATORS_DSMBSCROLL_H_
#define SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_MODULATORS_DSMBSCROLL_H_
#include "../DSMBModulator.h"
namespace Ogre {

class DSMBScroll: public DSMBModulator {
public:
	Vector2 speed=Vector2::UNIT_SCALE;

	DSMBScroll(DSMappingBlock * parent,MT_MultiData* params);
	virtual ~DSMBScroll();

	virtual void modulate();

};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_MODULATORS_DSMBSCROLL_H_ */
