/*
 * HLMSDSWorkspaceListener.cpp
 *
 *  Created on: Jul 7, 2016
 *      Author: marvin
 */

#include <Helper/HLMSDSWorkspaceListener.h>
#include <Compositor/Pass/OgreCompositorPass.h>
#include <Compositor/Pass/OgreCompositorPassDef.h>

#include "iostream"
namespace Ogre {

HLMSDSWorkspaceListener::HLMSDSWorkspaceListener() {
	// TODO Auto-generated constructor stub
	this->passType=PT_GBUFFER;
	this->passID=0;

}

void HLMSDSWorkspaceListener::passPreExecute(CompositorPass* pass) {
	int ids[6]={-1,800,801,802,803,804};


	for(int i=0;i<6;i++){
		if(ids[i]==pass->getDefinition()->mIdentifier){
			this->passType=(PassType)i;
		}
	}
	passID=pass->getDefinition()->mIdentifier;
}

HLMSDSWorkspaceListener::~HLMSDSWorkspaceListener() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */

Ogre::String Ogre::HLMSDSWorkspaceListener::i2n(int i) {


	if(i==800){
		return "Shadow";
	}
	if(i==801){
		return "Gbuffer";
	}
	if(i==802){
		return "Light";
	}
	if(i==803){
		return "Forward";
	}
	if(i==804){
		return "Post";
	}

	return "Unknown";

}

Ogre::String Ogre::HLMSDSWorkspaceListener::getName() {
	return i2n(this->passID);
}
