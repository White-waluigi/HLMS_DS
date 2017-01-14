/*
 * DSAutoParamTime.h
 *
 *  Created on: Aug 21, 2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_AUTOPARAMS_DSAUTOPARAMTIME_H_
#define SRC_DATABLOCKS_AUTOPARAMS_DSAUTOPARAMTIME_H_

#include "../../Datablocks/Data/DSMaterialParam.h"
#include "../Data/DSAutoParam.h"
#include "../../OgreHlmsDSPrerequisites.h"
#include "OgreHlmsDatablock.h"
#include "OgreHlmsTextureManager.h"
#include "OgreConstBufferPool.h"
#include "OgreHeaderPrefix.h"
namespace Ogre {

class DSAutoParamTime: public DSMaterialParam {
public:
	Ogre::Timer* timer;


	int timeLenght=1000;
	bool pulse=false;

	virtual void initialize(const HlmsParamVec &params);

	DSAutoParamTime();
	virtual ~DSAutoParamTime();
	virtual void update();
	static Ogre::String getParamName(){return "time";}

};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_AUTOPARAMS_DSAUTOPARAMTIME_H_ */
