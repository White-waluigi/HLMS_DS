/*
 * LightAutoParams.h
 *
 *  Created on: Feb 15, 2018
 *      Author: marvin
 *
 * LightAutoVal is used by LightMaterials to keep the Ogre::Light properties in sync with
 * with the Material Buffer of the Light Material
 *
 * It is a child class of AutoVal and as such a Resource used by Datablocks
 *
 * It keeps track of the following properties of the Light:
 *
 * - Position relative to Camera
 * - Attenuation (Range)
 * - Diffuse Color
 * - Specualr Color
 * - Direction for Spotlights
 * - PSSM Splits for Directional Lights
 * - Hemispehere for Ambient Lights
 *
 * Since the Position needs to be updated every Frame anyway, this Resource is perpetually dirty
 * So every Frame this Material takes its assigned Light and updates its properties.
 *
 * The advantages of using this AutoVal is, that the LightManager is now way simpler,
 * the Light Geometry doesn't need to be changed anymore, since its direction can be changed using Shaders,
 * there is no need for a LightDatablock class anymore,
 * it is possible to create all kinds of crazy Light Materials by extending the Datablock with further Variables and Shadercode,
 * such as Fires, giscolights, gobolights, different shaped lights, automatically flickering light, God Rays, etc.
 * and since the Deferred Lighting is now almost fully integrated into the Material System,
 * it is even possible to completely change from the Client Application without modifying this Source Code
 *
 */

#ifndef SRC_DS_MODULES_RESOURCES_AUTOVALS_LIGHTAUTOVAL_H_
#define SRC_DS_MODULES_RESOURCES_AUTOVALS_LIGHTAUTOVAL_H_
#include "../Value.h"
namespace Ogre {

class LightAutoVal: public Value {
public:
	//Cleans up, but DOES NOT destroy the parent light
	virtual ~LightAutoVal();

	//Requires parent light and passbuffer
	static refreq * getRequiredRefs();
	//Initalizes Resource
	LightAutoVal(reflist l);

	//Update the Data from the Light and upload to GPU
	virtual void upload(GPUPointer * ptr);
	//Get the Size  of this Resource (may vary with Light Type)
	virtual int getBufferSize();


};

} /* namespace Ogre */

#endif /* SRC_DS_MODULES_RESOURCES_AUTOVALS_LIGHTAUTOVAL_H_ */
