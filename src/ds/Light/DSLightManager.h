/*
 * DSLightManager.h
 *
 *  Created on: 24.11.2015
 *      Author: marvin
 */

#ifndef LIGHT_DSLIGHTMANAGER_H_
#define LIGHT_DSLIGHTMANAGER_H_
#include <OgreTexBufferPacked.h>

#include "OgreHlmsDS.h"
#include "Light/DSLight.h"
namespace Ogre {
//Keeps track of Ligths and their DS Light Volumes so users can just create Ogre Lights without knowing how DS actually works
class DSLightManager {
public:
	bool initalised=false;
	static const int MAX_LIGHT_FORWARD = 128;
	static const int getForwardLightBufferSize();

	Ogre::HlmsDS * hlmsman;
	IdType ambientLightID;
	DSLightManager(HlmsDS * parent);
	virtual ~DSLightManager();

	const CompositorShadowNode * shadowNode;
	int numPssm = 4;


	std::map<IdType, DSLight *> lightList;


	//Specialbuffer for Forwarded Material
	TexBufferPacked* ForwardLightBuffer;

	bool CheckForNewLights(SceneManager * sceneManager,
			const CompositorShadowNode * shadownode, int numpssm);
	DSLight * createNewDLight(const Ogre::Light* light,
			SceneManager * sceneManager);
	Ogre::HlmsDatablock* getLightMaterial(const Ogre::Light * light,
			const Matrix4 viewmat, ColourValue);
	void updateLightData(SceneManager * sceneManager,
			const CompositorShadowNode * shadownode, int numpssm);
	Ogre::HlmsParamVec getDatablockParams(const Light *, const Matrix4,
			ColourValue, int ShadowMapID, int stid, int casterID);
	int findShadowIDForLight(const Ogre::Light* id);
	void updateForwardLightBuffer();
	void initialise();
	DSLight* getDSLight(IdType id);

	TexBufferPacked*getForwardLightBuffer() {
		return ForwardLightBuffer;
	}

	void setForwardLightBuffer(TexBufferPacked* forwardLightBuffer) {
		ForwardLightBuffer = forwardLightBuffer;
	}
};

} /* namespace Ogre */

#endif /* LIGHT_DSLIGHTMANAGER_H_ */
