/*
 * PassBufferManager.h
 *
 *  Created on: Jan 21, 2018
 *      Author: marvin
 *
 * Works similar to the Datablock, uses a list of Resources in order to initialize every Passes
 * Passbuffer, which is a buffer used by every renderable and contains some universally
 * useful Resources.
 *
 * The PassBuffer doesn't create any Textures, Pieces or Properties (yet)
 *
 * This Resources can be extended by the User by adding custom Resources to its list
 */

#ifndef SRC_DS_HLMSMANAGERS_PASSBUFFERMANAGER_H_
#define SRC_DS_HLMSMANAGERS_PASSBUFFERMANAGER_H_

#include "../Modules/ModuleReference.h"
#include "../Modules/Resources/DSResource.h"
#include "../Util/GPUPointer.h"
#include "../OgreHlmsDS.h"

namespace Ogre {
class PassBufferDefaultVal;
typedef DSResList resList;
class PassBufferManager : public ModuleReference{


	//Reuse Datablocks Resources for Passbuffer variables
	resList mRes;

	//List of PassBuffers
	HlmsDS::ConstBufferPackedVec *mPacked=NULL;
	//Size of PassBuffer
	int mPBSize=0;

	//Parent HLMS
	HlmsDS * mParent=NULL;

public:

	//create PassBuffer
	PassBufferManager(HlmsDS * parent);

	//Create Default Passbuffer used by Deferred Shading
	void initialize();

	//Update PassBuffer
	ConstBufferPacked * update(int i);
	//get Specified PassBuffer
	ConstBufferPacked * get(int i);

	//Clean up, delete all Resources and the PassBuffer
	virtual ~PassBufferManager();
};

} /* namespace Ogre */

#endif /* SRC_DS_HLMSMANAGERS_PASSBUFFERMANAGER_H_ */
