/*
 * DSAutoTime.h
 *
 *  Created on: 01.04.2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_AUTOPARAMS_DSAUTOTIME_H_
#define SRC_DATABLOCKS_AUTOPARAMS_DSAUTOTIME_H_
#include "../Data/DSAutoParam.h"
#include "../../OgreHlmsDSPrerequisites.h"
#include "OgreHlmsDatablock.h"
#include "OgreHlmsTextureManager.h"
#include "OgreConstBufferPool.h"
#include "OgreHeaderPrefix.h"
namespace Ogre {

class DSAutoTime: public DSAutoParam {
public:
	DSAutoTime();
	virtual ~DSAutoTime();

	Ogre::Timer* timer;


	int timeLenght=1000;
	bool pulse=false;

	virtual void initialize(const HlmsParamVec &params);
	virtual void update();
	static Ogre::String getParamName(){return "time";}

};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_AUTOPARAMS_DSAUTOTIME_H_ */
