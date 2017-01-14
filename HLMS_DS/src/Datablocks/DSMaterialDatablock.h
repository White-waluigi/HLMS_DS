/*
 * DSMaterialDatablock.h
 *
 *  Created on: 23.03.2016
 *      Author: marvin
 */

#ifndef SRC_DSMATERIALDATABLOCK_H_
#define SRC_DSMATERIALDATABLOCK_H_

#include "DSDatablock.h"
#include <cstddef>
#include "OgreCommon.h"
#include "OgreHlms.h"
#include "DSDatablock.h"
#include "OgreLight.h"
namespace Ogre {

class DSMaterialDatablock: public DSDatablock {
public:
	DSMaterialDatablock(IdString name, HlmsDS *creator,
	        const HlmsMacroblock *macroblock,
	        const HlmsBlendblock *blendblock,
	        const HlmsParamVec &params  );
	virtual ~DSMaterialDatablock();
   virtual void initializeParamTypes();
   void initializeProperties();
   void setProperties();

};

} /* namespace Ogre */

#endif /* SRC_DSMATERIALDATABLOCK_H_ */
