/*
 * PassBufferDefaultVal.cpp
 *
 *  Created on: Jan 22, 2018
 *      Author: marvin
 */

#include "../AutoVals/PassBufferDefaultVal.h"

#include "../../../Util/DSMath.h"

namespace Ogre {

PassBufferDefaultVal::PassBufferDefaultVal(reflist l) :Value(l){

	//Set parent PassBuffer
	this->mParentPB=dynamic_cast<PassBufferManager*> (getReference("passbuffer"));

	//Hook into HLMS Events
	this->mParentHLMS->addListener(this);

	//Reserve Space for DataBuffer
	initData();
}

refreq * PassBufferDefaultVal::getRequiredRefs() {
	//Need a backlink to PassBufferManager
	refreq * r= Value::getRequiredRefs();
	r->push_back("passbuffer");
	return r;
}

PassBufferDefaultVal::~PassBufferDefaultVal() {
	//todo cleanup
}



void PassBufferDefaultVal::upload(GPUPointer* ptr) {

	//Simply upload stored Value
	ptr->map(mValue, getBufferSize());
	//Mark as clean (until next Pass happens)
	clean();
}


int PassBufferDefaultVal::getBufferSize() {
	//Min Size supported by GLSL
	return 2048;
}
void PassBufferDefaultVal::initData() {
	//initailize Buffer with 2kb
	this->mValue=new float[getBufferSize()];

}
void PassBufferDefaultVal::preparePassHash(
		const CompositorShadowNode* shadowNode, bool casterPass,
		bool dualParaboloid, SceneManager* sceneManager, Hlms* hlms) {


	//get screen
	RenderTarget *renderTarget =
			sceneManager->getCurrentViewport()->getTarget();

	//Get current
	Camera *camera = sceneManager->getCameraInProgress();

	//Get current view matrix
	Matrix4 viewMatrix = camera->getViewMatrix(true);

	//Get current Projection matrix
	Matrix4 projectionMatrix = camera->getProjectionMatrixWithRSDepth();
	Matrix4 identityProjMat;
	//Convert Matrix to correct Format
	mParentHLMS->mRenderSystem->_convertProjectionMatrix(Matrix4::IDENTITY, identityProjMat,
			true);

	//Flip Matrix if necessary
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

	//calculate viewproj matrix
	Matrix4 vpmat = projectionMatrix * viewMatrix;


	bool isShadowCastingPointLight = false;

	//Check if Cmaera is reflected
	const bool isCameraReflected = camera->isReflected();

	//Arbitrary 2kb (minimum supported by GL is 64kb), should be enough.
	const size_t maxBufferSize = 2 * 1024;



	//indexer for Buffer
	int ind=0;

	//upload viewprojmatrix
	for (size_t i = 0; i < 16; ++i)
		mValue[ind++] = (float) vpmat[0][i];

	//upload projmatrix
	for (size_t i = 0; i < 16; ++i)
		mValue[ind++] = (float) projectionMatrix[0][i];

	//upload viewmatrix
	for (size_t i = 0; i < 16; ++i)
		mValue[ind++] = (float) viewMatrix[0][i];



	//Upload clipping planes
	Real fNear, fFar;
	const Real depthRange = fFar - fNear;
	mValue[ind++] = fNear;
	mValue[ind++] = 1.0f / depthRange;
	mValue[ind++] = fFar;
	mValue[ind++] = depthRange;


	//set parent Matrixes (for Instance Buffer)
	mParentHLMS->mPreparedPass.viewProjMatrix[0] = projectionMatrix * viewMatrix;
	mParentHLMS->mPreparedPass.viewProjMatrix[1] = identityProjMat;


	//Upload render target dimenstion inverted, because multiplication on the GPU is cheaper than divison
	mValue[ind++] = 1.0f / (float) renderTarget->getWidth();
	mValue[ind++] = 1.0f / (float) renderTarget->getHeight();
	//Also upload normally just in case it is necessary
	mValue[ind++] = (float) renderTarget->getWidth();
	mValue[ind++] = (float) renderTarget->getHeight();



}

} /* namespace Ogre */


