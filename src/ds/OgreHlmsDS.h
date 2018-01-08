/*
 * OgreHlmsDS.h
 *
 *  Created on: Jan 4, 2018
 *      Author: marvin
 */

#ifndef SRC_DS_OGREHLMSDS_H_
#define SRC_DS_OGREHLMSDS_H_

#include "OgreHlmsDSPrerequisites.h"
#include "Datablock/DSDatablock.h"

#include <OgreConstBufferPool.h>
#include <OgreHlmsBufferManager.h>
#include "Datablock/DSDatablock.h"
#include "OgreHlmsBufferManager.h"
#include "OgreConstBufferPool.h"
#include "OgreMatrix4.h"
#include "OgreHeaderPrefix.h"
#include "OgreRoot.h"


namespace Ogre{


class _OgreHlmsDSExport HlmsDS: public Ogre::HlmsBufferManager, public Ogre::ConstBufferPool {
    struct PassData
    {
        Matrix4 viewProjMatrix[2];
    };

    PassData                mPreparedPass;
    ConstBufferPackedVec    mPassBuffers;
    uint32                  mCurrentPassBuffer;     /// Resets to zero every new frame.

    ConstBufferPool::BufferPool const *mLastBoundPool;

    uint32 mLastTextureHash;


public:


	HlmsDS( Archive *dataFolder, ArchiveVec *libraryFolders );

	virtual ~HlmsDS();

	virtual HlmsCache preparePassHash (const Ogre::CompositorShadowNode *shadowNode, bool casterPass, bool dualParaboloid, SceneManager *sceneManager);
    virtual void calculateHashForPreCreate( Renderable *renderable, PiecesMap *inOutPieces );
    virtual void calculateHashForPreCaster( Renderable *renderable, PiecesMap *inOutPieces );

    virtual void _changeRenderSystem( RenderSystem *newRs );

    void setProperties(Renderable* ,
    		PiecesMap* , bool);

    void setPieces(Renderable* ,
    		PiecesMap* , bool);

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


};
}
#endif /* SRC_DS_OGREHLMSDS_H_ */
