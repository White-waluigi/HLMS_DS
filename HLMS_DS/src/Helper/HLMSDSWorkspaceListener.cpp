/*
 * HLMSDSWorkspaceListener.cpp
 *
 *  Created on: Jul 7, 2016
 *      Author: marvin
 */

#include "../Helper/HLMSDSWorkspaceListener.h"
#include "Compositor/Pass/OgreCompositorPass.h"
#include "Compositor/Pass/OgreCompositorPassDef.h"

#include "iostream"
namespace Ogre {

HLMSDSWorkspaceListener::HLMSDSWorkspaceListener() {
	// TODO Auto-generated constructor stub


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
