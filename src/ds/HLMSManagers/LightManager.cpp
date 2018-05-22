/*
 * LightManager.cpp
 *
 *  Created on: Feb 15, 2018
 *      Author: marvin
 */

#include "LightManager.h"
#include "../OgreHlmsDS.h"
#include "Light/DSLight.h"
#include "OgreItem.h"


namespace Ogre {

LightManager::LightManager(HlmsDS * parent) {
	assert(parent);

	mParent=parent;
	this->ambientLightID = Ogre::Id::generateNewId<Ogre::Light>();
	mParent->addListener(this);

}

LightManager::~LightManager() {
	// TODO Auto-generated destructor stub
}

void LightManager::preparePassHash(const CompositorShadowNode* shadowNode,
		bool casterPass, bool dualParaboloid, SceneManager* sceneManager,
		Hlms* hlms) {


	const LightListInfo &globalLightList = sceneManager->getGlobalLightList();
	if (this->mLights.find(ambientLightID) == this->mLights.end()) {

		DSLight* dlight = createNewDSLight(NULL,sceneManager);

		this->mLights[ambientLightID]=(dlight);

	}

	for(uint i=0;i<globalLightList.lights.size();i++){
		if(mLights.find(globalLightList.lights[i]->getId())==mLights.end()){
			this->mLights[globalLightList.lights[i]->getId()]=createNewDSLight(globalLightList.lights[i],sceneManager);
		}
	}

	DSLightMap::iterator it;

//	for (it = mLights.begin(); it != mLights.end(); it++) {
//		if (it->second->isAttached()) {
//			delete it->second;
//			mLights.erase(it->first);
//		}
//
//	}

}

DSLight* LightManager::createNewDSLight(const Ogre::Light* light, SceneManager* sceneManager) {
	String meshName;
//	int type = 0;
//	if (light == NULL) {
//		type = 4;
//	} else if (light->getType() == Ogre::Light::LT_POINT) {
//		type = 1;
//	} else if (light->getType() == Ogre::Light::LT_DIRECTIONAL) {
//		type = 0;
//	} else if (light->getType() == Ogre::Light::LT_SPOTLIGHT) {
//		type = 2;
//
//	}
//
	DSLight * dslight = new Ogre::DSLight(light,mParent,
			Ogre::Id::generateNewId<Ogre::MovableObject>(),
			&sceneManager->_getEntityMemoryManager(Ogre::SCENE_DYNAMIC),
			sceneManager, 0);


//						Ogre::Item *item =sceneManager->createItem("Label.mesh",
//															Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
//															Ogre::SCENE_DYNAMIC);

	dslight->setVisibilityFlags(0x00000000);



	if(light!=NULL)
	{
		dslight->parentSN =
				light->getParentSceneNode();
//
//		dslight->parentSN->setPosition(light->getParentNode()->_getDerivedPositionUpdated());
				dslight->parentSN->_getFullTransformUpdated();


	}else
	{
		dslight->parentSN =
				sceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->createChildSceneNode(
						Ogre::SCENE_DYNAMIC);
	}
	dslight->parentSN->attachObject(dslight);
	return NULL;
}
DSLight* LightManager::DeleteDSLight(const Ogre::Light* light,
		SceneManager* sceneManager) {
}
} /* namespace Ogre */


