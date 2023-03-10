/*
 * DBParser.cpp
 *
 *  Created on: Nov 17, 2016
 *      Author: marvin
 */
#if !OGRE_NO_JSON

#include "DBParser/DBParser.h"

#include <OgreHlmsManager.h>
#include <OgreHlmsTextureManager.h>
#include <OgreTexture.h>
#include <OgreLogManager.h>
#include <Vao/OgreVaoManager.h>
#include <Vao/OgreConstBufferPacked.h>
#include <Vao/OgreTexBufferPacked.h>

#include "Datablocks/DSDatablock.h"
#include "OgreHlmsDS.h"
#include "Datablocks/Data/DSTextureParam.h"
#include "Datablocks/Data/DSTextureParamType.h"


#include "Datablocks/Data/DSMaterialParam.h"

#include "Light/StringValueUtils.h"

#include "Datablocks/Data/DSMaterialParam.h"
#include "Datablocks/Data/DSMaterialParamType.h"
namespace Ogre {

void DBParser::parse(HlmsManager *hlmsManager) {
	assert(false);
}

DBParser::DBParser() {
	hdb=NULL;
}

DBParser::~DBParser() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */



#endif
