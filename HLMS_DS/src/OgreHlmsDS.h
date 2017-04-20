/*
 * OgreDSHLMS.h
 *
 *  Created on: 24.11.2015
 *      Author: marvin
 */

#ifndef OGREDSHLMS_H_
#define OGREDSHLMS_H_
#include "OgreStableHeaders.h"
#include "OgreArchive.h"
#include "math.h"

#include "OgreHlmsBufferManager.h"
#include "OgreConstBufferPool.h"
#include "OgreHeaderPrefix.h"


#include "OgreHlmsDSPrerequisites.h"
#include "Datablocks/DSDatablock.h"

#include "Light/DSLightManager.h"

#include "Helper/HLMSDSWorkspaceListener.h"

#include "DBParser/JsonParser.h"
#define NUM_DEFAULT_GBUFFER 5;
namespace Ogre {
class ShadowManager;
class DSDatablock;
class CompositorShadowNode;
struct QueuedRenderable;
class DSLightManager;
class DSLightDatablock;
class _OgreHlmsDSExport HlmsDS: public HlmsBufferManager, public ConstBufferPool{
friend DSDatablock;

	std::vector<Ogre::DSDatablock* >* dsDatablocks;
public:
    struct PassData
    {
        FastArray<TexturePtr>   shadowMaps;
        vector<int>::type 		   shadowMapTUs;
        FastArray<float>    vertexShaderSharedBuffer;
        FastArray<float>    pixelShaderSharedBuffer;

        Matrix4 viewMatrix;
        Matrix4 projMatrix;



    };
    struct ShadowCam{
    	Ogre::Camera * cam;

    };
    //todo temporary solution
    FastArray<ShadowCam> shadowCams;


    //Default JSON Parser
    JsonParser * jsonDs;


    //Light is for Light Voumes, GBuffer is for deferred Materials and Forward is for Forward rendered Materials
    enum Datablock_Type { DT_Light, DT_GBuffer, DT_Forward };



    uint32 mLastTextureHash;

    PassData                mPreparedPass;
    ConstBufferPackedVec    mPassBuffers;
    HlmsSamplerblock const  *mShadowmapSamplerblock;    /// GL3+ only when not using depth textures
    HlmsSamplerblock const  *mShadowmapCmpSamplerblock; /// For depth textures & D3D11
    HlmsSamplerblock const  *mCurrentShadowmapSamplerblock;

    uint32                  mCurrentPassBuffer;     /// Resets every to zero every new frame.


    //is the PassBuffer corrupted?
    Ogre::Vector4 testdata= Vector4(0,1337,0.1234,5.4);

    //Which Compositor Pass are we curtrently in?
    int currentpassID=0;

    DSLightManager * lightmanager;
    ShadowManager * shadowmanager;

    static const int mNumGBuffers=NUM_DEFAULT_GBUFFER;
    std::vector<TexturePtr> *	mGBuffer;
    std::vector<int> *			mGBufferInd;


    //Determine the current pass ID
    HLMSDSWorkspaceListener * mWSListener;



   bool requiresTextureFlipping;

	HlmsDS( Archive *dataFolder, ArchiveVec *libraryFolders );
	~HlmsDS();

	virtual HlmsCache preparePassHash (const Ogre::CompositorShadowNode *shadowNode, bool casterPass, bool dualParaboloid, SceneManager *sceneManager);
    virtual void calculateHashForPreCreate( Renderable *renderable, PiecesMap *inOutPieces );
    virtual void calculateHashForPreCaster( Renderable *renderable, PiecesMap *inOutPieces );

    virtual void _changeRenderSystem( RenderSystem *newRs );


    virtual const HlmsCache* createShaderCacheEntry( uint32 renderableHash,
                                                     const HlmsCache &passCache,
                                                     uint32 finalHash,
                                                     const QueuedRenderable &queuedRenderable );

    virtual HlmsDatablock* createDatablockImpl( IdString datablockName,
                                                const HlmsMacroblock *macroblock,
                                                const HlmsBlendblock *blendblock,
                                                const HlmsParamVec &paramVec );
    virtual void destroyAllBuffers(void);


    virtual uint32 fillBuffersFor( const HlmsCache *cache, const QueuedRenderable &queuedRenderable,
                                   bool casterPass, uint32 lastCacheHash,
                                   uint32 lastTextureHash );
    FORCEINLINE uint32 fillBuffersFor( const HlmsCache *cache,
                                       const QueuedRenderable &queuedRenderable,
                                       bool casterPass, uint32 lastCacheHash,
                                       CommandBuffer *commandBuffer, bool isV1 );
    virtual uint32 fillBuffersForV1( const HlmsCache *cache,
                                     const QueuedRenderable &queuedRenderable,
                                     bool casterPass, uint32 lastCacheHash,
                                     CommandBuffer *commandBuffer );
    virtual uint32 fillBuffersForV2( const HlmsCache *cache,
                                     const QueuedRenderable &queuedRenderable,
                                     bool casterPass, uint32 lastCacheHash,
                                     CommandBuffer *commandBuffer );

    Ogre::String getShaderProfile();

    virtual void frameEnded(void);

    Matrix4 proj;
    Matrix4 view;
    Matrix4 model;
    Matrix4 curviewProjMatrix;

	Ogre::Timer * timer;


    ConstBufferPool::BufferPool const *mLastBoundPool;


    //Debug Variable in the Passbuffre
    enum DebugMode{
    	DM_OFF,
		DM_DEPTH,
		DM_NORMAL,
		DM_DIFFUSE,
		DM_GLOW,
		DM_SPECULAR,
		DM_LIGHT_GEOMETRY,
		DM_SHADOWMAPS,
    	DM_6SCREENS,
		DM_IDCOLOR,
		DM_NUM
    };
    DebugMode debugMode;

    void setDebugMode(DebugMode);

    enum FullBrightMode{
    	FBM_OFF,
		FBM_WHITE_DIFFUSE,
		FBM_NO_NORMAL,
		FBM_NO_GLOW,
		FBM_NO_DIFFUSE,
		FBM_NO_SPECULAR,
		FBM_NO_SWAP,
		FBM_NO_SHADOW,
		FBM_SPECULAR_ONLY,
		FBM_DIRECTIONAL_ONLY,
		FBM_NUM
    };
    FullBrightMode fullBrightMode;


    int Debugz=0;
    int Debugw=0;

    int CurLightMat=0;

    float incr=0;

    int NumPssmSplits;

    std::vector<Matrix4> LightCamMat;

    void setFullBright(FullBrightMode);
    int generateWorldMatrixBuffer(const HlmsCache* cache,
    		const QueuedRenderable& queuedRenderable, bool casterPass,
    		uint32 lastCacheHash, CommandBuffer* commandBuffer, bool isV1);

    void mapMatrixToBuffer(float *passBufferPtr,Matrix4 mat);
    Datablock_Type getDatablocktype(const HlmsDatablock *);
    const DSLightDatablock* CastDatablockLight(const HlmsDatablock * param);
    const DSDatablock* CastDatablockGBuffer(const HlmsDatablock * param);
    void uploadDirtyDSDatablocks();
    void setProperties(Renderable* ,
    		PiecesMap* , bool);

    void setPieces(Renderable* ,
    		PiecesMap* , bool);


	int getDebugw() const;
	void setDebugw(int debugw);
	int getDebugz() const;
	void setDebugz(int debugz);

	const HlmsSamplerblock* getSamplerBlock(HlmsSamplerblock );


#if !OGRE_NO_JSON
        /// @copydoc Hlms::_loadJson
        virtual void _loadJson( const rapidjson::Value &jsonValue, const HlmsJson::NamedBlocks &blocks,
                                HlmsDatablock *datablock ) const;
        /// @copydoc Hlms::_saveJson
        virtual void _saveJson( const HlmsDatablock *datablock, String &outString ) const;

        /// @copydoc Hlms::_collectSamplerblocks
        virtual void _collectSamplerblocks( set<const HlmsSamplerblock*>::type &outSamplerblocks,
                                            const HlmsDatablock *datablock ) const;
#endif


};
struct  _OgreHlmsDSExport DSProperty
{
    static const IdString MaterialsPerBuffer;

    static const IdString isLight;
    static const IdString isGBuffer;
    static const IdString isForward;

    static const IdString noTransf;

    static const IdString NumTextures;

    static const IdString DiffuseMap;

    static const IdString NormalMap;

    static const IdString SignedIntTex;

    static const Ogre::String Vec4Defines;
    static const Ogre::String Vec4ValDefines;

    static const Ogre::String TextureLoc;
    static const Ogre::String TextureValLoc;
    static const Ogre::String TextureDefines;

    static const Ogre::String TextureUVindex;

    static const String NumVec4Params;

    static const String NumTextureParams;

    static const String MaxVec4Params;

    static const String MaxTextureParams;

    static const String Shadow;


    static const Ogre::String MaterialVec4Params;
    static const Ogre::String MaterialTexParams;
    static const Ogre::String MaterialAutoparamParams;
    static const Ogre::String MaterialAutoTexParams;

    static const Ogre::String TextureHelper;

    static const Ogre::String NumSubTextures;

    static const Ogre::String NumShadowTex;

};

} /* namespace Ogre */

#endif /* OGREDSHLMS_H_ */

