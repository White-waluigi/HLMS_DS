/*
 * DSMBFactory.h
 *
 *  Created on: Jan 5, 2017
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_DSMBFACTORY_H_
#define SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_DSMBFACTORY_H_
#include "Datablocks/Data/DSBlocks/MappingBlocks/DSMappingBlock.h"
namespace Ogre {
class MT_MultiData;
class DSMBModulator;
class DSMBModulator;
class DSMBFactory {


public:
	DSMappingBlock * parent;

	DSMBFactory(DSMappingBlock * mb);
	virtual DSMBModulator* parseModulator(MT_MultiData * mt);
	virtual ~DSMBFactory();
};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_DSMBFACTORY_H_ */
