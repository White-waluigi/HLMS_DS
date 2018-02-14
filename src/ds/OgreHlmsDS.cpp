/*
 * OgreHlmsDS.cpp
 *
 *  Created on: Jan 4, 2018
 *      Author: marvin
 */
#include <iostream>
#include <OgreHlmsDS.h>
#include <Compositor/OgreCompositorCommon.h>
#include "Compositor/OgreCompositorShadowNode.h"
#include <OgreSceneManager.h>
#include <OgreConstBufferPacked.h>
#include <OgreHlmsListener.h>
#include "CommandBuffer/OgreCommandBuffer.h"
#include "OgreStableHeaders.h"

#if !OGRE_NO_JSON
#include "OgreHlmsJsonUnlit.h"
#endif

#include "OgreViewport.h"
#include "OgreRenderTarget.h"
#include "OgreCamera.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreHighLevelGpuProgram.h"

#include "OgreSceneManager.h"
#include "Compositor/OgreCompositorShadowNode.h"
#include "Vao/OgreVaoManager.h"
#include "Vao/OgreConstBufferPacked.h"
#include "Vao/OgreTexBufferPacked.h"
#include "Vao/OgreStagingBuffer.h"

#include "OgreHlmsManager.h"
#include "OgreLogManager.h"

#include "CommandBuffer/OgreCommandBuffer.h"
#include "CommandBuffer/OgreCbTexture.h"
#include "CommandBuffer/OgreCbShaderBuffer.h"

#include "Modules/ModuleBroker.h"

#include "PassBuffer/PassBufferManager.h"

#include "Util/DSMath.h"

#include "Modules/Resources/Property.h"
#include "Modules/Resources/Piece.h"
#include "Modules/Resources/DSTexture.h"
#include "Util/ShaderGenerator.h"

#include "OgreHlmsSamplerblock.h"

namespace Ogre {

HlmsDS::HlmsDS(Archive* dataFolder, ArchiveVec* libraryFolders) :
		HlmsBufferManager(HLMS_PBS, "ds", dataFolder, libraryFolders), ConstBufferPool(
				16, ConstBufferPool::ExtraBufferParams()) {
	this->mBroker = new ModuleBroker();

	this->mPBMgr = new PassBufferManager(this);
	mPBMgr->initialize(NULL,0);

}

HlmsDS::~HlmsDS() {
	destroyAllBuffers();
}
void HlmsDS::uploadDirtyDSDatablocks() {

	int i = 0;

	for (std::list<Ogre::DSDatablock*>::iterator it = mDSDatablocks.begin();
			it != mDSDatablocks.end(); ++it) {

		(*it)->upload();

	}

}

//fill Passbuffer, execute pass related things (in this case create light volumes and assign Gbuffers and Shadow textures to the right Textures)
HlmsCache HlmsDS::preparePassHash(const Ogre::CompositorShadowNode* shadowNode,
		bool casterPass, bool dualParaboloid, SceneManager* sceneManager) {

	mSetProperties.clear();

	//Set the properties and create/retrieve the cache.
	if (casterPass) {
		setProperty(HlmsBaseProp::ShadowCaster, 1);

	}



	Camera *camera = sceneManager->getCameraInProgress();
	if (camera && camera->isReflected())
		setProperty(HlmsBaseProp::GlobalClipDistances, 1);

	mListener->preparePassHash( shadowNode, casterPass, dualParaboloid, sceneManager, this );

	PassCache passCache;
	passCache.passPso = getPassPsoForScene(sceneManager);
	passCache.properties = mSetProperties;

	assert(
			mPassCache.size() <= (size_t)HlmsBits::PassMask && "Too many passes combinations, we'll overflow the bits assigned in the hash!");
	PassCacheVec::iterator it = std::find(mPassCache.begin(), mPassCache.end(),
			passCache);
	if (it == mPassCache.end()) {
		mPassCache.push_back(passCache);
		it = mPassCache.end() - 1;
	}

	const uint32 hash = (it - mPassCache.begin()) << HlmsBits::PassShift;

	//Fill the buffers
	HlmsCache retVal(hash, mType, HlmsPso());
	retVal.setProperties = mSetProperties;
	retVal.pso.pass = passCache.passPso;


	//mSetProperties.clear();

	bool isShadowCastingPointLight = false;

	//mat4 viewProj[2] + vec4 invWindowSize;
	size_t mapSize = (16 + 16 + 4) * 4;

	const bool isCameraReflected = camera->isReflected();
	//mat4 invViewProj
	// if( isCameraReflected || (casterPass && (mUsingExponentialShadowMaps || isShadowCastingPointLight)) )
	//    mapSize += 16 * 4;

	if (casterPass) {
		isShadowCastingPointLight = getProperty(HlmsBaseProp::ShadowCasterPoint)
				!= 0;

		//vec4 viewZRow
		//if( mUsingExponentialShadowMaps )
		//   mapSize += 4 * 4;
		//vec4 depthRange
		mapSize += (2 + 2) * 4;
		//vec4 cameraPosWS
		if (isShadowCastingPointLight)
			mapSize += 4 * 4;
	}
	//vec4 clipPlane0
	if (isCameraReflected)
		mapSize += 4 * 4;

	mapSize += mListener->getPassBufferSize(shadowNode, casterPass,
			dualParaboloid, sceneManager);

	//Arbitrary 2kb (minimum supported by GL is 64kb), should be enough.
	const size_t maxBufferSize = 2 * 1024;
	assert(mapSize <= maxBufferSize);


	mPBMgr->update(mCurrentPassBuffer++);



	//mTexBuffers must hold at least one buffer to prevent out of bound exceptions.
	if (mTexBuffers.empty()) {
		size_t bufferSize = std::min<size_t>(mTextureBufferDefaultSize,
				mVaoManager->getTexBufferMaxSize());
		TexBufferPacked *newBuffer = mVaoManager->createTexBuffer(
				PF_FLOAT32_RGBA, bufferSize, BT_DYNAMIC_PERSISTENT, 0, false);
		mTexBuffers.push_back(newBuffer);
	}

	mLastTextureHash = 0;
	mLastBoundPool = 0;
	uploadDirtyDSDatablocks();
	uploadDirtyDatablocks();

	return retVal;
}

//set Material related properties
void HlmsDS::setProperties(Renderable* renderable, PiecesMap* inOutPieces,
		DSDatablock * datablock, bool ishadow) {
	DSResource::propertyList l = datablock->genProperties();
	for (DSResource::propertyList::iterator it = l.begin(); it != l.end();
			++it) {
		setProperty(it->first, it->second);
	}
}
//set Material related shader pieces
void HlmsDS::setPieces(Renderable* renderable, PiecesMap* inOutPieces,
		DSDatablock * datablock, bool isshadow) {
	std::map<IdString, String> l = datablock->genPieces();
	for (std::map<IdString, String>::iterator it = l.begin(); it != l.end();
			++it) {
		inOutPieces[PixelShader][it->first] = it->second;
		inOutPieces[VertexShader][it->first] = it->second;
	}

}

//set Texture bindings
void HlmsDS::calculateHashForPreCreate(Renderable* renderable,
		PiecesMap* inOutPieces) {

	assert(dynamic_cast<DSDatablock*>( renderable->getDatablock() ));
	DSDatablock *datablock =
			static_cast<DSDatablock*>(renderable->getDatablock());

	setProperty(HlmsBaseProp::Skeleton, 0);
	setProperty(HlmsBaseProp::Normal, 0);
	setProperty(HlmsBaseProp::QTangent, 0);
	setProperty(HlmsBaseProp::Tangent, 0);
	setProperty(HlmsBaseProp::BonesPerVertex, 0);

	int texUnit = 2; //Vertex shader consumes 2 slots with its two tbuffers.
	int numTextures = 0;
	int numArrayTextures = 0;

	bool hasAnimationMatrices = false;
	bool hasPlanarReflection = false;

	setProperties(renderable, inOutPieces, datablock, false);
	setPieces(renderable, inOutPieces, datablock, false);
}

//Set shadowmap texture units
void Ogre::HlmsDS::calculateHashForPreCaster(Renderable* renderable,
		PiecesMap* inOutPieces) {
	DSDatablock *datablock =
			static_cast<DSDatablock*>(renderable->getDatablock());
	//HlmsUnlitDatablock *datablock = static_cast<HlmsUnlitDatablock*>(
	//                                              renderable->getDatablock() );

	HlmsPropertyVec::iterator itor = mSetProperties.begin();
	HlmsPropertyVec::iterator end = mSetProperties.end();

	while (itor != end) {
		if ( /*itor->keyName != UnlitProperty::HwGammaRead &&*/
		//itor->keyName != UnlitProperty::UvDiffuse &&
		itor->keyName != HlmsBaseProp::Skeleton
				&& itor->keyName != HlmsBaseProp::BonesPerVertex
				&& itor->keyName != HlmsBaseProp::DualParaboloidMapping
				&& itor->keyName != HlmsBaseProp::AlphaTest
				&& itor->keyName != HlmsBaseProp::AlphaBlend) {
			itor = mSetProperties.erase(itor);
			end = mSetProperties.end();
		} else {
			++itor;
		}
	}
	setProperties(renderable, inOutPieces, datablock, true);
	setPieces(renderable, inOutPieces, datablock, true);
}
void HlmsDS::createTextureCache(uint32 renderableHash,
		const HlmsCache &passCache, uint32 finalHash,
		const QueuedRenderable &queuedRenderable, const HlmsCache* retVal) {

	int texUnit = 2;
	DSDatablock *datablock =
			static_cast<DSDatablock*>(queuedRenderable.renderable->getDatablock());

	GpuProgramParametersSharedPtr psParams =
			retVal->pso.pixelShader->getDefaultParameters();

	int numArrayTextures = 0;

	if (!retVal->pso.pixelShader.isNull()) {


		GpuProgramParametersSharedPtr psParams =
				retVal->pso.pixelShader->getDefaultParameters();

		DSResource::TextureRef::textureBuffList l = datablock->genTextureBufs();
		for (DSResource::TextureRef::textureBuffList::iterator it=l.begin(); it != l.end(); ++it) {

			DSResource::TextureRef * lol=(*it);
			if (""==lol->CustomName) {

				psParams->setNamedConstant(
						"textureMapsArray["
								+ StringConverter::toString(numArrayTextures++)
								+ "]", texUnit++);

				//setProperty((ShaderKeyWordBank::NUMTEXTURES),numArrayTextures);

			} else {

				psParams->setNamedConstant((*it)->CustomName, texUnit++);

			}
		}

	}
}
//clean up
void HlmsDS::destroyAllBuffers(void) {
	HlmsBufferManager::destroyAllBuffers();

	mCurrentPassBuffer = 0;

	{
		ConstBufferPackedVec::const_iterator itor = mPassBuffers.begin();
		ConstBufferPackedVec::const_iterator end = mPassBuffers.end();

		while (itor != end) {
			if ((*itor)->getMappingState() != MS_UNMAPPED)
				(*itor)->unmap(UO_UNMAP_ALL);
			mVaoManager->destroyConstBuffer(*itor);
			++itor;
		}

		mPassBuffers.clear();

		//todo destroy passbuffermgr

	}
}

//Update material buffers
inline uint32 HlmsDS::fillBuffersFor(const HlmsCache* cache,
		const QueuedRenderable& queuedRenderable, bool casterPass,
		uint32 lastCacheHash, CommandBuffer* commandBuffer, bool isV1) {

	assert(
			dynamic_cast<const HlmsUnlitDatablock*>( queuedRenderable.renderable->getDatablock() ));
	DSDatablock *datablock =
			static_cast<DSDatablock*>(queuedRenderable.renderable->getDatablock());

	if ( OGRE_EXTRACT_HLMS_TYPE_FROM_CACHE_HASH( lastCacheHash ) != mType) {
		//We changed HlmsType, rebind the shared textures.
		mLastTextureHash = 0;
		mLastBoundPool = 0;

		//layout(binding = 0) uniform PassBuffer {} pass
		ConstBufferPacked *passBuffer = mPBMgr->get(mCurrentPassBuffer - 1);
		*commandBuffer->addCommand<CbShaderBuffer>() = CbShaderBuffer(
				VertexShader, 0, passBuffer, 0,
				passBuffer->getTotalSizeBytes());
		*commandBuffer->addCommand<CbShaderBuffer>() = CbShaderBuffer(
				PixelShader, 0, passBuffer, 0, passBuffer->getTotalSizeBytes());

		//layout(binding = 2) uniform InstanceBuffer {} instance
		if (mCurrentConstBuffer < mConstBuffers.size()
				&& (size_t) ((mCurrentMappedConstBuffer
						- mStartMappedConstBuffer) + 4)
						<= mCurrentConstBufferSize) {
			*commandBuffer->addCommand<CbShaderBuffer>() = CbShaderBuffer(
					VertexShader, 2, mConstBuffers[mCurrentConstBuffer], 0, 0);
			*commandBuffer->addCommand<CbShaderBuffer>() = CbShaderBuffer(
					PixelShader, 2, mConstBuffers[mCurrentConstBuffer], 0, 0);
		}

		rebindTexBuffer(commandBuffer);

		mListener->hlmsTypeChanged(casterPass, commandBuffer, datablock);
	}

	//Don't bind the material buffer on caster passes (important to keep
	//MDI & auto-instancing running on shadow map passes)

	//layout(binding = 1) uniform MaterialBuf {} materialArray

	ConstBufferPacked * materialbuffer = datablock->getMaterialBuffer();
	*commandBuffer->addCommand<CbShaderBuffer>() = CbShaderBuffer(PixelShader,
			MaterialBuffer, materialbuffer, 0,
			materialbuffer->getTotalSizeBytes());
	*commandBuffer->addCommand<CbShaderBuffer>() = CbShaderBuffer(VertexShader,
			MaterialBuffer, materialbuffer, 0,
			materialbuffer->getTotalSizeBytes());

	uint32 * RESTRICT_ALIAS currentMappedConstBuffer = mCurrentMappedConstBuffer;
	float * RESTRICT_ALIAS currentMappedTexBuffer = mCurrentMappedTexBuffer;

	const Matrix4 &worldMat =
			queuedRenderable.movableObject->_getParentNodeFullTransform();

	bool exceedsConstBuffer = (size_t) ((currentMappedConstBuffer
			- mStartMappedConstBuffer) + 4) > mCurrentConstBufferSize;

	const size_t minimumTexBufferSize = 16;
	bool exceedsTexBuffer = (currentMappedTexBuffer - mStartMappedTexBuffer)
			+ minimumTexBufferSize >= mCurrentTexBufferSize;

	if (exceedsConstBuffer || exceedsTexBuffer) {
		currentMappedConstBuffer = mapNextConstBuffer(commandBuffer);

		if (exceedsTexBuffer)
			mapNextTexBuffer(commandBuffer,
					minimumTexBufferSize * sizeof(float));
		else
			rebindTexBuffer(commandBuffer, true,
					minimumTexBufferSize * sizeof(float));

		currentMappedTexBuffer = mCurrentMappedTexBuffer;
	}

	//---------------------------------------------------------------------------
	//                          ---- VERTEX SHADER ----
	//---------------------------------------------------------------------------
	bool useIdentityProjection =
			queuedRenderable.renderable->getUseIdentityProjection();

	//uint materialIdx[]
	//*currentMappedConstBuffer = datablock->getAssignedSlot();
	*reinterpret_cast<float * RESTRICT_ALIAS>(currentMappedConstBuffer + 1) =
			datablock->mShadowConstantBias;
	*(currentMappedConstBuffer + 2) = useIdentityProjection;
	currentMappedConstBuffer += 4;

	//mat4 worldViewProj
	Matrix4 tmp = mPreparedPass.viewProjMatrix[useIdentityProjection]
			* worldMat;
#if !OGRE_DOUBLE_PRECISION
	memcpy(currentMappedTexBuffer, &tmp, sizeof(Matrix4));
	currentMappedTexBuffer += 16;
#else
	for( int y = 0; y < 4; ++y )
	{
		for( int x = 0; x < 4; ++x )
		{
			*currentMappedTexBuffer++ = tmp[ y ][ x ];
		}
	}
#endif

	//---------------------------------------------------------------------------
	//                          ---- PIXEL SHADER ----
	//---------------------------------------------------------------------------

//		if (datablock->mTextureHash != mLastTextureHash) {
			//Rebind textures
			size_t texUnit = 2;

			//UnlitBakedTextureArray::const_iterator itor = datablock->mBakedTextures.begin();
			// UnlitBakedTextureArray::const_iterator end  = datablock->mBakedTextures.end();

//	            while( itor != end )
//	            {
//	                *commandBuffer->addCommand<CbTexture>() =
//	                        CbTexture( texUnit++, true, itor->texture.get(), itor->samplerBlock );
//	                ++itor;
//	            }
			DSResource::TextureRef::textureBuffList l =
					datablock->genTextureBufs();

			for (DSResource::TextureRef::textureBuffList::iterator it =
					l.begin(); it != l.end(); it++) {

					HlmsSamplerblock sb=HlmsSamplerblock((*it)->samplerBlock );


					*commandBuffer->addCommand<CbTexture>() = CbTexture(
							texUnit++, true, (*it)->texloc.texture.get(),
							&sb );



			}
			*commandBuffer->addCommand<CbTextureDisableFrom>() =
					CbTextureDisableFrom(texUnit);
//		}


		mLastTextureHash = datablock->mTextureHash;
		mCurrentMappedConstBuffer = currentMappedConstBuffer;
		mCurrentMappedTexBuffer = currentMappedTexBuffer;

		return ((mCurrentMappedConstBuffer - mStartMappedConstBuffer) >> 2) - 1;

	}

	uint32 HlmsDS::fillBuffersForV1(const HlmsCache* cache,
			const QueuedRenderable& queuedRenderable, bool casterPass,
			uint32 lastCacheHash, CommandBuffer* commandBuffer) {
		//std::cout << "*****************************************\n"
		//		<< "fillBuffersForV1" << "\n";
		return fillBuffersFor(cache, queuedRenderable, casterPass, lastCacheHash,
				commandBuffer, true);
	}
	uint32	HlmsDS::fillBuffersForV2(const HlmsCache* cache,
			const QueuedRenderable& queuedRenderable, bool casterPass,
			uint32 lastCacheHash, CommandBuffer* commandBuffer) {
		//std::cout << "*****************************************\n"
		//		<< "fillBuffersForV2" << "\n";
		return fillBuffersFor(cache, queuedRenderable, casterPass, lastCacheHash,
				commandBuffer, false);

	}
	void HlmsDS::frameEnded(void) {
		//std::cout << "frameEnded" << "\n";
		HlmsBufferManager::frameEnded();
		mCurrentPassBuffer = 0;

	}
	const HlmsCache* HlmsDS::createShaderCacheEntry(uint32 renderableHash,
			const HlmsCache& passCache, uint32 finalHash,
			const QueuedRenderable& queuedRenderable) {

		//std::cout << "#######" << "createShaderCacheEntry" << "\n";

		const HlmsCache *retVal = Hlms::createShaderCacheEntry(renderableHash,
				passCache, finalHash, queuedRenderable);

		if (mShaderProfile == "hlsl" || mShaderProfile == "metal") {
			mListener->shaderCacheEntryCreated(mShaderProfile, retVal, passCache,
					mSetProperties, queuedRenderable);
			return retVal; //D3D embeds the texture slots in the shader.
		}

		//Set samplers.
		assert(
				dynamic_cast<const DSDatablock*>( queuedRenderable.renderable->getDatablock() ));
		const DSDatablock *datablock =
		static_cast<const DSDatablock*>(queuedRenderable.renderable->getDatablock());

		createTextureCache(renderableHash,
				passCache, finalHash,
				queuedRenderable,retVal);



		GpuProgramParametersSharedPtr vsParams =
		retVal->pso.vertexShader->getDefaultParameters();
		//vsParams->setNamedConstant("worldMatBuf", 0);
		//if( getProperty( UnlitProperty::TextureMatrix ) )
		//vsParams->setNamedConstant("animationMatrixBuf", 1);

		mListener->shaderCacheEntryCreated(mShaderProfile, retVal, passCache,
				mSetProperties, queuedRenderable);

		mRenderSystem->_setPipelineStateObject(&retVal->pso);

		mRenderSystem->bindGpuProgramParameters(GPT_VERTEX_PROGRAM, vsParams,
				GPV_ALL);
		if (!retVal->pso.pixelShader.isNull()) {
			GpuProgramParametersSharedPtr psParams =
			retVal->pso.pixelShader->getDefaultParameters();
			mRenderSystem->bindGpuProgramParameters(GPT_FRAGMENT_PROGRAM, psParams,
					GPV_ALL);
		}

		if (!mRenderSystem->getCapabilities()->hasCapability(
						RSC_CONST_BUFFER_SLOTS_IN_SHADER)) {
			//Setting it to the vertex shader will set it to the PSO actually.
			retVal->pso.vertexShader->setUniformBlockBinding("PassBuffer", 0);
			retVal->pso.vertexShader->setUniformBlockBinding("MaterialBuf", 1);
			retVal->pso.vertexShader->setUniformBlockBinding("InstanceBuffer", 2);
		}

		return retVal;

	}
	HlmsDatablock* HlmsDS::createDatablockImpl(IdString datablockName,
			const HlmsMacroblock* macroblock, const HlmsBlendblock* blendblock,
			const HlmsParamVec& paramVec) {

		DSDatablock*db = new Ogre::DSDatablock(datablockName, this, macroblock,
				blendblock, paramVec);

		//todo remove

		static int ctr=0;
		ctr++;

		reflist l;
		l["datablock"]=db;
		l["hlms"]=this;
		l["mandatory"]=new DSResource::MandatoryInit("testdata");
		Value * test=dynamic_cast<Value *>( mBroker->loadModule("Value",l));
		l["mandatory"]=new DSResource::MandatoryInit("testpiece");
		Piece * test2=dynamic_cast<Piece *>( mBroker->loadModule("Piece",l));
		l["mandatory"]=new DSResource::MandatoryInit("testprop");
		Property * test3=dynamic_cast<Property *>( mBroker->loadModule("Property",l));
		l["mandatory"]=new DSResource::MandatoryInit("testtex");




		int texctr=rand()%628;

		l["texinit"]=new DSTexture::TexInitData(StringConverter::toString(texctr+2) +".png", HlmsTextureManager::TEXTURE_TYPE_DIFFUSE,HlmsSamplerblock());
		DSTexture * test4 =dynamic_cast<DSTexture *>( mBroker->loadModule("DSTexture",l));

		l["mandatory"]=new DSResource::MandatoryInit("testtex2");
		texctr=rand()%628;
		l["texinit"]=new DSTexture::TexInitData(StringConverter::toString(texctr+2) +".png", HlmsTextureManager::TEXTURE_TYPE_DIFFUSE,HlmsSamplerblock());
		DSTexture * test5 =dynamic_cast<DSTexture *>( mBroker->loadModule("DSTexture",l));


		static float rc=0;

		rc+=1.0/13.0;
		float * data=DSMath::quickArray(DSMath::rainbow(rc));

		test->set(data);
		test2->setCode(ShaderGenerator::genTestFunction("",Vector4(0.3,0.8,0.2,0),"test"));
		test3->setValue(80085);

		db->addResource(test);
		db->addResource(test2);
		db->addResource(test3);

		db->addResource(test4);
		db->addResource(test5);

		db->initalize(NULL);

		mDSDatablocks.push_back(db);

		return db;

	}
	void HlmsDS::_changeRenderSystem(RenderSystem* newRs) {
		//std::cout << "*****************************************\n"
		//		<< "_changeRenderSystem" << "\n";
		if (this->mVaoManager)
		destroyAllBuffers();

		ConstBufferPool::_changeRenderSystem(newRs);
		HlmsBufferManager::_changeRenderSystem(newRs);

		if (newRs) {
			HlmsDatablockMap::const_iterator itor = mDatablocks.begin();
			HlmsDatablockMap::const_iterator end = mDatablocks.end();

			while (itor != end) {
				assert(dynamic_cast<HlmsUnlitDatablock*>( itor->second.datablock ));
				DSDatablock *datablock =
				static_cast<DSDatablock*>(itor->second.datablock);
//            requestSlot( datablock->mNumEnabledAnimationMatrices != 0, datablock,
//                         datablock->mNumEnabledAnimationMatrices != 0 );
				++itor;
			}
		}

	}

	uint32 HlmsDS::fillBuffersFor(const HlmsCache* cache,
			const QueuedRenderable& queuedRenderable, bool casterPass,
			uint32 lastCacheHash, uint32 lastTextureHash) {
		return 0;
	}

	ModuleBroker * HlmsDS::getModuleBroker()
	{
		return this->mBroker;
	}
	Ogre::String
	HlmsDS::getShaderProfile()
	{
		return mShaderProfile;
	}

}

