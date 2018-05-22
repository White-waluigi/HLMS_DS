/*
 * PassBufferManager.cpp
 *
 *  Created on: Jan 21, 2018
 *      Author: marvin
 */

#include "../HLMSManagers/PassBufferManager.h"

#include "../Modules/ModuleBroker.h"
#include "../OgreHlmsDS.h"
#include <OgreConstBufferPacked.h>

#include "../Modules/Resources/AutoVals/PassBufferDefaultVal.h"
#include "Vao/OgreConstBufferPacked.h"
#include "Vao/OgreTexBufferPacked.h"
#include "OgreHlmsBufferManager.h"

namespace Ogre {

PassBufferManager::PassBufferManager(HlmsDS * parent) {
	assert(parent);


	//Initalize Pass Buffer Vector
	mPacked= new HlmsDS::ConstBufferPackedVec();
	//Set parent HLMS
	mParent=parent;
	initialize();
}

PassBufferManager::~PassBufferManager() {
	// TODO Auto-generated destructor stub
}
ConstBufferPacked* Ogre::PassBufferManager::get(int i) {

	//check for wrong ID
	if(i >= mPacked->size()){
		return NULL;
	}
	//return select Buffer
	return  (*mPacked)[i];
}
ConstBufferPacked* Ogre::PassBufferManager::update(int i) {

	//If PassBuffer doesn't exist yet, create it
	if (i >= mPacked->size()) {
		mPacked->push_back(
				mParent->mVaoManager->createConstBuffer(mPBSize*4,
						BT_DYNAMIC_PERSISTENT, 0, false));
	}

	//if Const Buffer d
	ConstBufferPacked *passBuffer = (*mPacked)[i];
	GPUPointer * ptr=new GPUPointer();
	ptr->start(passBuffer,mPBSize);
	for (resList::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		Ogre::DSResource*r= (*it);
		r->update();
		r->upload(ptr);
	}

	passBuffer->unmap(UO_KEEP_PERSISTENT);
}

void PassBufferManager::initialize() {


	//Reflist necessary for the creation of PassBufferDefault
	reflist list;
	list["passbuffer"]=this;
	list["hlms"]=mParent;
	list["mandatory"]=new DSResource::MandatoryInit("PassBufferDefault");


	//Load Module PassBufferDefault
	mRes.push_back(dynamic_cast<DSResource *>(mParent->getModuleBroker()->loadModule("PassBufferDefaultVal",list) ));

	//Init all Resources
	for (resList::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		(*it)->init();
	}

	//Set Size
	mPBSize=0;
	//Lock all Resources
	for (resList::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		mPBSize+= (*it)->getBufferSize();
		(*it)->lock();
	}

}


} /* namespace Ogre */
