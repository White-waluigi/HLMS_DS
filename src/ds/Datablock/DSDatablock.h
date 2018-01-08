/*
 * DSDatablock.h
 *
 *  Created on: Jan 4, 2018
 *      Author: marvin
 */

#ifndef SRC_DS_DATABLOCK_DSDATABLOCK_H_
#define SRC_DS_DATABLOCK_DSDATABLOCK_H_
#include "OgreHlmsDatablock.h"
#include <OgreHlmsTextureManager.h>
#include <OgreConstBufferPool.h>
#include <OgreHeaderPrefix.h>

namespace Ogre {
class HlmsDS;

class DSDatablock: public Ogre::HlmsDatablock{
public:
	DSDatablock(IdString name, HlmsDS *creator,
			const HlmsMacroblock *macroblock, const HlmsBlendblock *blendblock,
			const HlmsParamVec &params, bool derived = false);
	virtual ~DSDatablock();
};

} /* namespace Ogre */

#endif /* SRC_DS_DATABLOCK_DSDATABLOCK_H_ */
