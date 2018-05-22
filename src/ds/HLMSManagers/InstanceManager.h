/*
 * InstanceManager.h
 *
 *  Created on: Feb 15, 2018
 *      Author: marvin
 *
 *  The Instance Manager allows creates the Instance Buffer, which is the Indvidual Data for every renderable.
 *  It is needed so that two objects may still have the same Material, but be rendered at different positions for example.
 *
 *  It can also be extended by the User and doesn't have fixed size. It is extended by adding further InstanceRes Modules.
 *
 *	Instance Manager differentiates between Data and Index.
 *
 *	Data is a big 1d texture filled with 4 byte values (floats or ints) that holds the data for every renderable. (Unless it becomes to big, then a new one is created)
 *
 *  Index is a simple GPU Buffer that holds the starting positon for every renderables data, sort of like an old Magnet band.
 *
 *  The reason for this is that there may potentially be a ton of renderables on screen and GPU Buffers are size limited.
 *
 */

#ifndef SRC_DS_HLMSMANAGERS_INSTANCEMANAGER_H_
#define SRC_DS_HLMSMANAGERS_INSTANCEMANAGER_H_
#include "Ogre.h"
#include "../Modules/Resources/DSResource.h"

#include <OgreHlmsListener.h>

namespace Ogre {
class HlmsDS;
class InstanceRes;
class InstanceManager: public ModuleReference,public HlmsListener {
	//Parent HLMS
	HlmsDS * mParent=NULL;


	//All lists are typedefed for readability
	typedef std::list<InstanceRes *> IRResList;

	//List of Instance Resources
	IRResList mRes;



public:


	//Size of Data Texture
	int getTotalSizeData();
	//Size of Index Buffer
	int getTotalSizeIndex();

	//Upload and update both index and data
	void upload(const QueuedRenderable& queuedRenderable,GPUPointer* index,GPUPointer *data);

	//Sets parent Hlms
	//Creates the default Module WorldMatIR.cpp
	InstanceManager(HlmsDS * parent );

	//returns parent Hlms
	const HlmsDS* getParent();

	//Clean up, delete all Instance Resources
	virtual ~InstanceManager();
};

} /* namespace Ogre */

#endif /* SRC_DS_HLMSMANAGERS_INSTANCEMANAGER_H_ */
