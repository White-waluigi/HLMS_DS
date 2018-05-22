/*
 * PassBufferDefaultVal.h
 *
 *  Created on: Jan 22, 2018
 *      Author: marvin
 *
 *  PassBufferDefaultVal is need by the PassBufferManager to create some default data necessary
 *  for the Scene to be rendered.
 *
 * These are:
 *
 * - ViewProjMatrix
 * - ViewMatrix
 * - Projection Matrix
 * - DepthRange
 * - RenderTargetSize
 *
 */

#ifndef SRC_DS_MODULES_RESOURCES_AUTOVALS_PASSBUFFERDEFAULTVAL_H_
#define SRC_DS_MODULES_RESOURCES_AUTOVALS_PASSBUFFERDEFAULTVAL_H_

#include "../Value.h"
#include "../Value.h"
#include <OgreHlmsListener.h>
#include "../../../HLMSManagers/PassBufferManager.h"


namespace Ogre {
//We need to be a HLMSListener to Hook into preparePassHash and get the required Scene Data to redirect to the PassBufferManager
class PassBufferDefaultVal: public Value ,public HlmsListener{

	//Parent PassBufferManager, need by some Resources which are used by the PassBufferManager
	PassBufferManager * mParentPB=NULL;
public:

	//We need a PassBufferManager to work
	static refreq * getRequiredRefs();
	//add Listener to HLMS_DS, set parent PassBufferManager
	PassBufferDefaultVal(reflist l);


	//collect data from HLMS_DS and save for later
	virtual void preparePassHash 	( 	const CompositorShadowNode *  	shadowNode,
			bool  	casterPass,
			bool  	dualParaboloid,
			SceneManager *  	sceneManager,
			Hlms *  	hlms
		);


	//upload collected data to GPU
	virtual void upload(GPUPointer * ptr);
	//Buffer Size is static and defaults to 2kb
	virtual int getBufferSize();
	//Setup temporary holding Array
	virtual void initData();

	//clean up
	virtual ~PassBufferDefaultVal();

};

} /* namespace Ogre */

#endif /* SRC_DS_MODULES_RESOURCES_AUTOVALS_PASSBUFFERDEFAULTVAL_H_ */
