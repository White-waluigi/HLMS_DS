/*
 * DSDefaultMBFactory.cpp
 *
 *  Created on: Dec 25, 2016
 *      Author: marvin
 */

#include "Datablocks/Data/DSBlocks/MappingBlocks/DSDefaultMBFactory.h"
#include "Datablocks/Data/DSBlocks/MappingBlocks/DSMBModulator.h"
#include "DBParser/Template/MTMultiData.h"
#include "Datablocks/Data/DSBlocks/MappingBlocks/Modulators/DSMBFrameAnimation.h"
#include "Datablocks/Data/DSBlocks/MappingBlocks/Modulators/DSMBScroll.h"
#include "Datablocks/Data/DSBlocks/MappingBlocks/Modulators/DSMBRotate.h"

namespace Ogre{

DSDefaultMBFactory::DSDefaultMBFactory(DSMappingBlock * mb) :DSMBFactory(mb){
	// TODO Auto-generated constructor stub

}
DSMBModulator *DSDefaultMBFactory::parseModulator(MT_MultiData* mt) {
	Ogre::String t=mt->getData("type").s;

if(t.compare("frame")==0)
		return new DSMBFrameAnimation(parent,mt);
if(t.compare("rotate")==0)
		return new DSMBRotate(parent,mt);
if(t.compare("scroll")==0)
		return new DSMBScroll(parent,mt);

	return NULL;
}


DSDefaultMBFactory::~DSDefaultMBFactory() {
	// TODO Auto-generated destructor stub
}

}
