/*
 * DSMBFrameAnimation.h
 *
 *  Created on: Jan 4, 2017
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_MODULATORS_DSMBFRAMEANIMATION_H_
#define SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_MODULATORS_DSMBFRAMEANIMATION_H_
#include "../DSMBModulator.h"
namespace Ogre {

class DSMBFrameAnimation: public DSMBModulator {

	int numUV;
	float fps;


public:
	DSMBFrameAnimation(DSMappingBlock * parent,MT_MultiData* params);
	virtual ~DSMBFrameAnimation();

	virtual void modulate();

};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_MODULATORS_DSMBFRAMEANIMATION_H_ */
