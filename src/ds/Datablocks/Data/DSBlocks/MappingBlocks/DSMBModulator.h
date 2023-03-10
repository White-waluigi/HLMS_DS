/*
 * DSMBModulator.h
 *
 *  Created on: Jan 4, 2017
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_DSMBMODULATOR_H_
#define SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_DSMBMODULATOR_H_

#include "Datablocks/Data/DSBlocks/MappingBlocks/DSMappingBlock.h"

namespace Ogre {

class DSMBModulator {


public:

	float timeScale;

	DSMappingBlock * parent;
	Ogre::String name;

	std::vector<float> elapsed;
	std::vector<float> timeFrame;

	bool pulse=false;

	DSMBModulator(DSMappingBlock * parent,MT_MultiData * params);
	virtual void modulate();
	virtual ~DSMBModulator();

};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_DSMBMODULATOR_H_ */
