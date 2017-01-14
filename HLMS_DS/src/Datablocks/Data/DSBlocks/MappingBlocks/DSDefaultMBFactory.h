/*
 * DSDefaultMBFactory.h
 *
 *  Created on: Dec 25, 2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_DSDEFAULTMBFACTORY_H_
#define SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_DSDEFAULTMBFACTORY_H_

#include "Ogre.h"
#include "DSMBFactory.h"

namespace Ogre{

class DSDefaultMBFactory : public  DSMBFactory{

public:

	virtual DSMBModulator* parseModulator(MT_MultiData * mt);


	DSDefaultMBFactory(DSMappingBlock * mb);
	virtual ~DSDefaultMBFactory();
};

}
#endif /* SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_DSDEFAULTMBFACTORY_H_ */
