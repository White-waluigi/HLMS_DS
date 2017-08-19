/*
 * PassBuffer.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: marvin
 */

#include "HLMSHelper/PassBuffer/PassBuffer.h"
#include "OgreHlmsDS.h"

namespace Ogre {

PassBuffer::PassBuffer(HlmsDS * parent) {
	this->parent=parent;
	int sizes[10]=			 {		4,		16,			16,		 16,           16,		      4,		8,		   4,			 4,		  4};
	Ogre::String sizename[10]={	"debug",    "proj",     "view",  "viewproj",  "invView",    "timer", "clipping","textureflip","camera","test"};

	for(int i=0;i<10;i++){
		defmap[sizename[i]]=sizes[i];
	}
	for(int i=0;i<10;i++){
		slots.push_back(new PassBufferSlot(sizename[i],this));
	}
	dualParaboloid=false;
	casterPass=false;


}

std::vector<float>* PassBuffer::getPassBufferArray(Ogre::Camera *curcam,const Ogre::CompositorShadowNode *shadowNode,bool casterPass, bool dualParaboloid) {


	this->curcam=curcam;
	this->shadowNode=shadowNode;
	this->casterPass=casterPass;
	this->dualParaboloid=dualParaboloid;

	curFloat=new std::vector<float>();

	for(uint i=0;i<this->slots.size();i++){
		this->slots[i]->bindData();
	}

	return curFloat;
}

void PassBuffer::addData(float f) {
	curFloat->push_back(f);
}


PassBuffer::~PassBuffer() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */
