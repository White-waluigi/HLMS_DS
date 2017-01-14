/*
 * HLMSDSWorkspaceListener.h
 *
 *  Created on: Jul 7, 2016
 *      Author: marvin
 */

#ifndef SRC_HELPER_HLMSDSWORKSPACELISTENER_H_
#define SRC_HELPER_HLMSDSWORKSPACELISTENER_H_
#include "Ogre.h"
#include "OgreVector4.h"
#include "Compositor/OgreCompositorWorkspace.h"
#include "Compositor/OgreCompositorWorkspaceListener.h"

namespace Ogre {



class HLMSDSWorkspaceListener : public CompositorWorkspaceListener {
public:


	const static int ShadowID=800;
	const static int GbufferID=801;
	const static int LightID=802;
	const static int ForwardID=803;
	const static int PostID=804;


	enum PassType{
		PT_UNKOWN,
		PT_SHADOW,
		PT_GBUFFER,
		PT_LIGHT,
		PT_FORWARD,
		PT_POST

	} passType;

	uint32 passID;
	HLMSDSWorkspaceListener();
	virtual void passPreExecute(CompositorPass * pass);
	virtual ~HLMSDSWorkspaceListener();
};

} /* namespace Ogre */

#endif /* SRC_HELPER_HLMSDSWORKSPACELISTENER_H_ */