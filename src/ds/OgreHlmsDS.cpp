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

namespace Ogre {

HlmsDS::HlmsDS(Archive* dataFolder, ArchiveVec* libraryFolders) :
		HlmsBufferManager(HLMS_PBS, "ds", dataFolder, libraryFolders), ConstBufferPool(
				16, ConstBufferPool::ExtraBufferParams()) {
	// TODO Auto-generated constructor stub

}

HlmsDS::~HlmsDS() {
	destroyAllBuffers();
}
Vector4 rainbow(float phase,bool cap=false)
{
	if(cap&&(phase<0||phase>1)){
		return Vector4(0,0,0,0);
	}
	float p=fmod(phase,1.0);
	float c=p*3.0;
	float level = c;

	float x=fmod(c,1.0);
	float y=1-fmod(c,1.0);

	float r,g,b;
	if(level<1){
		r=y;
		g=x;
		b=0;
	}else if(level<2){
		r=0;
		g=y;
		b=x;
	}else if(level<3){
		r=x;
		g=0;
		b=y;
	}

	return Vector4(r,g,b,0);
}

//fill Passbuffer, execute pass related things (in this case create light volumes and assign Gbuffers and Shadow textures to the right Textures)
HlmsCache HlmsDS::preparePassHash(const Ogre::CompositorShadowNode* shadowNode,
		bool casterPass, bool dualParaboloid, SceneManager* sceneManager) {
	mSetProperties.clear();

	//Set the properties and create/retrieve the cache.
	if (casterPass) {
		setProperty(HlmsBaseProp::ShadowCaster, 1);

		const CompositorPass *pass = sceneManager->getCurrentCompositorPass();
		if (pass) {
			const uint8 shadowMapIdx = pass->getDefinition()->mShadowMapIdx;
			const Light *light = shadowNode->getLightAssociatedWith(
					shadowMapIdx);
			if (light->getType() == Light::LT_DIRECTIONAL)
				setProperty(HlmsBaseProp::ShadowCasterDirectional, 1);
			else if (light->getType() == Light::LT_POINT)
				setProperty(HlmsBaseProp::ShadowCasterPoint, 1);
		}
	}

	RenderTarget *renderTarget =
			sceneManager->getCurrentViewport()->getTarget();
	setProperty(HlmsBaseProp::ShadowUsesDepthTexture,
			renderTarget->getForceDisableColourWrites() ? 1 : 0);
	setProperty(HlmsBaseProp::RenderDepthOnly,
			renderTarget->getForceDisableColourWrites() ? 1 : 0);

	Camera *camera = sceneManager->getCameraInProgress();
	if (camera && camera->isReflected())
		setProperty(HlmsBaseProp::GlobalClipDistances, 1);

	//mListener->preparePassHash( shadowNode, casterPass, dualParaboloid, sceneManager, this );

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

	Matrix4 viewMatrix = camera->getViewMatrix(true);

	Matrix4 projectionMatrix = camera->getProjectionMatrixWithRSDepth();
	Matrix4 identityProjMat;
	mRenderSystem->_convertProjectionMatrix(Matrix4::IDENTITY, identityProjMat,
			true);

	if (renderTarget->requiresTextureFlipping()) {
		projectionMatrix[1][0] = -projectionMatrix[1][0];
		projectionMatrix[1][1] = -projectionMatrix[1][1];
		projectionMatrix[1][2] = -projectionMatrix[1][2];
		projectionMatrix[1][3] = -projectionMatrix[1][3];

		identityProjMat[1][0] = -identityProjMat[1][0];
		identityProjMat[1][1] = -identityProjMat[1][1];
		identityProjMat[1][2] = -identityProjMat[1][2];
		identityProjMat[1][3] = -identityProjMat[1][3];
	}
	//Matrix4( row0{1.50888 0 0 0 } row1{-0 -2.41421 -0 -0 } row2{0 0 -1.00004 -0.400008 } row3{0 0 -1 0 })nope

	//Matrix4( row0{-0.52448 4.14252e-06 0.851443 41.2936 } row1{0.43897 0.856853 0.270391 -58.4228 } row2{-0.729562 0.515564 -0.449407 -495.751 } row3{0 0 0 1 })nope


	mPreparedPass.viewProjMatrix[0] = projectionMatrix * viewMatrix;
	mPreparedPass.viewProjMatrix[1] = identityProjMat;

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

	mapSize += mListener->getPassBufferSize( shadowNode, casterPass,
	                                         dualParaboloid, sceneManager );

	//Arbitrary 2kb (minimum supported by GL is 64kb), should be enough.
	const size_t maxBufferSize = 2 * 1024;
	assert(mapSize <= maxBufferSize);

	if (mCurrentPassBuffer >= mPassBuffers.size()) {
		mPassBuffers.push_back(
				mVaoManager->createConstBuffer(maxBufferSize,
						BT_DYNAMIC_PERSISTENT, 0, false));
	}

	ConstBufferPacked *passBuffer = mPassBuffers[mCurrentPassBuffer++];
	float *passBufferPtr = reinterpret_cast<float*>(passBuffer->map(0, mapSize));

#ifndef NDEBUG
	const float *startupPtr = passBufferPtr;
#endif
	//---------------------------------------------------------------------------
	//                          ---- VERTEX SHADER ----
	//---------------------------------------------------------------------------

	//mat4 viewProj[0];
	static Ogre::Timer t;


	Vector4 test=rainbow(t.getMilliseconds()/3000.0);

	for (size_t i = 0; i < 4; ++i)
		*passBufferPtr++ = test[i];



	for (size_t i = 0; i < 16; ++i)
		*passBufferPtr++ = (float) mPreparedPass.viewProjMatrix[0][0][i];

	//mat4 viewProj[1] (identityProj);
	for (size_t i = 0; i < 16; ++i)
		*passBufferPtr++ = (float) mPreparedPass.viewProjMatrix[1][0][i];

	//vec4 clipPlane0
	if (isCameraReflected) {
		const Plane &reflPlane = camera->getReflectionPlane();
		*passBufferPtr++ = (float) reflPlane.normal.x;
		*passBufferPtr++ = (float) reflPlane.normal.y;
		*passBufferPtr++ = (float) reflPlane.normal.z;
		*passBufferPtr++ = (float) reflPlane.d;
	}

	//if( isCameraReflected || (casterPass && (mUsingExponentialShadowMaps || isShadowCastingPointLight)) )
	//{
	//    //We don't care about the inverse of the identity proj because that's not
	//    //really compatible with shadows anyway.
	//    Matrix4 invViewProj = mPreparedPass.viewProjMatrix[0].inverse();
	//    for( size_t i=0; i<16; ++i )
	//        *passBufferPtr++ = (float)invViewProj[0][i];
	//}

	if (casterPass) {
		//vec4 viewZRow
		if ( /*mUsingExponentialShadowMaps*/true) {
			*passBufferPtr++ = viewMatrix[2][0];
			*passBufferPtr++ = viewMatrix[2][1];
			*passBufferPtr++ = viewMatrix[2][2];
			*passBufferPtr++ = viewMatrix[2][3];
		}

		//vec2 depthRange;
		Real fNear, fFar;
		shadowNode->getMinMaxDepthRange(camera, fNear, fFar);
		const Real depthRange = fFar - fNear;
		*passBufferPtr++ = fNear;
		*passBufferPtr++ = 1.0f / depthRange;
		passBufferPtr += 2;

		//vec4 cameraPosWS;
		if (isShadowCastingPointLight) {
			const Vector3 &camPos = camera->getDerivedPosition();
			*passBufferPtr++ = (float) camPos.x;
			*passBufferPtr++ = (float) camPos.y;
			*passBufferPtr++ = (float) camPos.z;
			*passBufferPtr++ = 1.0f;
		}
	}

	//vec4 invWindowSize;
	*passBufferPtr++ = 1.0f / (float) renderTarget->getWidth();
	*passBufferPtr++ = 1.0f / (float) renderTarget->getHeight();
	*passBufferPtr++ = 1.0f;
	*passBufferPtr++ = 1.0f;

	passBufferPtr = mListener->preparePassBuffer(shadowNode, casterPass,
			dualParaboloid, sceneManager, passBufferPtr);

	assert((size_t)(passBufferPtr - startupPtr) * 4u == mapSize);

	passBuffer->unmap(UO_KEEP_PERSISTENT);

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

	uploadDirtyDatablocks();

	return retVal;
}


//set Material related properties
void HlmsDS::setProperties(Renderable* renderable, PiecesMap* inOutPieces,
		bool ishadow) {


}
//set Material related shader pieces
void HlmsDS::setPieces(Renderable* renderable, PiecesMap* inOutPieces,
		bool bool1) {

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


}

//Set shadowmap texture units
void Ogre::HlmsDS::calculateHashForPreCaster(Renderable* renderable,
		PiecesMap* inOutPieces) {
	std::cout << "*****************************************\n"
			<< "fillBuffersFor" << "\n";
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
	}
}


//Update material buffers
inline uint32 HlmsDS::fillBuffersFor(const HlmsCache* cache,
		const QueuedRenderable& queuedRenderable, bool casterPass,
		uint32 lastCacheHash, CommandBuffer* commandBuffer, bool isV1) {

	assert(
			dynamic_cast<const HlmsUnlitDatablock*>( queuedRenderable.renderable->getDatablock() ));
	const DSDatablock *datablock =
			static_cast<const DSDatablock*>(queuedRenderable.renderable->getDatablock());

	if ( OGRE_EXTRACT_HLMS_TYPE_FROM_CACHE_HASH( lastCacheHash ) != mType) {
		//We changed HlmsType, rebind the shared textures.
		mLastTextureHash = 0;
		mLastBoundPool = 0;

		//layout(binding = 0) uniform PassBuffer {} pass
		ConstBufferPacked *passBuffer = mPassBuffers[mCurrentPassBuffer - 1];
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
//	    if( mLastBoundPool != datablock->getAssignedPool() && !casterPass )
//	    {
//	        //layout(binding = 1) uniform MaterialBuf {} materialArray
//	        const ConstBufferPool::BufferPool *newPool = datablock->getAssignedPool();
//	        *commandBuffer->addCommand<CbShaderBuffer>() = CbShaderBuffer( PixelShader,
//	                                                                       1, newPool->materialBuffer, 0,
//	                                                                       newPool->materialBuffer->
//	                                                                       getTotalSizeBytes() );
//	        if( newPool->extraBuffer )
//	        {
//	            TexBufferPacked *extraBuffer = static_cast<TexBufferPacked*>( newPool->extraBuffer );
//	            *commandBuffer->addCommand<CbShaderBuffer>() = CbShaderBuffer( VertexShader, 1,
//	                                                                           extraBuffer, 0,
//	                                                                           extraBuffer->
//	                                                                           getTotalSizeBytes() );
//	        }
//
//	        mLastBoundPool = newPool;
//	    }

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

	if (!casterPass) {
		if (datablock->mTextureHash != mLastTextureHash) {
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

			*commandBuffer->addCommand<CbTextureDisableFrom>() =
					CbTextureDisableFrom(texUnit);

			mLastTextureHash = datablock->mTextureHash;
		}
	}

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
uint32 HlmsDS::fillBuffersForV2(const HlmsCache* cache,
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
			dynamic_cast<const HlmsUnlitDatablock*>( queuedRenderable.renderable->getDatablock() ));
	const DSDatablock *datablock =
			static_cast<const DSDatablock*>(queuedRenderable.renderable->getDatablock());

	if (!retVal->pso.pixelShader.isNull()) {
		GpuProgramParametersSharedPtr psParams =
				retVal->pso.pixelShader->getDefaultParameters();

		int texUnit = 2; //Vertex shader consumes 2 slots with its two tbuffers.

//        UnlitBakedTextureArray::const_iterator itor = datablock->mBakedTextures.begin();
//        UnlitBakedTextureArray::const_iterator end  = datablock->mBakedTextures.end();
//
//        if( !getProperty( HlmsBaseProp::ShadowCaster ) )
//        {
//            int numTextures = 0;
//            int numArrayTextures = 0;
//            while( itor != end )
//            {
//                if( itor->texture->getTextureType() == TEX_TYPE_2D_ARRAY )
//                {
//                    psParams->setNamedConstant( "textureMapsArray[" +
//                                                StringConverter::toString( numArrayTextures++ ) +
//                                                "]", texUnit++ );
//                }
//                else
//                {
//                    psParams->setNamedConstant( "textureMaps[" +
//                                                StringConverter::toString( numTextures++ ) + "]",
//                                                texUnit++ );
//                }
//
//                ++itor;
//            }
//        }
	}

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
	//std::cout << "*********************************createDatablockImpl" << "\n";
	Ogre::HlmsManager *hlmsManager = Root::getSingleton().getHlmsManager();

	HlmsDatablock*db = (new Ogre::DSDatablock(datablockName, this, macroblock,
			blendblock, paramVec));

	return new Ogre::DSDatablock(datablockName, this, macroblock, blendblock,
			paramVec);

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
}
