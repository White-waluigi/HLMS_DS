/*
 * DSMappingBlock.h
 *
 *  Created on: Jan 4, 2017
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_DSMAPPINGBLOCK_H_
#define SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_DSMAPPINGBLOCK_H_
#include <Ogre.h>

#include "DBParser/Template/MTMultiData.h"


namespace Ogre {
class DSMBModulator;
class DSTextureParam;
class DSMBFactory;
class DSMappingBlock {
public:
	Ogre::Timer timer;

	Ogre::String name;

	FastArray<DSMBModulator *> modulators;

	//Quick way to create custom Texture Mapping animations
	std::vector<DSMBFactory* > modulatorFactorys;




	bool animated=false;
	Ogre::Matrix4 matOffset;
	Ogre::Matrix4 BasematOffset;
	int UVindexOffset;

	DSMappingBlock(MT_MultiData * mt);
	void initialize(MT_MultiData * mt);
	void modulate(DSTextureParam * parent);
	virtual ~DSMappingBlock();
	 Ogre::Timer getTimer() ;
	void setTimer( Ogre::Timer timer);
};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSBLOCKS_MAPPINGBLOCKS_DSMAPPINGBLOCK_H_ */
