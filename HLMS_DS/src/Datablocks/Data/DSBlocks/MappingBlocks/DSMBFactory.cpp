/*
 * DSMBFactory.cpp
 *
 *  Created on: Jan 5, 2017
 *      Author: marvin
 */

#include "DSMBFactory.h"

namespace Ogre {

DSMBFactory::DSMBFactory(DSMappingBlock * mb) {
	this->parent=mb;
}

DSMBFactory::~DSMBFactory() {
	// TODO Auto-generated destructor stub
}
DSMBModulator* Ogre::DSMBFactory::parseModulator(MT_MultiData* mt) {
}
} /* namespace Ogre */


