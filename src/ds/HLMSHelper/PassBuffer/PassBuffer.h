/*
 * PassBuffer.h
 *
 *  Created on: Apr 16, 2017
 *      Author: marvin
 */

#ifndef SRC_HLMSHELPER_PASSBUFFER_H_
#define SRC_HLMSHELPER_PASSBUFFER_H_
#include <Ogre.h>

#include "HLMSHelper/PassBuffer/PassBufferSlot.h"


namespace Ogre {
class HlmsDS;

class PassBuffer {
std::vector<PassBufferSlot *> slots;
std::vector<float> * curFloat=NULL;



public:
	std::map<Ogre::String,int> defmap;
	HlmsDS * parent=NULL;
	Ogre::Camera *curcam=NULL;
	const Ogre::CompositorShadowNode *shadowNode=NULL;
	bool casterPass;
	bool dualParaboloid;

	std::vector<float> * getPassBufferArray(Ogre::Camera *curcam,const Ogre::CompositorShadowNode *shadowNode,bool casterPass, bool dualParaboloid);

	void addData(float f);
	PassBuffer(HlmsDS * parent);
	virtual ~PassBuffer();
};

} /* namespace Ogre */

#endif /* SRC_HLMSHELPER_PASSBUFFER_H_ */
