/*
 * DSLight.h
 *
 *  Created on: 24.11.2015
 *      Author: marvin
 *
 *  Creates a fully Renderable Geometry used for Light Materials
 *  Light Datablocks need to be applied to something in Order to do something,
 *  and while it would be easy to just create a Quad that always spans the entire screen
 *  and apply a universal Light Datablock to it, which takes care of all the lighting, in order
 *  to increase performance and not run lighting code on Pixels which simply aren't affected by any
 *  given light, Light Geometry is used to only run light code on the affected screen area.
 *
 *  For example imageine a castle hallway filled with torches, which could look something like this:
 *
 *  __________________________________________________________________________________________________
 *
 *              o             o             o             o             o             o             o
 *              |             |             |             |             |             |             |
 *  ___________________________________________________________________________________________________
 *
 *  Now torches have a pretty small area of effect, because torches are pretty terrible light sources. sSo it would be wasteful to run code for each
 *  and every one on every pixel of the final render.
 *  Lets say the effect an area of 50cm around themselves, since who ever was responsible for replacing them forgot about it and they have become very dim
 *
 *  So we create a sphere geometry encompassing this space for each light
 *
 *  __________________________________________________________________________________________________
 *            .....          .....         .....         .....         .....         .....         .....
 *           :  o  :        :  o  :       :  o  :       :  o  :       :  o  :       :  o  :       :  o  :
 *           :  |  :        :  |  :       :  |  :       :  |  :       :  |  :       :  |  :       :  |  :
 *  __________.....__________....._________....._________....._________....._________....._________.....____________________________________________________________________________________
 *
 *
 *  Now the Pixel Shader for each given light is only executed when it is inside this light geometry
 *
 *  Ambient and Directional Lights, still use Quads, since their area of effect is always Global.
 */

#ifndef LIGHT_DSLIGHT_H_
#define LIGHT_DSLIGHT_H_
#include <OgreSimpleRenderable.h>
#include <Math/Array/OgreObjectMemoryManager.h>
#include <OgreSceneManager.h>
#include <OgreMovableObject.h>
#include <OgreRenderable.h>
#include <Ogre.h>
#include <OgreMovableObject.h>
#include <OgreRenderable.h>
#include <OgreIdString.h>
#include <OgreId.h>
#include <OgreSceneNode.h>

#include "../../Util/GeomUtils.h"
#include "../../Util/GPUPointer.h"
#include "../../Util/DSMath.h"
#include "../../Modules/ModuleReference.h"
namespace Ogre {
class HlmsDS;
class DSLight   : public Ogre::MovableObject, public Ogre::Renderable, public ModuleReference {
	//Special priviliedges for LightManager
	friend class LightManager;


	//Is the current Light an Ambient Light (Doesn't have a parent light)
	bool ambient=false;


public:
	//Parent Light (May be NULL)
    const Ogre::Light *mParentLight;

    //Parent HLMS
    HlmsDS * mParent=NULL;

    //Parent SceneNode, shared with Light
    Ogre::SceneNode *parentSN;


    //Initalize Geometry, create Renderable, applie Datablock and add it to Scene
	DSLight(const Ogre::Light * parent,HlmsDS * hlms,Ogre::IdType id,
    		Ogre::ObjectMemoryManager *objectMemoryManager,
    		Ogre::SceneManager *manager, Ogre::uint8 renderQueueId);

	//Delete Geometry and Renderable (DO NOT DELETE SCENE NODE OR PARENT LIGHT)
	virtual ~DSLight();

	//Create Geometry
	void createBuffers(void);

    //Overrides from MovableObject
    virtual const Ogre::String& getMovableType(void) const;

    //Overrides from Renderable
    virtual const  Ogre::LightList& getLights(void) const;
    virtual void getRenderOperation(  Ogre::v1::RenderOperation& op, bool casterPass );
    virtual void getWorldTransforms(  Ogre::Matrix4* xform ) const;
    virtual bool getCastsShadows(void) const;
    void setAmbient(bool);

    //Was parent light deleted
    bool isDead();



};

} /* namespace Ogre */

#endif /* LIGHT_DSLIGHT_H_ */
