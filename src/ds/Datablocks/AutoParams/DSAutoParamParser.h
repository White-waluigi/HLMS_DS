/*
 * DSAutoParamParser.h
 *
 *  Created on: 01.04.2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_AUTOPARAMS_DSAUTOPARAMPARSER_H_
#define SRC_DATABLOCKS_AUTOPARAMS_DSAUTOPARAMPARSER_H_
#include <OgreException.h>
#include <OgreArchive.h>
#include <stddef.h>
#include <OgreHlmsDatablock.h>
#include <OgreHlmsTextureManager.h>
#include <OgreConstBufferPool.h>
#include <OgreHeaderPrefix.h>

#include "Datablocks/Data/DSAutoParam.h"
#include "DBParser/Template/MTMultiData.h"
#include "OgreHlmsDSPrerequisites.h"

namespace Ogre {
class DSMaterialParam;
class DSAutoParamParser {
public:
	DSAutoParamParser();
	virtual ~DSAutoParamParser();

	DSMaterialParam*  getAutoParam(Ogre::String string);
	DSMaterialParam*  getAutoParam(MT_MultiData * md);

};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_AUTOPARAMS_DSAUTOPARAMPARSER_H_ */
