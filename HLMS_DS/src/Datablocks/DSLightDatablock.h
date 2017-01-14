/*
 * DSLightDatablock.h
 *
 *  Created on: 20.03.2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DSLIGHTDATABLOCK_H_
#define SRC_DATABLOCKS_DSLIGHTDATABLOCK_H_
#include <cstddef>
#include "OgreCommon.h"
#include "OgreHlms.h"
#include "DSDatablock.h"
#include "OgreLight.h"
namespace Ogre {


class DSLightDatablock: public DSDatablock {

	bool updateable;

public:
	const HlmsSamplerblock * GBufferSamplerblock;

	DSLightDatablock(IdString name, HlmsDS *creator,
	        const HlmsMacroblock *macroblock,
	        const HlmsBlendblock *blendblock,
	        const HlmsParamVec &params  );
	virtual ~DSLightDatablock();

   virtual void initializeParamTypes();
   void initializeProperties();
   void setProperties();
   Ogre::Light::LightTypes getType();
};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DSLIGHTDATABLOCK_H_ */
