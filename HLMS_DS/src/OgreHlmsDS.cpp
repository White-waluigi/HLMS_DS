/*
 * OgreDSHLMS.cpp
 *
 *  Created on: 24.11.2015
 *      Author: marvin
 */
#include "Datablocks/Data/DSMaterialParamType.h"
#include "OgreStableHeaders.h"

#include "OgreHlmsDS.h"
#include "Datablocks/DSDatablock.h"
#include "Datablocks/DSLightDatablock.h"
#include "OgreHlmsManager.h"
#include "OgreHlmsListener.h"

#include "OgreViewport.h"
#include "OgreRenderTarget.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreHighLevelGpuProgram.h"
#include "OgreForward3D.h"

#include "OgreSceneManager.h"
#include "Compositor/OgreCompositorShadowNode.h"
#include "Vao/OgreVaoManager.h"
#include "Vao/OgreConstBufferPacked.h"
#include "Vao/OgreTexBufferPacked.h"

#include "CommandBuffer/OgreCommandBuffer.h"
#include "CommandBuffer/OgreCbTexture.h"
#include "CommandBuffer/OgreCbShaderBuffer.h"

#include "Animation/OgreSkeletonInstance.h"

#include "Light/StringValueUtils.h"

#include "Datablocks/Data/DSVec4Param.h"
#include "Datablocks/Data/DSTextureParam.h"
#include "Datablocks/Data/DSTextureParamType.h"
#include "Datablocks/DSMaterialDatablock.h"
#include "limits"
#include "Shadow/ShadowManager.h"

#include "OgreHlmsJson.h"
#include "OgreLwString.h"
namespace Ogre {

const IdString DSProperty::MaterialsPerBuffer = IdString(
		"materials_per_buffer");

const IdString DSProperty::isForward = IdString("isForward");
const IdString DSProperty::isGBuffer = IdString("isGBuffer");
const IdString DSProperty::isLight = IdString("isLight");

const IdString DSProperty::noTransf = IdString("noTransf");

const IdString DSProperty::NumTextures = IdString("num_textures");

const IdString DSProperty::DiffuseMap = IdString("diffuse_map");

const IdString DSProperty::NormalMap = IdString("normal_map");

const IdString DSProperty::SignedIntTex = IdString("signed_int_textures");

const Ogre::String DSProperty::Vec4Defines = ("vec4_");
const Ogre::String DSProperty::Vec4ValDefines = ("vec4v_");

const Ogre::String DSProperty::TextureLoc = ("tex_");
const Ogre::String DSProperty::TextureValLoc = ("texv_");
const Ogre::String DSProperty::TextureDefines = ("texd_");
const Ogre::String DSProperty::TextureUVindex = ("uvi_");

const String DSProperty::NumTextureParams = ("NumTextureDefines");
const String DSProperty::NumVec4Params = ("NumVec4Defines");

const String DSProperty::MaxTextureParams = ("MaxTextureDefines");
const String DSProperty::MaxVec4Params = ("MaxVec4Defines");

const String DSProperty::Shadow = ("Shadow");

const Ogre::String DSProperty::MaterialVec4Params = ("MaterialVec4Params");
const Ogre::String DSProperty::MaterialTexParams = ("MaterialTexParams");
const Ogre::String DSProperty::MaterialAutoparamParams =
		("MaterialAutoparamParams");
const Ogre::String DSProperty::MaterialAutoTexParams = ("MaterialAutoTexParams");

const Ogre::String DSProperty::TextureHelper = ("TextureHelper");

const Ogre::String DSProperty::NumSubTextures = ("NumSubTextures");

const Ogre::String DSProperty::NumShadowTex = ("NumShadowTex");

HlmsDS::HlmsDS(Archive* dataFolder, ArchiveVec* libraryFolders) :
		HlmsBufferManager(HLMS_PBS, "ds", dataFolder, libraryFolders), ConstBufferPool(
				DSDatablock::MaterialSizeInGpuAligned,
				ConstBufferPool::ExtraBufferParams()) {
	//std::cout <<"*****************************************\n"<<"HlmsDS"<<"\n";
	this->lightmanager = new DSLightManager(this);
	this->shadowmanager = new ShadowManager(new ShadowManager::Config(), this);
	this->mShadowmapCmpSamplerblock = 0;
	this->mShadowmapSamplerblock = 0;
	this->mCurrentShadowmapSamplerblock = 0;

	mCurrentPassBuffer = 0;
	mGBuffer = new std::vector<TexturePtr>();
	mGBufferInd = new std::vector<int>();

	this->dsDatablocks = new std::vector<Ogre::DSDatablock*>();
	this->timer = new Ogre::Timer();

	this->debugMode = DM_OFF;
	this->fullBrightMode = FBM_OFF;

	this->mWSListener = new HLMSDSWorkspaceListener();

	this->jsonDs = new JsonParser(mHlmsManager, this);

}
HlmsCache HlmsDS::preparePassHash(const Ogre::CompositorShadowNode* shadowNode,
		bool casterPass, bool dualParaboloid, SceneManager* sceneManager) {

	incr = 0;
	currentpassID++;
	mSetProperties.clear();

	int GbufferTexID = -1;


	//find the GBUffer Compoistor texture
	int size = sceneManager->getCompositorTextures().size();
	for (uint i = 0; i < size; i++) {
		IdString name =
				sceneManager->getCompositorTextures().at(i).name.mDebugString;
		if ((name == IdString("GBuffer"))) {
			GbufferTexID = i;
		}

	}
	//need a GBUffer Compositor Texture
	assert(
			GbufferTexID != -1
					|| mWSListener->passID != HLMSDSWorkspaceListener::LightID);

	if (GbufferTexID != -1) {
		for (int i = 0; i < this->mNumGBuffers; i++) {

			mGBuffer->push_back(
					sceneManager->getCompositorTextures().at(GbufferTexID).textures->at(
							i));
		}
	}else{

	}

	//clear the old shadowMaps
	mPreparedPass.shadowMaps.clear();

	HlmsCache retVal = Hlms::preparePassHashBase(shadowNode, casterPass,
			dualParaboloid, sceneManager);

	int32 numShadowMaps = getProperty(HlmsBaseProp::NumShadowMaps);
	int32 numPssmSplits = getProperty(HlmsBaseProp::PssmSplits);
	this->NumPssmSplits = numPssmSplits;

	mPreparedPass.shadowMaps.reserve(numShadowMaps);
	for (int32 i = 0; i < numShadowMaps; ++i)
		mPreparedPass.shadowMaps.push_back(
				shadowNode->getLocalTextures()[i].textures[0]);

	//check check if we are in the first pass
	if (mWSListener->passType == HLMSDSWorkspaceListener::PT_SHADOW
			&& !casterPass) {
		lightmanager->CheckForNewLights(sceneManager, shadowNode,
				numPssmSplits);
		lightmanager->updateLightData(sceneManager, shadowNode, numPssmSplits);

		mWSListener->passType = (HLMSDSWorkspaceListener::PassType) 0;

	}

	if (mShadowmapSamplerblock
			&& !getProperty(HlmsBaseProp::ShadowUsesDepthTexture))
		mCurrentShadowmapSamplerblock = mShadowmapSamplerblock;
	else
		mCurrentShadowmapSamplerblock = mShadowmapCmpSamplerblock;

	Ogre::Camera * camera = sceneManager->getCameraInProgress();
	//map all the matrixes to the passbuffer
	static bool setonce = false;
	view = sceneManager->getCameraInProgress()->getViewMatrix();
	proj = sceneManager->getCameraInProgress()->getProjectionMatrix();
	RenderTarget *renderTarget =
			sceneManager->getCurrentViewport()->getTarget();
	requiresTextureFlipping = false;

	if (renderTarget->requiresTextureFlipping()) {
		proj[1][0] = -proj[1][0];
		proj[1][1] = -proj[1][1];
		proj[1][2] = -proj[1][2];
		proj[1][3] = -proj[1][3];
		requiresTextureFlipping = true;

	}

	setonce = true;

	mPreparedPass.viewMatrix = view;

	mPreparedPass.projMatrix = proj;
	curviewProjMatrix = proj;

//	}
	Matrix4 tempTexProjMat;


//seams more modern to hold the passbufferdata in a buffer instead of directly sending to thre GPU, since its a few kb at most and lets me keep better track of it and debug it easier

	std::vector<float> * passbufferBuffer = new std::vector<float>();

	//******************Debug Modes**********************************************
	uint dm = static_cast<int>(debugMode);
	uint dm2 = static_cast<int>(fullBrightMode);
	uint dm3 = static_cast<int>(Debugz);
	uint dm4 = static_cast<int>(Debugw);

	float fdm = reinterpret_cast<float &>(dm);
	float fdm2 = reinterpret_cast<float &>(dm2);
	float fdm3 = reinterpret_cast<float &>(dm3);
	float fdm4 = reinterpret_cast<float &>(dm4);

	passbufferBuffer->push_back((fdm));
	passbufferBuffer->push_back((fdm2));
	passbufferBuffer->push_back(fdm3);
	passbufferBuffer->push_back(fdm4);
//**********************View Projection Matrix******************************************
	Matrix4 tmp = mPreparedPass.projMatrix.transpose();

	for (size_t i = 0; i < 16; ++i)
		passbufferBuffer->push_back((float) tmp[0][i]);

	//*passBufferPtr++ = (float) tmp[0][i];

//**********************View Matrix******************************************
	Matrix4 tview = mPreparedPass.viewMatrix;
	tmp = tview.transpose();

	for (size_t i = 0; i < 16; ++i)
		passbufferBuffer->push_back((float) tmp[0][i]);
//**********************Inverse View Matrix******************************************
	Matrix4 itview = mPreparedPass.viewMatrix.inverseAffine();
	tmp = itview.transpose();

	for (size_t i = 0; i < 16; ++i)
		passbufferBuffer->push_back((float) tmp[0][i]);
//****************************************************************

	Vector3 lightPos;
	Matrix3 viewMatrix3, invViewMatrix3;
	mPreparedPass.viewMatrix.extract3x3Matrix(viewMatrix3);
	invViewMatrix3 = invViewMatrix3.Inverse();

//********************Timers for Testing********************************************

	float timer1 = (timer->getMicroseconds() % 1000000) / 1000000.0;
	float timer2 = (timer->getMicroseconds() % 10000000) / 10000000.0;

	float timer3 = ((timer->getMicroseconds() % 1000000)) / 1000000.0;
	if ((timer->getMicroseconds() % 2000000) > 1000000) {
		timer3 = (1000000 - (timer->getMicroseconds() % 1000000)) / 1000000.0;
	}

	float timer4 = ((timer->getMicroseconds() % 3000000)) / 3000000.0;
	if ((timer->getMicroseconds() % 6000000) > 3000000) {
		timer4 = (3000000 - (timer->getMicroseconds() % 3000000)) / 3000000.0;
	}

	passbufferBuffer->push_back(timer1);
	passbufferBuffer->push_back(timer2);
	passbufferBuffer->push_back(timer3);
	passbufferBuffer->push_back(timer4);

//**********************Clipping+Screen Size Information********************************************
	passbufferBuffer->push_back(camera->getNearClipDistance());
	passbufferBuffer->push_back(camera->getFarClipDistance());
	passbufferBuffer->push_back(camera->getLastViewport()->getActualWidth());
	passbufferBuffer->push_back(camera->getLastViewport()->getActualHeight());

	Vector3 wspc = camera->getWorldSpaceCorners()[4];
	float flip = 1;
	if (requiresTextureFlipping) {
		flip = -1;
	}

	passbufferBuffer->push_back((camera->getViewMatrix(true) * (wspc)).x);
	passbufferBuffer->push_back(
			(camera->getViewMatrix(true) * (wspc)).y * flip);
	passbufferBuffer->push_back((camera->getViewMatrix(true) * wspc).z);
	passbufferBuffer->push_back(0);

	//**********************Texture flipping********************************************

	Vector4 test = mPreparedPass.viewMatrix.transformAffine(
			Vector4(1, 0, 0, 0));

	passbufferBuffer->push_back(mPreparedPass.viewMatrix[0][0]);
	passbufferBuffer->push_back(mPreparedPass.viewMatrix[0][1]);
	passbufferBuffer->push_back(mPreparedPass.viewMatrix[0][2]);
	passbufferBuffer->push_back(mPreparedPass.viewMatrix[0][3]);

	//**********************Camera Clip********************************************

	//vec2 depthRange;

	Real fNear, fFar;

	fFar = camera->getFarClipDistance();
	fNear = camera->getNearClipDistance();

	const Real depthRange = fFar - fNear;

	passbufferBuffer->push_back(fNear);
	passbufferBuffer->push_back(fFar);
	passbufferBuffer->push_back(1.0f / depthRange);
	passbufferBuffer->push_back(depthRange);

	//**********************TestData********************************************
	passbufferBuffer->push_back(testdata.x);
	passbufferBuffer->push_back(testdata.y);
	passbufferBuffer->push_back(testdata.z);
	passbufferBuffer->push_back(testdata.w);

	//**********************Shadow Info********************************************
	//Why is this the only way to link shadow maps to lights?
	//mapp all the required shadow map and their data to the Passbuffer (Deferred shadows use data in their Datablocks, but this is needed for the forwarded lights)
	if (!casterPass && shadowNode != NULL) {

		int numPssmSplits = getProperty(HlmsBaseProp::PssmSplits);

		for (size_t i = 0; i < numPssmSplits; i++) {
			passbufferBuffer->push_back(
					shadowNode->getPssmSplits(0)->at(i + 1));
			passbufferBuffer->push_back(
					1.0 / shadowNode->getPssmSplits(0)->at(i + 1));
			passbufferBuffer->push_back(0);
			passbufferBuffer->push_back(0);

		}

		for (int j = 0; j < numShadowMaps; j++) {
			int currentShadowMap = 0;

			uint lightID = 0;
			if (shadowNode) {
				LightClosestArray scl = shadowNode->getShadowCastingLights();

				for (size_t i = 0; i < scl.size(); i++) {

					if (currentShadowMap == j) {
						//iterator->second->ShadowMapId =shadownode->getShadowCastingLights()[i].globalIndex;
						lightID = scl[i].light->getId();
					}
					if (shadowNode->getShadowCastingLights()[i].light->getType()
							== Light::LT_DIRECTIONAL) {
						currentShadowMap += NumPssmSplits;
					} else {
						currentShadowMap++;
					}

				}
			}
			float flightID = reinterpret_cast<float &>(lightID);
			passbufferBuffer->push_back(
					((float) shadowNode->getLocalTextures().at(j).textures[0]->getWidth()));
			passbufferBuffer->push_back(
					((float) shadowNode->getLocalTextures().at(j).textures[0]->getHeight()));
			passbufferBuffer->push_back(
					1.0
							/ ((float) shadowNode->getLocalTextures().at(j).textures[0]->getWidth()));
			passbufferBuffer->push_back(
					1.0
							/ ((float) shadowNode->getLocalTextures().at(j).textures[0]->getHeight()));

			passbufferBuffer->push_back(0);
			passbufferBuffer->push_back(0);
			passbufferBuffer->push_back(0);
			passbufferBuffer->push_back(flightID);

			Real fNear, fFar;
			if (shadowNode != NULL) {
				shadowNode->getMinMaxDepthRange(j, fNear, fFar);
			}
			const Real depthRange = fFar - fNear;

			passbufferBuffer->push_back(fNear);
			passbufferBuffer->push_back(fFar);
			passbufferBuffer->push_back(1.0f / depthRange);
			passbufferBuffer->push_back(depthRange);

			Matrix4 tmp = shadowNode->getViewProjectionMatrix(j).transpose();
			//tmp = tempTexProjMat.transpose();

			for (size_t i = 0; i < 16; ++i) {
				passbufferBuffer->push_back((float) tmp[0][i]);
			}

		}
	}
//	} else {
//		for (int j = 0; j < 5; j++) {
//			for (size_t i = 0; i < 16; ++i)
//				passbufferBuffer->push_back(0);
//		}
//
//	}

//*************************************Redirecting to actual passbuffer*******************************************

	size_t maxBufferSize = passbufferBuffer->size() * sizeof(float);

	//Arbitrary 16kb (minimum supported by GL), should be enough.
	maxBufferSize = 16 * 1024;

	if (mCurrentPassBuffer >= mPassBuffers.size()) {
		mPassBuffers.push_back(
				mVaoManager->createConstBuffer(maxBufferSize,
						BT_DYNAMIC_PERSISTENT, 0, false));
	}
	ConstBufferPacked *passBuffer = mPassBuffers[mCurrentPassBuffer++];

	float *passBufferPtr = reinterpret_cast<float*>(passBuffer->map(0,
			maxBufferSize));

	for (uint i = 0; i < passbufferBuffer->size(); i++) {
		//finally map it to the Passsbuffer on the GPU

		*passBufferPtr++ = passbufferBuffer->at(i);
	}

	if (mTexBuffers.empty()) {
		size_t bufferSize = std::min<size_t>(mTextureBufferDefaultSize,
				mVaoManager->getTexBufferMaxSize());
		TexBufferPacked *newBuffer = mVaoManager->createTexBuffer(
				PF_FLOAT32_RGBA, bufferSize, BT_DYNAMIC_PERSISTENT, 0, false);
		mTexBuffers.push_back(newBuffer);
	}

	//*********************Cleaning Up*********************************************

	passBuffer->unmap(UO_KEEP_PERSISTENT);


	uploadDirtyDatablocks();
	uploadDirtyDSDatablocks();

	mLastTextureHash = 0;

	mLastBoundPool = 0;
	return retVal;
}
void HlmsDS::setProperties(Renderable* renderable, PiecesMap* inOutPieces,
		bool ishadow) {
	IdString("hw_gamma_read");
	setProperty(IdString("NumShadowMaps"), mPreparedPass.shadowMaps.size());

	//might be obsolete soon
	int dbType = getDatablocktype(renderable->getDatablock());

	int count = 0;

	if (dbType == DT_Forward) {
		count++;
		setProperty(DSProperty::isForward, 1);

	}

	if (dbType == DT_Light) {
		count++;
		setProperty(DSProperty::isLight, 1);
	}

	if (dbType == DT_GBuffer) {

		count++;
		setProperty(DSProperty::isGBuffer, 1);


	}
	assert(count == 1);
	DSDatablock * datablock =
			dynamic_cast<DSDatablock*>(renderable->getDatablock());

	setProperty(DSProperty::NumTextures, datablock->mBakedTextures.size());

	for (uint i = 0; i < datablock->getPropertyParams()->size(); i++) {
		DSPropertyParam *param = datablock->getPropertyParams()->at(i);

		setProperty(param->getKey(), param->getValue());
	}

}
void HlmsDS::setPieces(Renderable* renderable, PiecesMap* inOutPieces,
		bool bool1) {

	//all necessary pieces are already defined in the datablocks
	DSDatablock * datablock =
			dynamic_cast<DSDatablock*>(renderable->getDatablock());

	for (uint i = 0; i < datablock->getPieceParams()->size(); i++) {
		DSPieceParam *param = datablock->getPieceParams()->at(i);
		inOutPieces[param->getShader()][param->getId()] = param->getPiece();
	}

}
void HlmsDS::calculateHashForPreCreate(Renderable* renderable,
		PiecesMap* inOutPieces) {
	Ogre::String uwot = renderable->getDatablock()->getName().getFriendlyText();


	setProperties(renderable, inOutPieces, false);
	setPieces(renderable, inOutPieces, false);
	String slotsPerPoolStr = StringConverter::toString(mSlotsPerPool);

	inOutPieces[VertexShader][DSProperty::MaterialsPerBuffer] = slotsPerPoolStr;
	inOutPieces[PixelShader][DSProperty::MaterialsPerBuffer] = slotsPerPoolStr;

}

void Ogre::HlmsDS::calculateHashForPreCaster(Renderable* renderable,
		PiecesMap* inOutPieces) {

	int dbType = getDatablocktype(renderable->getDatablock());

	HlmsPropertyVec::iterator itor = mSetProperties.begin();
	HlmsPropertyVec::iterator end = mSetProperties.end();

	//todo skeleton, animation support

	DSDatablock *datablock =
			static_cast<DSDatablock*>(renderable->getDatablock());

	//ShadowObjects always treated as GBuffer objects
	setProperty(DSProperty::isGBuffer, 1);
	setPieces(renderable, inOutPieces, false);

	String slotsPerPoolStr = StringConverter::toString(mSlotsPerPool);
	inOutPieces[VertexShader][DSProperty::MaterialsPerBuffer] = slotsPerPoolStr;
	inOutPieces[PixelShader][DSProperty::MaterialsPerBuffer] = slotsPerPoolStr;
}

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

		for (size_t i = 0; i < dsDatablocks->size(); i++) {
			dsDatablocks->at(i)->frameEnded();

			if (dsDatablocks->at(i)->MaterialBuffer->getMappingState()
					!= MS_UNMAPPED)
				dsDatablocks->at(i)->MaterialBuffer->unmap(UO_UNMAP_ALL);
			mVaoManager->destroyConstBuffer(
					dsDatablocks->at(i)->MaterialBuffer);

			if (dsDatablocks->at(i)->ShadowMaterialBuffer->getMappingState()
					!= MS_UNMAPPED)
				dsDatablocks->at(i)->ShadowMaterialBuffer->unmap(UO_UNMAP_ALL);
			mVaoManager->destroyConstBuffer(
					dsDatablocks->at(i)->ShadowMaterialBuffer);
		}

		mPassBuffers.clear();
	}
}
//very usefull for testing if buffers are uploaded correctly
Vector3 rainbow(float x) {

	float div = (std::abs(fmod(x, 1.0f)) * 6);
	int ascending = (int) (fmod(div, 1.0f) * 255);
	int descending = 255 - ascending;

	float a = ascending / 255.0;
	float d = descending / 255.0;

	switch ((int) div) {
	case 0:
		return Vector3(1, a, 0);
	case 1:
		return Vector3(d, 1, 0);
	case 2:
		return Vector3(0, 1, a);
	case 3:
		return Vector3(0, d, 1);
	case 4:
		return Vector3(a, 0, 1);
	default: // case 5:
		return Vector3(1, 0, d);
	}
}

//Also for testing
Vector3 rainbow() {
	static float r = 0;
	r += 0.0001;
	if (r > 1) {
		r = 0;
	}
	return rainbow(r);

}
inline uint32 HlmsDS::fillBuffersFor(const HlmsCache* cache,
		const QueuedRenderable& queuedRenderable, bool casterPass,
		uint32 lastCacheHash, CommandBuffer* commandBuffer, bool isV1) {

	int dbt = getDatablocktype(queuedRenderable.renderable->getDatablock());
	if (casterPass) {
		const DSDatablock *ldatablock =
				static_cast<const DSDatablock*>(queuedRenderable.renderable->getDatablock());

	}
	if ( OGRE_EXTRACT_HLMS_TYPE_FROM_CACHE_HASH( lastCacheHash ) != HLMS_PBS) {
		ConstBufferPacked *passBuffer = mPassBuffers[mCurrentPassBuffer - 1];

		int i = mPassBuffers.size();
//    	    *commandBuffer->addCommand<CbShaderBuffer>() = CbShaderBuffer( PixelShader,
//    	                                                                   2, passBuffer, 0,
//																		   passBuffer->getTotalSizeBytes() );
		int texUnit = 5;

		//map the Gbuffer
		if (dbt == this->DT_Light) {
			const DSLightDatablock *ldatablock =
					static_cast<const DSLightDatablock*>(queuedRenderable.renderable->getDatablock());

			for (int i = 0; i < this->mNumGBuffers; i++) {
				*commandBuffer->addCommand<CbTexture>() = CbTexture(
						this->mGBufferInd->at(i), true,
						this->mGBuffer->at(i).get(),
						ldatablock->GBufferSamplerblock);

				texUnit = this->mGBufferInd->at(i);
			}
			FastArray<TexturePtr>::const_iterator itors =
					mPreparedPass.shadowMaps.begin();
			FastArray<TexturePtr>::const_iterator ends =
					mPreparedPass.shadowMaps.end();

			int i = 0;
			while (itors != ends) {

				*commandBuffer->addCommand<CbTexture>() = CbTexture(
						mPreparedPass.shadowMapTUs[i], true, itors->get(),
						mCurrentShadowmapSamplerblock);

				++texUnit;
				++itors;
				++i;
			}

//						mHlmsManager->getTextureManager()->createOrRetrieveTexture("BumpyMetal.jpg",HlmsTextureManager::TEXTURE_TYPE_DIFFUSE).texture.get()

		}

		*commandBuffer->addCommand<CbShaderBuffer>() = CbShaderBuffer(
				VertexShader, 0, passBuffer, 0,
				passBuffer->getTotalSizeBytes());

		mLastTextureHash = 0;
		mLastBoundPool = 0;

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

		mListener->hlmsTypeChanged(casterPass, commandBuffer,
				queuedRenderable.renderable->getDatablock());

	}

	uint32 assSlot = 0;

	DSDatablock *datablock =
			static_cast<DSDatablock*>(queuedRenderable.renderable->getDatablock());

//	if (datablock->getParam("position") != NULL) {
//		std::cout << *datablock->getParam("position") << "\t" <<datablock->isDirty()<< "\n";
//	}
	ConstBufferPacked *materialBuffer = datablock->getMaterialBuffer();

	*commandBuffer->addCommand<CbShaderBuffer>() = CbShaderBuffer(PixelShader,
			1, materialBuffer, 0, materialBuffer->getTotalSizeBytes());

	//---------------------------------------------------------------------------
	//                          ---- VERTEX SHADER ----
	//---------------------------------------------------------------------------
	int drawid = generateWorldMatrixBuffer(cache, queuedRenderable, casterPass,
			lastCacheHash, commandBuffer, isV1);
	//---------------------------------------------------------------------------
	//                          ---- PIXEL SHADER ----
	//---------------------------------------------------------------------------
	if (dbt == DT_GBuffer) {
		const DSDatablock *datablock =
				static_cast<const DSDatablock*>(queuedRenderable.renderable->getDatablock());

		if (datablock->mTextureHash != mLastTextureHash) {
			//Rebind textures
			size_t texUnit = 1;

			DSDatablock::DSBakedTexturesArray::const_iterator itor =
					datablock->mBakedTextures.begin();
			DSDatablock::DSBakedTexturesArray::const_iterator end =
					datablock->mBakedTextures.end();

			while (itor != end) {

				*commandBuffer->addCommand<CbTexture>() = CbTexture(texUnit++,
						true, itor->texture.get(), itor->samplerBlock);

				++itor;

			}

			*commandBuffer->addCommand<CbTextureDisableFrom>() =
					CbTextureDisableFrom(texUnit);

			mLastTextureHash = datablock->mTextureHash;
		}

	}

	//return ((mCurrentMappedConstBuffer - mStartMappedConstBuffer) >> 2) - 1;
	return drawid;
}

uint32 HlmsDS::fillBuffersForV1(const HlmsCache* cache,
		const QueuedRenderable& queuedRenderable, bool casterPass,
		uint32 lastCacheHash, CommandBuffer* commandBuffer) {
//	std::cout <<"*****************************************\n"<<"fillBuffersForV1"<<"\n";
	return fillBuffersFor(cache, queuedRenderable, casterPass, lastCacheHash,
			commandBuffer, true);
}

uint32 HlmsDS::fillBuffersForV2(const HlmsCache* cache,
		const QueuedRenderable& queuedRenderable, bool casterPass,
		uint32 lastCacheHash, CommandBuffer* commandBuffer) {
//	std::cout <<"*****************************************\n"<<"fillBuffersForV2"<<"\n";
	return fillBuffersFor(cache, queuedRenderable, casterPass, lastCacheHash,
			commandBuffer, false);
}

void HlmsDS::frameEnded(void) {

	HlmsBufferManager::frameEnded();
	mCurrentPassBuffer = 0;

	for (size_t i = 0; i < dsDatablocks->size(); i++) {
		dsDatablocks->at(i)->frameEnded();
	}
	CurLightMat = 0;
	currentpassID = 0;
}

const HlmsCache* HlmsDS::createShaderCacheEntry(uint32 renderableHash,
		const HlmsCache& passCache, uint32 finalHash,
		const QueuedRenderable& queuedRenderable) {
	const HlmsCache *retVal = Hlms::createShaderCacheEntry(renderableHash,
			passCache, finalHash, queuedRenderable);

	if (mShaderProfile == "hlsl") {
		mListener->shaderCacheEntryCreated(mShaderProfile, retVal, passCache,
				mSetProperties, queuedRenderable);
		return retVal; //D3D embeds the texture slots in the shader.
	}

	//Set samplers.
	if (!retVal->pso.pixelShader.isNull()) {
		GpuProgramParametersSharedPtr psParams =
				retVal->pso.pixelShader->getDefaultParameters();

		int texUnit = 1; //Vertex shader consumes 1 slot with its tbuffer.

		//Forward3D consumes 2 more slots.

		if (getDatablocktype(queuedRenderable.renderable->getDatablock())
				== DT_Light) {
			for (int i = 0; i < this->mNumGBuffers; i++) {

				std::stringstream ss;
				ss << "GBuffer" << i;
				std::string s = ss.str();
				this->mGBufferInd->push_back(texUnit);
				psParams->setNamedConstant(s, texUnit++);
			}
			if (!mPreparedPass.shadowMaps.empty()) {
				vector<int>::type shadowMaps;
				shadowMaps.reserve(mPreparedPass.shadowMaps.size());
				mPreparedPass.shadowMapTUs.reserve(
						mPreparedPass.shadowMaps.size());
				for (size_t i = 0; i < mPreparedPass.shadowMaps.size(); ++i) {
					shadowMaps.push_back(texUnit);
					mPreparedPass.shadowMapTUs.push_back(texUnit);
					texUnit++;
				}

				psParams->setNamedConstant("texShadowMap", &shadowMaps[0],
						shadowMaps.size(), 1);

			}

		}
		if (getDatablocktype(queuedRenderable.renderable->getDatablock())
				== DT_GBuffer) {
			int numTextures = getProperty(DSProperty::NumTextures);
			for (int i = 0; i < numTextures; ++i) {
				psParams->setNamedConstant(
						"textureMaps[" + StringConverter::toString(i) + "]",
						texUnit++);
			}

		}

	}

	GpuProgramParametersSharedPtr vsParams =
			retVal->pso.vertexShader->getDefaultParameters();
//vsParams->setNamedConstant( "worldMatBuf", 0 );

	mListener->shaderCacheEntryCreated(mShaderProfile, retVal, passCache,
			mSetProperties, queuedRenderable);
	mRenderSystem->_setPipelineStateObject(&retVal->pso);
	//mRenderSystem->pso._setProgramsFromHlms(retVal);

	mRenderSystem->bindGpuProgramParameters(GPT_VERTEX_PROGRAM, vsParams,
			GPV_ALL);
	if (!retVal->pso.pixelShader.isNull()) {
		GpuProgramParametersSharedPtr psParams =
				retVal->pso.pixelShader->getDefaultParameters();
		mRenderSystem->bindGpuProgramParameters(GPT_FRAGMENT_PROGRAM, psParams,
				GPV_ALL);
	}

	return retVal;
}

HlmsDatablock* HlmsDS::createDatablockImpl(IdString datablockName,
		const HlmsMacroblock* macroblock, const HlmsBlendblock* blendblock,
		const HlmsParamVec& paramVec) {

	Ogre::DSDatablock* datab;

	bool light = false;

	for (uint i = 0; i < paramVec.size(); i++) {
		if (paramVec.at(i).first == IdString("type")) {
			light = paramVec[i].second.compare("light") == 0;
		}
	}

	if (light) {

		datab = OGRE_NEW DSLightDatablock(datablockName, this, macroblock,
				blendblock, paramVec);

	} else {

		datab = OGRE_NEW DSMaterialDatablock(datablockName, this, macroblock,
				blendblock, paramVec);
	}
	dsDatablocks->push_back(datab);
	return datab;

}
void HlmsDS::_changeRenderSystem(RenderSystem* newRs) {
//std::cout <<"*****************************************\n"<<"_changeRenderSystem"<<"\n";
	ConstBufferPool::_changeRenderSystem(newRs);
	HlmsBufferManager::_changeRenderSystem(newRs);

	if (newRs) {
		HlmsDatablockMap::const_iterator itor = mDatablocks.begin();
		HlmsDatablockMap::const_iterator end = mDatablocks.end();

		while (itor != end) {
			assert(dynamic_cast<DSDatablock*>(itor->second.datablock));
			DSDatablock *datablock =
					static_cast<DSDatablock*>(itor->second.datablock);

			++itor;
		}

		HlmsSamplerblock samplerblock;
		samplerblock.mU = TAM_BORDER;
		samplerblock.mV = TAM_BORDER;
		samplerblock.mW = TAM_CLAMP;
		samplerblock.mBorderColour = ColourValue::White;

		if (mShaderProfile != "hlsl") {
			samplerblock.mMinFilter = FO_ANISOTROPIC;
			samplerblock.mMagFilter = FO_ANISOTROPIC;
			samplerblock.mMipFilter = FO_ANISOTROPIC;

			if (!mShadowmapSamplerblock)
				mShadowmapSamplerblock = mHlmsManager->getSamplerblock(
						samplerblock);
		}

		samplerblock.mMinFilter = FO_LINEAR;
		samplerblock.mMagFilter = FO_LINEAR;
		samplerblock.mMipFilter = FO_NONE;
		samplerblock.mCompareFunction = CMPF_LESS_EQUAL;

		if (!mShadowmapCmpSamplerblock)
			mShadowmapCmpSamplerblock = mHlmsManager->getSamplerblock(
					samplerblock);
	}
}

uint32 HlmsDS::fillBuffersFor(const HlmsCache* cache,
		const QueuedRenderable& queuedRenderable, bool casterPass,
		uint32 lastCacheHash, uint32 lastTextureHash) {
//std::cout <<"*****************************************\n"<<"fillBuffersFor"<<"\n";
	return 0;
}
void HlmsDS::mapMatrixToBuffer(float *passBufferPtr, Matrix4 mat) {

	for (int i = 0; i < 4; i++) {
		for (int ii = 0; ii < 4; ii++) {

			*passBufferPtr++ = mat[i][ii];
		}

	}
}
HlmsDS::Datablock_Type HlmsDS::getDatablocktype(
		const HlmsDatablock * hlmsDatablock) {

	if (dynamic_cast<const DSDatablock*>(hlmsDatablock)->dbtype
			== DSDatablock::DS_DATABLOCK_MATERIAL) {

		return DT_GBuffer;
	} else if (dynamic_cast<const DSDatablock*>(hlmsDatablock)->dbtype
			== DSDatablock::DS_DATABLOCK_LIGHT) {
		return DT_Light;
	} else {

		assert(false);
	}
}
const DSLightDatablock* HlmsDS::CastDatablockLight(
		const HlmsDatablock*hlmsDatablock) {
	if (dynamic_cast<const DSDatablock*>(hlmsDatablock)->dbtype
			== DSDatablock::DS_DATABLOCK_MATERIAL) {

		assert(false);
	} else if (dynamic_cast<const DSDatablock*>(hlmsDatablock)->dbtype
			== DSDatablock::DS_DATABLOCK_LIGHT) {

		return dynamic_cast<const DSLightDatablock*>(hlmsDatablock);
	} else {

		assert(false);
	}
}

const DSDatablock* HlmsDS::CastDatablockGBuffer(
		const HlmsDatablock* hlmsDatablock) {
	if (dynamic_cast<const DSDatablock*>(hlmsDatablock) != NULL) {

		return dynamic_cast<const DSDatablock*>(hlmsDatablock);
	} else if (dynamic_cast<const DSLightDatablock*>(hlmsDatablock) != NULL) {

		assert(false);
	} else {

		assert(false);
	}
}
void HlmsDS::setDebugMode(DebugMode dm) {
	this->debugMode = dm;
}
void HlmsDS::setFullBright(FullBrightMode fbm) {
	this->fullBrightMode = fbm;
}
void HlmsDS::uploadDirtyDSDatablocks() {
	for (size_t i = 0; i < this->dsDatablocks->size(); ++i) {
		this->dsDatablocks->at(i)->syncWithGPU();
	}
}
int HlmsDS::getDebugw() const {
	return Debugw;
}

void HlmsDS::setDebugw(int debugw) {
	Debugw = debugw;
}

int HlmsDS::getDebugz() const {
	return Debugz;
}

void HlmsDS::setDebugz(int debugz) {
	Debugz = debugz;
}
int HlmsDS::generateWorldMatrixBuffer(const HlmsCache* cache,
		const QueuedRenderable& queuedRenderable, bool casterPass,
		uint32 lastCacheHash, CommandBuffer* commandBuffer, bool isV1) {

	uint32 * RESTRICT_ALIAS currentMappedConstBuffer = mCurrentMappedConstBuffer;
	float * RESTRICT_ALIAS currentMappedTexBuffer = mCurrentMappedTexBuffer;

	bool hasSkeletonAnimation =
			queuedRenderable.renderable->hasSkeletonAnimation();
	const Matrix4 &worldMat =
			queuedRenderable.movableObject->_getParentNodeFullTransform();

#if !OGRE_DOUBLE_PRECISION

	int drawid = ((mCurrentMappedConstBuffer - mStartMappedConstBuffer) / 4);

	const size_t currentConstOffset = (currentMappedTexBuffer
			- mStartMappedTexBuffer) >> (2 + 1);

	currentMappedConstBuffer = currentConstOffset + mStartMappedConstBuffer;
	bool exceedsConstBuffer = (size_t) ((currentMappedConstBuffer
			- mStartMappedConstBuffer) + 4) > mCurrentConstBufferSize;

	const size_t minimumTexBufferSize = 16 * (1 + 1);
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



	memcpy(currentMappedTexBuffer, &worldMat, 4 * 4 * sizeof(float));

	currentMappedTexBuffer += 16;

	Matrix4 tmp = mPreparedPass.viewMatrix.concatenateAffine(worldMat);
	tmp = mPreparedPass.viewMatrix * worldMat;
#ifdef OGRE_GLES2_WORKAROUND_1
	tmp = tmp.transpose();
#endif



	memcpy(currentMappedTexBuffer, &tmp, sizeof(Matrix4));
	currentMappedTexBuffer += 16;


#else
#error Not Coded Yet! (cannot use memcpy on Matrix4)
#endif

	size_t distToWorldMatStart = mCurrentMappedTexBuffer
			- mStartMappedTexBuffer;
	distToWorldMatStart >>= 2;

	*currentMappedConstBuffer = distToWorldMatStart;
	currentMappedConstBuffer += 4;
	mCurrentMappedConstBuffer = currentMappedConstBuffer;
	mCurrentMappedTexBuffer = currentMappedTexBuffer;

	return drawid;

}

#if !OGRE_NO_JSON
//-----------------------------------------------------------------------------------
void HlmsDS::_loadJson(const rapidjson::Value &jsonValue,
		const HlmsJson::NamedBlocks &blocks, HlmsDatablock *datablock) const {

	//take a default Datablock and transform it into a JSON Datablock
	jsonDs->loadMaterial(jsonValue, blocks, datablock);
}
//-----------------------------------------------------------------------------------
void HlmsDS::_saveJson(const HlmsDatablock *datablock,
		String &outString) const {
	//todo
	jsonDs->saveMaterial(datablock, outString);
}
//-----------------------------------------------------------------------------------
void HlmsDS::_collectSamplerblocks(
		set<const HlmsSamplerblock*>::type &outSamplerblocks,
		const HlmsDatablock *datablock) const {
	//todo
	JsonParser::collectSamplerblocks(datablock, outSamplerblocks);
}
#endif
const HlmsSamplerblock* HlmsDS::getSamplerBlock(
		//shortcut
		HlmsSamplerblock hlmsSamplerblock) {
	return mHlmsManager->getSamplerblock(hlmsSamplerblock);
}

Ogre::String HlmsDS::getShaderProfile() {
	return mShaderProfile;
}

} /* namespace Ogre */

