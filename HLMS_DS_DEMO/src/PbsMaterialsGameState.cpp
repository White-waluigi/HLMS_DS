#include "PbsMaterialsGameState.h"
#include "CameraController.h"
#include "GraphicsSystem.h"

#include "OgreSceneManager.h"
#include "OgreItem.h"

#include "OgreMeshManager.h"
#include "OgreMeshManager2.h"
#include "OgreMesh2.h"

#include "OgreCamera.h"
#include "OgreRenderWindow.h"

#include "Datablocks/DSDatablock.h"
#include "OgreHlmsSamplerblock.h"

#include "OgreRoot.h"
#include "OgreHlmsManager.h"
#include "OgreHlmsTextureManager.h"
#include "OgreHlmsDS.h"
#include "Helper/HLMSDSWorkspaceListener.h"

#include "Museum/Museum.h"

using namespace Demo;

namespace Demo {



Ogre::Real PbsMaterialsGameState::randf() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
Ogre::Vector3 PbsMaterialsGameState::randv1() {
	Ogre::Vector3 retv;
//	float ret = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
//	retv.x = ret;
//
//	ret = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
//	retv.z = ret;
//
//	ret = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
//	retv.y = ret;

	float big,small,swap;

	big = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	small = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	if(big<small){
		swap=big;
		big=small;
		small=swap;
	}
	retv.x=big-small;
	retv.y=small;
	retv.z=1.0-big;


	//retv.normalise();

	return retv;
}
Ogre::Vector3 PbsMaterialsGameState::randv2() {
	Ogre::Vector3 retv;

	float big,small,swap;

	big = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	if(big<0.3){
		retv.z=1;
	}
	if(big<0.7){
		retv.y=1;
	}
	if(big<1){
		retv.x=1;
	}

	//retv.normalise();

	return retv;
}
Ogre::Vector3 PbsMaterialsGameState::RainbowPhase(float f){
	float level = (f );

	float x=fmod(f,1.0);
	float y=1-fmod(f,1.0);

	float r,g,b;
	if(level<1){
		r=y;
		g=x;
		b=0;
	}else if(level<2){
		r=0;
		g=y;
		b=x;
	}else if(level<3){
		r=x;
		g=0;
		b=y;
	}

	return Ogre::Vector3(r,g,b);
}
Ogre::Vector3 PbsMaterialsGameState::randv3() {
	Ogre::Vector3 retv=Ogre::Vector3::ZERO;
	float big,small,swap;

	big = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	if(big>0.8){
		retv.x=0.4;
		retv.y=0.5;
		retv.z=0.3;
	}
	return retv;
}

PbsMaterialsGameState::PbsMaterialsGameState(
		const Ogre::String &helpDescription) :
		TutorialGameState(helpDescription), mAnimateObjects(true), mNumSpheres(
				0), mTransparencyMode(0), mTransparencyValue(1.0f) {
	memset(mSceneNode, 0, sizeof(mSceneNode));
}
//-----------------------------------------------------------------------------------
void PbsMaterialsGameState::createScene01(void) {
	//for the trees
	srand ( time(NULL) );
	Ogre::SceneManager *sceneManager = mGraphicsSystem->getSceneManager();
	Ogre::HlmsManager *hlmsManager =
			mGraphicsSystem->getRoot()->getHlmsManager();
	Ogre::HlmsTextureManager *hlmsTextureManager =
			hlmsManager->getTextureManager();

	Ogre::SceneNode *rootNode = sceneManager->getRootSceneNode();
	mGraphicsSystem->getCamera()->setPosition(-19.5,2,-15);
	mGraphicsSystem->getCamera()->setDirection(1,0,0);
	mGraphicsSystem->getCamera()->setFarClipDistance(10000);
	hlmsDS = static_cast<Ogre::HlmsDS*>(hlmsManager->getHlms(Ogre::HLMS_PBS));
	museum=new Museum(hlmsDS,sceneManager);
	museum->initExhibits();
	museum->initLights();
	museum->setupExhibits();
	museum->setupMuseum();
	museum->setupLights();

	mGraphicsSystem->mWorkspace->setListener(hlmsDS->mWSListener);

//	Ogre::Light *light = sceneManager->createLight();
//	Ogre::SceneNode *lightNode = rootNode->createChildSceneNode();
//	lightNode->attachObject(light);
//	light->setPowerScale(3.1f);
//	light->setType(Ogre::Light::LT_DIRECTIONAL);
//	//light->setDirection(Ogre::Vector3(-1.5, -1.7, -1.2).normalisedCopy());
//	light->setDirection(Ogre::Vector3(-0.2,-0.50,-0.7).normalisedCopy());
//	light->setCastShadows(true);
//	lightNode->setPosition(-10.0f, 10.0f, 10.0f);
//	light->setDiffuseColour(0.7f, 1.0f, 1.0f); //Warm



//
//	mLightNodes[0] = lightNode;
//
//	sceneManager->setAmbientLight(
//			Ogre::ColourValue(0.3f, 0.5f, 0.7f) * 0.1f * 0.75f,
//			Ogre::ColourValue(0.6f, 0.45f, 0.3f) * 0.065f * 0.75f,
//			-light->getDirection() + Ogre::Vector3::UNIT_Y * 0.2f);
//
//	light = sceneManager->createLight();
//	lightNode = rootNode->createChildSceneNode();
//	lightNode->attachObject(light);
//	light->setDiffuseColour(1.0f, 1.0f, 0.0f); //Warm
//	light->setSpecularColour(0.8f, 0.4f, 0.2f);
//	light->setPowerScale(1.0);
//	light->setType(Ogre::Light::LT_SPOTLIGHT);
//	lightNode->setPosition(-10.0f, 10.0f, 10.0f);
//	light->setDirection(Ogre::Vector3(1, -1, -1).normalisedCopy());
//	light->setAttenuationBasedOnRadius(10.0f, 0.01f);
//	light->setCastShadows(true);
//
//	mLightNodes[1] = lightNode;
//
//	light = sceneManager->createLight();
//	lightNode = rootNode->createChildSceneNode();
//	lightNode->attachObject(light);
//	light->setDiffuseColour(0.0f,1.0f, 1.0f); //Cold
//	light->setSpecularColour(0.2f, 0.4f, 0.8f);
//	light->setPowerScale(1.0);
//	light->setType(Ogre::Light::LT_SPOTLIGHT);
//	lightNode->setPosition(10.0f, 10.0f, -10.0f);
//	light->setDirection(Ogre::Vector3(-1, -1, 1).normalisedCopy());
//	light->setAttenuationBasedOnRadius(10.0f, 0.01f);
//	light->setCastShadows(true);
//
//	mLightNodes[2] = lightNode;
//
//	light = sceneManager->createLight();
//	lightNode = rootNode->createChildSceneNode();
//	lightNode->attachObject(light);
//	light->setDiffuseColour(1.0f, 0.0f, 0.0f); //Cold
//	light->setSpecularColour(0.2f, 0.4f, 0.8f);
//	light->setPowerScale(1.0);
//	light->setType(Ogre::Light::LT_POINT);
//	lightNode->setPosition(14.0f, 3.0f, 14.0f);
//	light->setDirection(Ogre::Vector3(-1, -1, 1).normalisedCopy());
//	light->setAttenuation(200, 1.0, 0.3, 0);
//	light->setCastShadows(true);
//
//	mLightNodes[3] = lightNode;
//
//	light = sceneManager->createLight();
//	lightNode = rootNode->createChildSceneNode();
//	lightNode->attachObject(light);
//	light->setDiffuseColour(0.0f, 0.0f, 1.0f); //Cold
//	light->setSpecularColour(0.2f, 0.4f, 0.8f);
//	light->setPowerScale(1.0);
//	light->setType(Ogre::Light::LT_POINT);
//	lightNode->setPosition(-14.0f, 3.0f, -14.0f);
//	light->setDirection(Ogre::Vector3(-1, -1, 1).normalisedCopy());
//	light->setAttenuation(200, 1.0, 0.3, 0);
//	light->setCastShadows(true);
//
//	mLightNodes[4] = lightNode;
//
//	light = sceneManager->createLight();
//	lightNode = rootNode->createChildSceneNode();
//	lightNode->attachObject(light);
//	light->setDiffuseColour(0.2f, 0.4f, 0.8f); //Cold
//	light->setSpecularColour(0.2f, 0.4f, 0.8f);
//	light->setPowerScale(1.0);
//	light->setType(Ogre::Light::LT_SPOTLIGHT);
//	lightNode->setPosition(10.0f, 20.0f, -10.0f);
//	light->setDirection(Ogre::Vector3(0, -1, 0).normalisedCopy());
//	light->setAttenuationBasedOnRadius(10.0f, 0.01f);
//	light->setCastShadows(true);

//	mLight=light;
//	mLightNodes[5] = lightNode;

	mCameraController = new CameraController(mGraphicsSystem, false);

	TutorialGameState::createScene01();
}
//-----------------------------------------------------------------------------------
void PbsMaterialsGameState::update(float timeSinceLast) {
	museum->updateExhibits(timeSinceLast);

	float sunlight=timeSinceLast*(pow(blocksun,2));
	if(blocksun==3){
		sunlight=0;
	}

	daycycle= museum->updateLights(sunlight);

	TutorialGameState::update(timeSinceLast);
}
//-----------------------------------------------------------------------------------
void PbsMaterialsGameState::generateDebugText(float timeSinceLast,
		Ogre::String &outText) {
	Ogre::uint32 visibilityMask =
			mGraphicsSystem->getSceneManager()->getVisibilityMask();




	TutorialGameState::generateDebugText(timeSinceLast, outText);


	outText += "\n Camera Position: " + Ogre::StringConverter::toString(mGraphicsSystem->getCamera()->getDerivedPosition());

	outText += "\n f6-f9 to set Debug Mode:";
	outText += "\nY to set Debug Forward/Backward:";
	outText += (cycleMode==1?"Forward":"Backward");
	outText += "\nGBuffer: "+Ogre::StringConverter::toString(cycleDM)+" Light: "+Ogre::StringConverter::toString(cycleFBM)+" Z: "+Ogre::StringConverter::toString(cycleX)+" W: "+Ogre::StringConverter::toString(cycleY);


	outText += "\n\ndaycycle: "+Ogre::StringConverter::toString(daycycle.dayphase)+"\n";

	outText += "\nmorningweight: "+Ogre::StringConverter::toString(daycycle.morningweight)+"\n";
	outText += "\nsunweight: "+Ogre::StringConverter::toString(daycycle.dayweight)+"\n";
	outText += "\nevening: "+Ogre::StringConverter::toString(daycycle.eveningweight)+"\n";
	outText += "\nnight: "+Ogre::StringConverter::toString(daycycle.nightweight)+"\n";


}
//-----------------------------------------------------------------------------------
void PbsMaterialsGameState::setTransparencyToMaterials(void) {
	Ogre::HlmsManager *hlmsManager =
			mGraphicsSystem->getRoot()->getHlmsManager();
//
//        assert( dynamic_cast<Ogre::HlmsPbs*>( hlmsManager->getHlms( Ogre::HLMS_PBS ) ) );
//
	Ogre::HlmsDS *hlmsDS = static_cast<Ogre::HlmsDS*>(hlmsManager->getHlms(
			Ogre::HLMS_PBS));
//
//        Ogre::HlmsPbsDatablock::TransparencyModes mode =
//                static_cast<Ogre::HlmsPbsDatablock::TransparencyModes>( mTransparencyMode );

//        if( mTransparencyValue >= 1.0f )
//            mode = Ogre::HlmsPbsDatablock::None;
//
//        if( mTransparencyMode < 1.0f && mode == Ogre::HlmsPbsDatablock::None )
//            mode = Ogre::HlmsPbsDatablock::Transparent;

	for (size_t i = 0; i < mNumSpheres; ++i) {
		Ogre::String datablockName = "Test"
				+ Ogre::StringConverter::toString(i);
		Ogre::DSDatablock *datablock =
				static_cast<Ogre::DSDatablock*>(hlmsDS->getDatablock(
						datablockName));

//            datablock->setTransparency( mTransparencyValue, mode );
	}
}
//-----------------------------------------------------------------------------------
void PbsMaterialsGameState::keyReleased(const SDL_KeyboardEvent &arg) {
	if ((arg.keysym.mod & ~(KMOD_NUM | KMOD_CAPS)) != 0) {
		TutorialGameState::keyReleased(arg);
		return;
	}

	if (arg.keysym.sym == SDLK_F2) {
		mAnimateObjects = !mAnimateObjects;
	} else if (arg.keysym.sym == SDLK_F3) {
		Ogre::uint32 visibilityMask =
				mGraphicsSystem->getSceneManager()->getVisibilityMask();
		bool showMovingObjects = (visibilityMask & 0x00000001);
		showMovingObjects = !showMovingObjects;
		visibilityMask &= ~0x00000001;
		visibilityMask |= (Ogre::uint32) showMovingObjects;
		mGraphicsSystem->getSceneManager()->setVisibilityMask(visibilityMask);
	} else if (arg.keysym.sym == SDLK_F4) {
		Ogre::uint32 visibilityMask =
				mGraphicsSystem->getSceneManager()->getVisibilityMask();
		bool showPalette = (visibilityMask & 0x00000002) != 0;
		showPalette = !showPalette;
		visibilityMask &= ~0x00000002;
		visibilityMask |= (Ogre::uint32) (showPalette) << 1;
		mGraphicsSystem->getSceneManager()->setVisibilityMask(visibilityMask);
	} else if (arg.keysym.sym == SDLK_F5) {
//            mTransparencyMode = mTransparencyMode == Ogre::HlmsPbsDatablock::Fade ?
//                                                            Ogre::HlmsPbsDatablock::Transparent :
//                                                            Ogre::HlmsPbsDatablock::Fade;
		if (mTransparencyValue != 1.0f)
			setTransparencyToMaterials();
	} else if (arg.keysym.scancode == SDL_SCANCODE_KP_PLUS) {
//		if (mTranTutorialGameStatesparencyValue < 1.0f) {
//			mTransparencyValue += 0.1f;
//			mTransparencyValue = Ogre::min(mTransparencyValue, 1.0f);
//			setTransparencyToMaterials();
//		}
	} else if (arg.keysym.scancode == SDL_SCANCODE_MINUS
			|| arg.keysym.scancode == SDL_SCANCODE_KP_MINUS) {
		if (mTransparencyValue > 0.0f) {
			mTransparencyValue -= 0.1f;
			mTransparencyValue = Ogre::max(mTransparencyValue, 0.0f);
			setTransparencyToMaterials();
		}
	} else if (arg.keysym.sym == SDLK_y) {
		cycleMode*=-1;
	} else if (arg.keysym.sym == SDLK_F6) {
		cycleDM+=cycleMode;
		int max = Ogre::HlmsDS::DM_NUM;

		cycleDM %= *reinterpret_cast<int*>(&max);
		if(cycleDM<0){cycleDM=0;}


		hlmsDS->setDebugMode((Ogre::HlmsDS::DebugMode) cycleDM);


	} else if (arg.keysym.sym == SDLK_F7) {
		cycleFBM+=cycleMode;
		int max = Ogre::HlmsDS::FBM_NUM;

		cycleFBM %= *reinterpret_cast<int*>(&max);
		if(cycleFBM<0){cycleFBM=0;}


		hlmsDS->setFullBright((Ogre::HlmsDS::FullBrightMode) cycleFBM);
	} else if (arg.keysym.sym == SDLK_F8) {
		cycleX+=cycleMode;

		if(cycleX<0){cycleX=0;}
		hlmsDS->setDebugz(cycleX);
	} else if (arg.keysym.sym == SDLK_F9) {
		cycleY+=cycleMode;

		if(cycleY<0){cycleY=0;}
		hlmsDS->setDebugw(cycleY);
	} else if (arg.keysym.sym == SDLK_l) {
		museum->slowDown=!museum->slowDown;
	} else if (arg.keysym.sym == SDLK_h) {
		blocksun++;
		blocksun%=4;

	} else {
		TutorialGameState::keyReleased(arg);
	}
}
}
