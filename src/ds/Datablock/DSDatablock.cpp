/*
 * DSDatablock.cpp
 *
 *  Created on: Jan 4, 2018
 *      Author: marvin
 */

#include "DSDatablock.h"

#include "../OgreHlmsDS.h"
namespace Ogre {

DSDatablock::DSDatablock(IdString name, HlmsDS *creator,
		const HlmsMacroblock *macroblock, const HlmsBlendblock *blendblock,
		const HlmsParamVec &params, bool derived) :
		HlmsDatablock(name, creator, macroblock, blendblock, params) {
	// TODO Auto-generated constructor stub

}

DSDatablock::~DSDatablock() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */
