/*
 * DSLight.h
 *
 *  Created on: 24.11.2015
 *      Author: marvin
 */

#ifndef LIGHT_DSLIGHT_H_
#define LIGHT_DSLIGHT_H_
#include "OgreSimpleRenderable.h"
#include "Math/Array/OgreObjectMemoryManager.h"
#include "OgreSceneManager.h"
#include "GeomUtils.h"
#include "OgreMovableObject.h"
#include "OgreRenderable.h"
#include "Ogre.h"
#include "OgreMovableObject.h"
#include "OgreRenderable.h"
#include "OgreIdString.h"
#include "OgreId.h"
#include "OgreSceneNode.h"
namespace Ogre {

class DSLight   : public Ogre::MovableObject, public Ogre::Renderable{
	friend class LightManager;

	bool ambient=false;


public:
    const Ogre::Light *parent;

    Ogre::SceneNode *parentSN;

    int ShadowMapId=-1;
    int ShadowCasterID=-1;
	DSLight(const Ogre::Light * parent,Ogre::IdType id,
    		Ogre::ObjectMemoryManager *objectMemoryManager,
    		Ogre::SceneManager *manager, Ogre::uint8 renderQueueId);
	virtual ~DSLight();

	void createBuffers(void);

    //Overrides from MovableObject
    virtual const Ogre::String& getMovableType(void) const;

    //Overrides from Renderable
    virtual const  Ogre::LightList& getLights(void) const;
    virtual void getRenderOperation(  Ogre::v1::RenderOperation& op, bool casterPass );
    virtual void getWorldTransforms(  Ogre::Matrix4* xform ) const;
    virtual bool getCastsShadows(void) const;
    void setAmbient(bool);
    bool isDead();


};

} /* namespace Ogre */

#endif /* LIGHT_DSLIGHT_H_ */
