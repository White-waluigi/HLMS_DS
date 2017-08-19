
#include "GraphicsSystem.h"
#include "PbsMaterialsGameState.h"
#include "SdlInputHandler.h"

#include "OgreTimer.h"
#include "OgreSceneManager.h"
#include "OgreCamera.h"
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreConfigFile.h"
#include "Compositor/OgreCompositorManager2.h"

//Declares WinMain / main
#include "MainEntryPointHelper.h"

using namespace Demo;

namespace Demo
{
    class PbsMaterialsGraphicsSystem : public GraphicsSystem
    {
        virtual Ogre::CompositorWorkspace* setupCompositor()
        {
            Ogre::CompositorManager2 *compositorManager = mRoot->getCompositorManager2();
            return compositorManager->addWorkspace( mSceneManager, mRenderWindow, mCamera,
                                                    "DeferredShadingWorkspace", true );
        }

        virtual void setupResources(void)
        {
            GraphicsSystem::setupResources();

            Ogre::ConfigFile cf;
            cf.load(mResourcePath + "resources2.cfg");

            Ogre::String dataFolder = cf.getSetting( "DoNotUseAsResource", "Hlms", "" );

            if( dataFolder.empty() )
                dataFolder = "./";
            else if( *(dataFolder.end() - 1) != '/' )
                dataFolder += "/";

            dataFolder += "2.0/scripts/materials/PbsMaterials";

            addResourceLocation( dataFolder, "FileSystem", "General" );
        }

    public:
        PbsMaterialsGraphicsSystem( GameState *gameState ) :
            GraphicsSystem( gameState )
        {
        }
    };
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMainApp( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int mainApp()
#endif
{
    PbsMaterialsGameState pbsMaterialsGameState(
        "Shows how to use the PBS material system. There's nothing really fancy,\n"
        "it's just programmer art. The PBS materials can be setup from script or\n"
        "code. This sample does both. At the time being, not all settings from the\n"
        "PBS implementation can be tweaked with scripts. See PbsDatablock::PbsDatablock\n"
        "constructor documentation. Also see the Hlms section of the porting guide in\n"
        "the Docs/2.0 folder.\n"
        "\n"
        "The sphere palette shows what happens when tweaking the roughness around the\n"
        "X axis; and the fresnel term around the Z axis.\n"
        "The scene is being lit by a white directional light (3-split PSSM) and two spot\n"
        "lights, one of warm colour, one cold. Both are also shadowed."
        "\n"
        "Of all the features supported by the PBS implementation, perhaps the hardest to\n"
        "to understand is the Detail Weight Map. It allows you to 'paint' the detail maps\n"
        "over the mesh, by controlling weight of each of the 4 maps via the RGBA channels\n"
        "of the weight map. 'R' controls the detail map 0, 'G' the detail map 1,\n"
        "'B' the detail map 2, and 'A' the detail map 3.\n"
        "\n"
        "This sample depends on the media files:\n"
        "   * Samples/Media/2.0/scripts/Compositors/PbsMaterials.compositor\n"
        "   * Samples/Media/2.0/materials/PbsMaterials/PbsMaterials.material\n"
        "\n"
        "Known issues:\n"
        " * Non shadow casting point & spot lights require Forward3D to be enabled (on desktop).\n"
        "   This is by design (more implementations will come: Forward+ & Deferred; for now the\n"
        "   only one working is F3D).\n"
        " * Shadow casting point lights don't work or work poorly. (feature not implemented yet)\n"
        " * If PSSM shadow casting enabled, the system requires at least one shadow-casting\n"
        "   directional light (bug)\n"
        " * Mobile version only supports forward lighting.\n"
        "\n"
        "LEGAL: Uses Saint Peter's Basilica (C) by Emil Persson under CC Attrib 3.0 Unported\n"
        "See Samples/Media/materials/textures/Cubemaps/License.txt for more information.");
    PbsMaterialsGraphicsSystem graphicsSystem( &pbsMaterialsGameState );

    pbsMaterialsGameState._notifyGraphicsSystem( &graphicsSystem );

    graphicsSystem.initialize( "DS Materials Sample" );

    if( graphicsSystem.getQuit() )
    {
        graphicsSystem.deinitialize();
        return 0; //User cancelled config
    }

    Ogre::RenderWindow *renderWindow = graphicsSystem.getRenderWindow();

    graphicsSystem.createScene01();
    graphicsSystem.createScene02();

    //Do this after creating the scene for easier the debugging (the mouse doesn't hide itself)
    SdlInputHandler *inputHandler = graphicsSystem.getInputHandler();
    inputHandler->setGrabMousePointer( false );
    inputHandler->setMouseVisible( true );

    Ogre::Timer timer;
    unsigned long startTime = timer.getMicroseconds();

    double timeSinceLast = 1.0 / 60.0;

    while( !graphicsSystem.getQuit() )
    {
        graphicsSystem.beginFrameParallel();
        graphicsSystem.update( static_cast<float>( timeSinceLast ) );
        graphicsSystem.finishFrameParallel();
        graphicsSystem.finishFrame();

        if( !renderWindow->isVisible() )
        {
            //Don't burn CPU cycles unnecessary when we're minimized.
            Ogre::Threads::Sleep( 500 );
        }

        unsigned long endTime = timer.getMicroseconds();
        timeSinceLast = (endTime - startTime) / 1000000.0;
        timeSinceLast = std::min( 1.0, timeSinceLast ); //Prevent from going haywire.
        startTime = endTime;
    }

    graphicsSystem.destroyScene();
    graphicsSystem.deinitialize();

    return 0;
}
