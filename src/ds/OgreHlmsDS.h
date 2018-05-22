/*
 * OgreHlmsDS.h
 *
 *  Created on: Jan 4, 2018
 *      Author: marvin
 *
 *  Main file for the HLMS_DS Plugin
 *  its Tasks consist of
 *
 *  - implementing HLMS Interface
 *  - loading Datablocks
 *  - redirect Renderevents to the responsible
 *    Managers
 *
 *  This File is mostly meant to be an empty hask that
 *  outsources the heavy lifting to other classes, as not
 *  to bloat the HLMS
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
#include "Modules/ModuleReference.h"

namespace Ogre {

class DSDatablock;
class ModuleBroker;
class PassBufferManager;
class LightManager;
class ConstBufferPackedVec;
class PassBufferDefaultVal;
class HlmsListener;


struct PassData {
	Matrix4 viewProjMatrix[2];


};


class _OgreHlmsDSExport HlmsDS: public Ogre::HlmsBufferManager,
		public Ogre::ConstBufferPool,
		public ModuleReference {

	friend DSDatablock;
	friend PassBufferManager;
	friend PassBufferDefaultVal;


	std::list<Ogre::DSDatablock*> mDSDatablocks;
	float rbctr=0;
	ModuleBroker * mBroker;

	//Managers
	PassBufferManager * mPBMgr;
	LightManager * mLMgr;
	InstanceManager* mIMgr;

	//Multilistner
	typedef std::list<HlmsListener *> ListenerList;
	ListenerList   mListeners;

	PassData mPreparedPass;
	ConstBufferPackedVec mPassBuffers;
	uint32 mCurrentPassBuffer=0;     /// Resets to zero every new frame.

	ConstBufferPool::BufferPool const *mLastBoundPool;

	uint32 mLastTextureHash;

	bool firstPass=true;

	enum BufferSlots {
		PassBuffer, MaterialBuffer, InstanceBuffer
	};

public:

	void uploadDirtyDSDatablocks();

	HlmsDS(Archive *dataFolder, ArchiveVec *libraryFolders);

	virtual ~HlmsDS();

	virtual HlmsCache preparePassHash(
			const Ogre::CompositorShadowNode *shadowNode, bool casterPass,
			bool dualParaboloid, SceneManager *sceneManager);
	virtual void calculateHashForPreCreate(Renderable *renderable,
			PiecesMap *inOutPieces);
	virtual void calculateHashForPreCaster(Renderable *renderable,
			PiecesMap *inOutPieces);

	virtual void _changeRenderSystem(RenderSystem *newRs);

	void setProperties(Renderable*, PiecesMap*, DSDatablock*, bool);

	void setPieces(Renderable*, PiecesMap*, DSDatablock*, bool);

	virtual const HlmsCache* createShaderCacheEntry(uint32 renderableHash,
			const HlmsCache &passCache, uint32 finalHash,
			const QueuedRenderable &queuedRenderable);

	virtual HlmsDatablock* createDatablockImpl(IdString datablockName,
			const HlmsMacroblock *macroblock, const HlmsBlendblock *blendblock,
			const HlmsParamVec &paramVec);

	virtual void createTextureCache(uint32 renderableHash,
			const HlmsCache &passCache, uint32 finalHash,
			const QueuedRenderable &queuedRenderable,const HlmsCache * cache);

	virtual void destroyAllBuffers(void);

	virtual uint32 fillBuffersFor(const HlmsCache *cache,
			const QueuedRenderable &queuedRenderable, bool casterPass,
			uint32 lastCacheHash, uint32 lastTextureHash);FORCEINLINE uint32 fillBuffersFor(
			const HlmsCache *cache, const QueuedRenderable &queuedRenderable,
			bool casterPass, uint32 lastCacheHash, CommandBuffer *commandBuffer,
			bool isV1);
	virtual uint32 fillBuffersForV1(const HlmsCache *cache,
			const QueuedRenderable &queuedRenderable, bool casterPass,
			uint32 lastCacheHash, CommandBuffer *commandBuffer);
	virtual uint32 fillBuffersForV2(const HlmsCache *cache,
			const QueuedRenderable &queuedRenderable, bool casterPass,
			uint32 lastCacheHash, CommandBuffer *commandBuffer);

	Ogre::String getShaderProfile();

	virtual void frameEnded(void);

	void addListener(HlmsListener *);


	const PassData  getPreparedPass () const;
	void setPreparedPass(PassData preparedPass);

	ModuleBroker* getModuleBroker();
};
}
#endif /* SRC_DS_OGREHLMSDS_H_ */
