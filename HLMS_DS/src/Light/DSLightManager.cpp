/*
 * DSLightManager.cpp
 *
 *  Created on: 24.11.2015
 *      Author: marvin
 */

#include "../Light/DSLightManager.h"
#include "StringValueUtils.h"
#include "OgreQuaternion.h"
#include "StringValueUtils.h"
#include "OgreHlmsManager.h"
#include "../Datablocks/DSLightDatablock.h"
#include "Compositor/OgreCompositorShadowNode.h"
namespace Ogre {

DSLightManager::DSLightManager(HlmsDS * parent) {
	this->hlmsman = parent;
	//todo fix this
	this->ambientLightID = Ogre::Id::generateNewId<Ogre::Light>();
//	this->ambientLightID= 4294123295;
}

DSLightManager::~DSLightManager() {
	// TODO Auto-generated destructor stub
}

bool DSLightManager::CheckForNewLights(SceneManager * sceneManager,
		const CompositorShadowNode * shadownode, int numpssm) {
	this->numPssm = numpssm;

	const LightListInfo &globalLightList = sceneManager->getGlobalLightList();

	this->shadowNode = shadownode;
	//todo Hacky way to get uinque light ID for ambient light without actualy having a an Ogre Light object
	if (this->lightList.find(ambientLightID) == this->lightList.end()) {

		DSLight* dlight = createNewDLight(NULL, sceneManager);
		dlight->setAmbient(true);
		this->lightList.insert(
				std::pair<IdType, DSLight *>(ambientLightID, dlight));

	} else {
	}

	for (uint i = 0; i < globalLightList.lights.size(); i++) {
		if (this->lightList.find(globalLightList.lights[i]->getId())
				== this->lightList.end()) {

			if (globalLightList.lights[i]->getType() != Light::LT_DIRECTIONAL) {
				DSLight* dlight = createNewDLight(globalLightList.lights[i],
						sceneManager);
				this->lightList.insert(
						std::pair<IdType, DSLight *>(
								globalLightList.lights[i]->getId(), dlight));
			}
		} else {

		}
	}

	for (uint i = 0; i < globalLightList.lights.size(); i++) {
		if (this->lightList.find(globalLightList.lights[i]->getId())
				== this->lightList.end()) {

			if (globalLightList.lights[i]->getType() == Light::LT_DIRECTIONAL) {
				DSLight* dlight = createNewDLight(globalLightList.lights[i],
						sceneManager);
				this->lightList.insert(
						std::pair<IdType, DSLight *>(
								globalLightList.lights[i]->getId(), dlight));
			}
		} else {

		}
	}

	map<IdType, DSLight *>::iterator it;

	for (it = lightList.begin(); it != lightList.end(); it++) {
		if (it->second->isDead()) {
			delete it->second;
			lightList.erase(it->first);

		}

	}
	return true;
}
DSLight* DSLightManager::createNewDLight(const Ogre::Light* light,
		SceneManager * sceneManager) {
	String meshName;
	int type = 0;
	if (light == NULL) {
		type = 4;
	} else if (light->getType() == Ogre::Light::LT_POINT) {
		type = 1;
	} else if (light->getType() == Ogre::Light::LT_DIRECTIONAL) {
		type = 0;
	} else if (light->getType() == Ogre::Light::LT_SPOTLIGHT) {
		type = 2;

	}
	Ogre::DSLight * item = new Ogre::DSLight(light,
			Ogre::Id::generateNewId<Ogre::MovableObject>(),
			&sceneManager->_getEntityMemoryManager(Ogre::SCENE_DYNAMIC),
			sceneManager, 0);

	//					Ogre::Item *item =sceneManager->createItem(meshName,
	//														Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
	//														Ogre::SCENE_DYNAMIC);

	item->setVisibilityFlags(0x0008000);

	item->setDatablock(
			getLightMaterial(light,
					sceneManager->getCameraInProgress()->getViewMatrix(),
					sceneManager->getAmbientLightUpperHemisphere()));
	size_t idx = 0 * 4 + 16;
	item->parentSN =
			sceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->createChildSceneNode(
					Ogre::SCENE_DYNAMIC);

	item->parentSN->attachObject(item);

//	if (light != NULL) {
//		item->parentSN->setPosition(
//				light->getParentNode()->_getDerivedPositionUpdated());
//		//					sn->setScale(light->getAttenuationRange(),
//		item->parentSN->setOrientation(
//				light->getParentNode()->_getDerivedOrientationUpdated());
//	}
	//are lights affected by scale?
	//item->parentSN->setScale(light->getParentNode()->_getDerivedScale());

	//							light->getAttenuationRange(),
	//							light->getAttenuationRange());

//	item->parentSN->setScale(1, 1, 1);
//
//	if (light != NULL) {
//
//		Quaternion quat = Vector3::UNIT_Y.getRotationTo(light->getDerivedDirectionUpdated());
//
//		item->parentSN->setOrientation(quat);
//		item->parentSN->_getFullTransformUpdated();
//	}
	return item;

}
Ogre::HlmsDatablock* DSLightManager::getLightMaterial(const Ogre::Light * light,
		const Matrix4 viewmat, ColourValue AmbientColour) {
	Ogre::DSLightDatablock * defaultLightMaterial = NULL;
	if (defaultLightMaterial == NULL) {
		uint Number = ambientLightID;
		if (light != NULL) {
			Number = light->getId();	//number to convert int a string
		}
		String Result;	//string which will contain the result

		stringstream convert; // stringstream used for the conversion

		convert << Number; //add the value of Number to the characters in the stream

		Result = convert.str(); //set Result to the content of the stream
		Ogre::HlmsBlendblock blendblock;
		Ogre::HlmsMacroblock macro;
		macro.mDepthFunc = Ogre::CMPF_ALWAYS_PASS;

		macro.mDepthCheck = false;
		if (light == NULL || light->getType() == Light::LT_DIRECTIONAL) {
			macro.mCullMode = Ogre::CULL_ANTICLOCKWISE;
			blendblock.setBlendType(Ogre::SBT_ADD);

			if(light == NULL ){
				//Ambient Light
				macro.mDepthFunc = Ogre::CMPF_GREATER;
				blendblock.setBlendType(Ogre::SBT_ADD);

			}else{
				macro.mDepthFunc = Ogre::CMPF_ALWAYS_PASS;

			}
		} else {
			macro.mCullMode = Ogre::CULL_ANTICLOCKWISE;
			blendblock.setBlendType(Ogre::SBT_ADD);

		}



		Ogre::HlmsParamVec params = getDatablockParams(light, viewmat,
				AmbientColour, -1, -1);

		defaultLightMaterial =
				static_cast<Ogre::DSLightDatablock*>(this->hlmsman->createDatablock(
						Result + "_Light_Material", "Light_Material", macro,
						blendblock, params));
		//defaultLightMaterial->setDiffuse(Vector3(0.5, 0.7, 0.3));
		//this->defaultLightMaterial->setDiffuse(Ogre::ColourValue(1, 1, 0));

		//this->defaultLightMaterial->setLightMaterial(true);
	}
	assert(defaultLightMaterial);
	return defaultLightMaterial;

}
Vector4 RainbowPhase(float f) {
	float level = (f);

	float x = fmod(f, 1.0);
	float y = 1 - fmod(f, 1.0);

	float r, g, b;
	if (level < 1) {
		r = y;
		g = x;
		b = 0;
	} else if (level < 2) {
		r = 0;
		g = y;
		b = x;
	} else if (level < 3) {
		r = x;
		g = 0;
		b = y;
	}

	return Vector4(r, g, b, 0.0);
}
void DSLightManager::updateLightData(SceneManager * sceneManager,
		const CompositorShadowNode * shadownode, int numpssm) {

	this->numPssm = numpssm;

	for (std::map<IdType, DSLight *>::iterator iterator = lightList.begin();
			iterator != lightList.end(); iterator++) {

		if (iterator->second->parent != NULL) {

			iterator->second->parent->getParentNode()->_getFullTransformUpdated();

			iterator->second->parentSN->setPosition(
					iterator->second->parent->getParentNode()->_getDerivedPositionUpdated());

			//					sn->setScale(light->getAttenuationRange(),
			iterator->second->parentSN->setOrientation(
					iterator->second->parent->getParentNode()->_getDerivedOrientationUpdated());
			iterator->second->parentSN->_getFullTransformUpdated();

			iterator->second->parentSN->setPosition(
					iterator->second->parent->getParentNode()->_getDerivedPositionUpdated());
			//					sn->setScale(light->getAttenuationRange(),
			iterator->second->parentSN->setOrientation(
					iterator->second->parent->getParentNode()->_getDerivedOrientationUpdated());

			//are lights affected by scale?
			iterator->second->parentSN->setScale(1, 1, 1);

			//							light->getAttenuationRange(),
			//							light->getAttenuationRange());

			//iterator->second->parentSN->setScale(1, 1, 1);

			//iterator->second->parentSN->attachObject(item);

			Quaternion quat = Vector3::UNIT_Y.getRotationTo(
					iterator->second->parent->getDerivedDirectionUpdated());

			iterator->second->parentSN->setOrientation(quat);
			iterator->second->parentSN->_getFullTransformUpdated();
		}
		//are lights affected by scale?
		//item->parentSN->setScale(light->getParentNode()->_getDerivedScale());
		if (shadownode != NULL && iterator->second->parent != NULL) {
			LightClosestArray test = shadownode->getShadowCastingLights();

			//Why is this the only way to link shadow maps to lights?
			int currentShadowMap = 0;
			for (int i = 0; i < test.size(); i++) {
				if (shadownode->getShadowCastingLights()[i].light->getId()
						== iterator->second->parent->getId()) {

					iterator->second->ShadowCasterID = i;

					//iterator->second->ShadowMapId =shadownode->getShadowCastingLights()[i].globalIndex;
					iterator->second->ShadowMapId = currentShadowMap;

				}
				if (shadownode->getShadowCastingLights()[i].light->getType()
						== Light::LT_DIRECTIONAL) {
					currentShadowMap += this->hlmsman->NumPssmSplits;
				} else {
					currentShadowMap++;
				}

			}
		}
		Ogre::HlmsParamVec paramVec = getDatablockParams(
				iterator->second->parent,
				sceneManager->getCameraInProgress()->getViewMatrix(),
				sceneManager->getAmbientLightUpperHemisphere(),
				iterator->second->ShadowMapId,
				iterator->second->ShadowCasterID);

		DSLightDatablock* db =
				static_cast<DSLightDatablock*>(iterator->second->getDatablock());

//
//		static float lightpos=0;
//		static float lightposincr=0;
//		static float lightposincrincr=0.0000001;
//
//
//		if(lightpos<-1){
//			lightposincrincr=0.0000001;
//		}
//		if(lightpos>1){
//			lightposincrincr=-0.0000001;
//		}
//		lightposincr+=lightposincrincr;
//		lightpos+=lightposincr;
//
//		lightparams[1][0]=lightpos;

		db->setData(paramVec);
	}
}

Ogre::HlmsParamVec DSLightManager::getDatablockParams(const Light* light,
		const Matrix4 viewmat, ColourValue AmbientColour, int ShadowMapID,
		int casterID) {

	Ogre::HlmsParamVec params = Ogre::HlmsParamVec();
	params.push_back(std::pair<IdString, String>(IdString("type"), "light"));

	typedef union {
		uint i;
		float f;
	} type_;

	type_ smID;
	smID.i = ShadowMapID;
	type_ LightID;

	type_ type;
	if (light != NULL) {
		LightID.i = light->getId();
		type.i = light->getType();
		params.push_back(
				std::pair<IdString, String>(IdString("lightparams"),
						StringValueUtils::getVectorStr(
								Vector4(type.f, light->getPowerScale(),
										LightID.f, smID.f))));
	} else {
		type.i = 4;
		params.push_back(
				std::pair<IdString, String>(IdString("lightparams"),
						StringValueUtils::getVectorStr(
								Vector4(type.f, 1.0, ambientLightID, smID.f))));
	}

	if (light != NULL) {

		Vector4 vec =
				(light->getType() == Light::LT_DIRECTIONAL) ?
						viewmat
								* Vector4(light->getDerivedDirection().x,
										light->getDerivedDirection().y,
										light->getDerivedDirection().z, 0) :

						viewmat.transformAffine(light->getAs4DVector());

//		vec = (light->getType() == Light::LT_DIRECTIONAL) ?
//				Vector4(light->getDerivedDirection().x,
//						light->getDerivedDirection().y,
//						light->getDerivedDirection().z, 0) :
//				(light->getAs4DVector());

		params.push_back(
				std::pair<IdString, String>(IdString("position"),
						StringValueUtils::getVectorStr(vec)));
		params.push_back(
				std::pair<IdString, String>(IdString("diffuse"),
						StringValueUtils::getColorStr(
								light->getDiffuseColour())));
		params.push_back(
				std::pair<IdString, String>(IdString("specular"),
						StringValueUtils::getColorStr(
								light->getSpecularColour())));

		params.push_back(
				std::pair<IdString, String>(IdString("attentuation"),
						StringValueUtils::getVectorStr(
								Vector4(light->getAttenuationRange(),
										light->getAttenuationConstant(),
										light->getAttenuationLinear(),
										light->getAttenuationQuadric()))));

		Vector4 vec2 = viewmat
				* Vector4(light->getDerivedDirection().x,
						light->getDerivedDirection().y,
						light->getDerivedDirection().z, 0);

		vec2 = Vector4(light->getDerivedDirection().x,
				light->getDerivedDirection().y, light->getDerivedDirection().z,
				0);

		params.push_back(
				std::pair<IdString, String>(IdString("spotdirection"),
						StringValueUtils::getVectorStr((vec2))));
		params.push_back(
				std::pair<IdString, String>(IdString("spotparams"),
						StringValueUtils::getVectorStr(
								Vector4(
										Math::Cos(
												light->getSpotlightInnerAngle().valueRadians()
														* 0.5f),
										Math::Cos(
												light->getSpotlightOuterAngle().valueRadians()
														* 0.5f),
										light->getSpotlightFalloff(), 1.0))));
		Real fNear, fFar;
		Real depthRange;
		float wi = 0;
		float he = 0;
		Matrix4 mat = Matrix4::IDENTITY;
		if (light->getType() == Light::LT_DIRECTIONAL) {

			std::stringstream ssSR;
			std::stringstream ssmat;
			std::stringstream ssDR;
			std::stringstream ssPSSM;
			ssPSSM << this->numPssm<<";";

			for (int i = 0; i < this->numPssm; i++) {

				if (ShadowMapID != -1) {
					wi = (float) shadowNode->getLocalTextures().at(
							ShadowMapID + i).textures[0]->getWidth();
					he = (float) shadowNode->getLocalTextures().at(
							ShadowMapID + i).textures[0]->getHeight();

					mat = shadowNode->getViewProjectionMatrix(ShadowMapID + i);

					if (shadowNode != NULL) {
						shadowNode->getMinMaxDepthRange(ShadowMapID + i, fNear,
								fFar);
					}
					depthRange = fFar - fNear;
				}

				ssSR
						<< StringValueUtils::getVectorStr(
								Vector4(wi, he, 1.0 / wi, 1.0 / he)) << ";";

				ssmat << StringValueUtils::getMatrixStr(mat) << ";";

				ssDR
						<< StringValueUtils::getVectorStr(
								Vector4(fNear, fFar, 1.0 / depthRange,
										depthRange)) << ";";

				ssPSSM << shadowNode->getPssmSplits(0)->at(i+1)<<";";

			}
			String s = ssSR.str();
			params.push_back(
					std::pair<IdString, String>(IdString("shadowRes"),
							ssSR.str()));

			params.push_back(
					std::pair<IdString, String>(IdString("shadowMat"),
							ssmat.str()));

			params.push_back(
					std::pair<IdString, String>(IdString("shadowDr"),
							ssDR.str()));

			params.push_back(
					std::pair<IdString, String>(IdString("shadowPssmSplits"),
							ssPSSM.str()));
		} else {

			if (casterID != -1) {
				wi =
						(float) shadowNode->getLocalTextures().at(ShadowMapID).textures[0]->getWidth();
				he =
						(float) shadowNode->getLocalTextures().at(ShadowMapID).textures[0]->getHeight();
				mat = shadowNode->getViewProjectionMatrix(ShadowMapID);

				if (shadowNode != NULL) {
					shadowNode->getMinMaxDepthRange(ShadowMapID, fNear, fFar);
				}
				depthRange = fFar - fNear;
			}

			params.push_back(
					std::pair<IdString, String>(IdString("shadowRes"),
							StringValueUtils::getVectorStr(
									Vector4(wi, he, 1.0 / wi, 1.0 / he))));

			params.push_back(
					std::pair<IdString, String>(IdString("shadowMat"),
							StringValueUtils::getMatrixStr(mat)));

			params.push_back(
					std::pair<IdString, String>(IdString("shadowDr"),
							StringValueUtils::getVectorStr(
									Vector4(fNear, fFar, 1.0 / depthRange,
											depthRange))));

			String s = StringValueUtils::getMatrixStr(mat);

		}

		params.push_back(
				std::pair<IdString, String>(IdString("shadowParams"),
						StringValueUtils::getVectorStr(
								Vector4(smID.f, light->getShadowFarDistance(),
										light->getShadowNearClipDistance(),
										light->getShadowFarDistanceSquared()))));

		params.push_back(
				std::pair<IdString, String>(IdString("shadowQualityParams"),
						StringValueUtils::getVectorStr(Vector4(2, 0, 0, 0))));



//		for (int i = 0; i < 3; i++) {
//			if (casterID == -1) {
//				casterID = 0;
//				params.push_back(
//						std::pair<IdString, String>(
//								IdString(
//										"campos"
//												+ Ogre::StringConverter::toString(
//														i)),
//								StringValueUtils::getVectorStr(
//										Vector4(0, 0, 0, 0))));
//
//				continue;
//			}
//			params.push_back(
//					std::pair<IdString, String>(
//							IdString(
//									"campos"
//											+ Ogre::StringConverter::toString(
//													i)),
//							StringValueUtils::getVectorStr(
//									shadowNode->getViewProjectionMatrix(
//											casterID + i).inverse().getTrans())));
//
//		}

//		params.push_back(.
//				std::pair<IdString, String>(IdString("shadowParams"),
//						StringValueUtils::getVectorStr(
//								this->hlmsman->testdata)));

	} else {
		params.push_back(
				std::pair<IdString, String>(IdString("diffuse"),
						StringValueUtils::getColorStr(AmbientColour)));
	}
	std::sort(params.begin(), params.end());

	return params;

}
} /* namespace Ogre */
