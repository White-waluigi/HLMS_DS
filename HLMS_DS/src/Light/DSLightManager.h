/*
 * DSLightManager.h
 *
 *  Created on: 24.11.2015
 *      Author: marvin
 */

#ifndef LIGHT_DSLIGHTMANAGER_H_
#define LIGHT_DSLIGHTMANAGER_H_
#include "../OgreHlmsDS.h"
#include "DSLight.h"
namespace Ogre {

class DSLightManager {
public:
	Ogre::HlmsDS * hlmsman;
	IdType ambientLightID;
	DSLightManager(HlmsDS * parent);
	virtual ~DSLightManager();

	const CompositorShadowNode * shadowNode;
	int numPssm=4;

	std::map<IdType,DSLight *>	lightList;

	bool CheckForNewLights( SceneManager * sceneManager,const CompositorShadowNode * shadownode,int numpssm);
	DSLight * createNewDLight(const Ogre::Light* light, SceneManager * sceneManager);
	Ogre::HlmsDatablock* getLightMaterial(const Ogre::Light * light,const Matrix4 viewmat,ColourValue) ;
	void updateLightData(SceneManager * sceneManager,
			const CompositorShadowNode * shadownode,int numpssm);
	Ogre::HlmsParamVec getDatablockParams(const Light *,const Matrix4,ColourValue,int ShadowMapID,int casterID);

};

} /* namespace Ogre */

#endif /* LIGHT_DSLIGHTMANAGER_H_ */
