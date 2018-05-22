/*
 * InstanceRes.h
 *
 *  Created on: Mar 22, 2018
 *      Author: marvin
 *
 *  Used by InstanceManager to create custom behaviour/data for the InstanceBuffer
 *  which is used to differentiate data for Objects who share a datablock,
 *  since the are probably going to have different positions, orientations, skeletal animation,
 *  etc.
 *  A custom Instance Buffer could for example be used for grass that the player steps on,
 *  or other physical interaction
 *
 *  It works similar to Datablock and Passbuffer Resources, but it only uploads data to
 *  a GPU Buffer
 */

#ifndef SRC_DS_HLMSMANAGERS_INSTANCERES_INSTANCERES_H_
#define SRC_DS_HLMSMANAGERS_INSTANCERES_INSTANCERES_H_


#include "../InstanceManager.h"

#include "../../Modules/DSModule.h"
namespace Ogre{
class InstanceRes: public DSModule {
public:

	//requires a parent HLMS and a Parent Instance Manager
	static refreq * getRequiredRefs();

	//needs a parent InstanceManager
	InstanceManager * mParentIMgr=NULL;

	//set parent Instance Manger
	InstanceRes(reflist r);

	//Size added to Instance Buffer
	virtual int getSize()=0;

	//upload to Instance Buffer
	virtual void upload(const QueuedRenderable& queuedRenderable,GPUPointer* data)=0;


};


}


#endif /* SRC_DS_HLMSMANAGERS_INSTANCERES_INSTANCERES_H_ */
