
#ifndef _Demo_PbsMaterialsGameState_H_
#define _Demo_PbsMaterialsGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"
#include "Museum/Museum.h"
#include "Ogre.h"
#include "OgreVector4.h"
#include "Compositor/OgreCompositorWorkspaceDef.h"

#include "Compositor/OgreCompositorWorkspace.h"


namespace Ogre{
	class HlmsDS;
	class DSDatablock;

}
namespace Demo
{
    class PbsMaterialsGameState : public TutorialGameState
    {
        Ogre::SceneNode     *mSceneNode[16];

        Ogre::SceneNode     *mLightNodes[6];

        bool                mAnimateObjects;


        Ogre::Vector3 camPos;
        int cycleDM=0;
        int cycleFBM=0;
        int cycleX=0;
        int cycleY=0;

        int cycleMode=1;


        int blocksun=1;

        Museum::daystruct daycycle;

        Ogre::HlmsDS * hlmsDS;

        Ogre::HlmsDatablock * floord;


        Ogre::Light * mLight;

        size_t          mNumSpheres;
        Ogre::uint8     mTransparencyMode;
        float           mTransparencyValue;

        Ogre::DSDatablock * ghost_db;
        Ogre::DSDatablock * ghost_anim;

        Ogre::DSDatablock * pole_db;


        Ogre::SceneNode     *mSceneNodeGhost;

        Ogre::SceneNode     *mSceneNodePole;
        Ogre::SceneNode     *mSceneNodePole2;


        Ogre::SceneNode     *mSceneNodeAnim;

        Museum *museum;

        virtual void generateDebugText( float timeSinceLast, Ogre::String &outText );

        void setTransparencyToMaterials(void);

    public:
        PbsMaterialsGameState( const Ogre::String &helpDescription );

        virtual void createScene01(void);

        virtual void update( float timeSinceLast );

        virtual void keyReleased( const SDL_KeyboardEvent &arg );

        struct wColor
		{
        	Ogre::Vector3 color;
        	Ogre::Vector3 scolor;

        	float from;
        	float to;

        	float weight=0;


		};

        static Ogre::Real randf();
        static Ogre::Vector3 randv1();
        static Ogre::Vector3 randv2();
        static Ogre::Vector3 randv3();
        static Ogre::Vector3 RainbowPhase(float f);
        static bool between(float x,float a,float b){
        	if(x>a&&x<b) {
        		return true;
        	}
        	return false;
        }

        static float fbetween(float x,float a,float b){
    		if(a>b){
    			if(a>x){
    				x+=2.0;
    			}
    			b+=2.0;
    		}
        	if(x>a&&x<b) {


        		float ret=((x-a)/(b-a));



        		if(ret<0.5)
        			return ret*2.0;
        		else{
        			return (1.0-(ret))*2.0;
        		}
        	}

        	return 0;
        }

        float lerp(float a, float b, float f)
        {
            return a + f * (b - a);
        }

       static   Ogre::Vector3 between(std::vector< PbsMaterialsGameState::wColor *> colors  ,int size,float progress){

        	Ogre::Vector3 retval;
        	float totalWeight=0;
        	Ogre::Vector3 totalColor=Ogre::Vector3(0,0,0);

        	for(uint i=0;i<colors.size();i++){


        		colors[i]->weight=fbetween(progress, colors[i]->from,colors[i]->to);

        		totalColor+=colors[i]->weight*colors[i]->color;
        		totalWeight+=colors[i]->weight;
        	}
        	if(totalWeight!=0){
        		totalColor/=totalWeight;

        	}
        	return totalColor;

        }


    };
}

#endif
