/*
 * LightManager.h
 *
 *  Created on: Feb 15, 2018
 *      Author: marvin
 *
 *
 *  Creates and destroy new Light Geometry as they are created and deleted by the User
 *
 *  !!!!!!!!!! NOTE !!!!!!!!!!!
 *  Light and Shadow Data are now automatically handled by the Datablock,
 *  or more specifically the LightAutoVal
 *
 */

#ifndef SRC_DS_HLMSMANAGERS_LIGHTMANAGER_H_
#define SRC_DS_HLMSMANAGERS_LIGHTMANAGER_H_

#include "../Modules/Resources/DSResource.h"
#include <OgreHlmsListener.h>


namespace Ogre {
class DSLight;
//We need to hock into HLMSDS events
class LightManager: public ModuleReference ,public HlmsListener{
	//Parent HLMS
	HlmsDS * mParent=NULL;

	//All lists are typedefed for readablity
	typedef std::map<IdType, DSLight*> DSLightMap;

	//List of created Lights
	DSLightMap mLights;

	//Static ID for ambient Light
	IdType ambientLightID;
public:
	//Creates ambient Light by default
	LightManager(HlmsDS * parent);


	//Hook into Pass function of HLMS
	virtual void preparePassHash 	( 	const CompositorShadowNode *  	shadowNode,
			bool  	casterPass,
			bool  	dualParaboloid,
			SceneManager *  	sceneManager,
			Hlms *  	hlms
		);

	//Initialize a new Light Geometry Object once needed
	DSLight * createNewDSLight(const Ogre::Light* light, SceneManager* sceneManager);

	//Remove deleted Lights from Scene
	DSLight * DeleteDSLight(const Ogre::Light* light, SceneManager* sceneManager);


	//Deletes all DSLights (DO NOT DELETE OGRE LIGHTS)
	virtual ~LightManager();
};

} /* namespace Ogre */

#endif /* SRC_DS_HLMSMANAGERS_LIGHTMANAGER_H_ */
