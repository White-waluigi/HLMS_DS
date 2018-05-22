/*
 * InstanceManager.cpp
 *
 *  Created on: Feb 15, 2018
 *      Author: marvin
 */

#include "InstanceManager.h"
#include "../Modules/ModuleBroker.h"
#include "../OgreHlmsDS.h"
#include "InstanceRes/InstanceRes.h"
#include "InstanceRes/WorldMatIR.h"

namespace Ogre {

InstanceManager::InstanceManager(HlmsDS * p) {
	assert(p);

	//Set parent hlms
	mParent = p;

	//create reflist with values necessary to create WorldMapIR
	reflist r;
	r["hlms"]=p;
	r["instanceMgr"]=this;

	//Create default WorldMapIR to store Object position
	this->mRes.push_back(  dynamic_cast<InstanceRes*>( p->getModuleBroker()->loadModule("WorldMapIR",r) )  );



}

void InstanceManager::upload(const QueuedRenderable& queuedRenderable,GPUPointer *index,GPUPointer* data) {

	//Current Index offset is 0
	int curIndex=0;


	//todo change mapping Index per Resource to index per Renderable
	for (IRResList::iterator it= this->mRes.begin(); it!=mRes.end();it++){
		//Upload data for each Renderable
		(*it)->upload(queuedRenderable,data);

		index->map(curIndex);
		//Padding
		index->map(0,3);


		//Increase Index Offset by size of Resource
		curIndex+=(*it)->getSize();
	}
}

int InstanceManager::getTotalSizeData() {

	//Simply add each Resources Size do size to determine overall Buffer Size necessary
	int size=0;

	for (IRResList::iterator it= this->mRes.begin(); it!=mRes.end();it++){
		size+=(*it)->getSize();
	}

	return size;
}

int InstanceManager::getTotalSizeIndex() {
	//todo wrong, is always 4
	return mRes.size()*4;
}

const HlmsDS* InstanceManager::getParent() {
	return mParent;
}



InstanceManager::~InstanceManager() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */
