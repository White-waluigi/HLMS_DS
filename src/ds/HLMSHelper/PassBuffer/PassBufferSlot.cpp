/*
 * PassBufferSlot.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: marvin
 */

#include "HLMSHelper/PassBuffer/PassBufferSlot.h"
#include "HLMSHelper/PassBuffer/PassBuffer.h"
#include "OgreHlmsDS.h"
#include "Helper/DSMath.h"

namespace Ogre {

Ogre::PassBufferSlot::PassBufferSlot(Ogre::String idString, PassBuffer* p) {
	parent = p;
	this->type = idString;
	this->size = WhatSize();

}

int PassBufferSlot::getSize() {
	return size;
}
int PassBufferSlot::WhatSize() {
	return parent->defmap[type];
}
PassBufferSlot::~PassBufferSlot() {
	// TODO Auto-generated destructor stub
}
void PassBufferSlot::bindData() {
	float flip = -1;
	if (parent->parent->requiresTextureFlipping) {
		flip = -1;
	}
	Matrix4 tmp;
	if (type.compare("debug") == 0) {
		//******************Debug Modes**********************************************
		uint dm = static_cast<int>(parent->parent->debugMode);
		uint dm2 = static_cast<int>(parent->parent->fullBrightMode);
		uint dm3 = static_cast<int>(parent->parent->Debugz);
		uint dm4 = static_cast<int>(parent->parent->Debugw);

		float fdm = reinterpret_cast<float &>(dm);
		float fdm2 = reinterpret_cast<float &>(dm2);
		float fdm3 = reinterpret_cast<float &>(dm3);
		float fdm4 = reinterpret_cast<float &>(dm4);

		parent->addData((fdm));
		parent->addData((fdm2));
		parent->addData(fdm3);
		parent->addData(fdm4);
	} else if (type.compare("proj") == 0) {
		//**********************View Projection Matrix******************************************
		Matrix4 tmp = parent->parent->mPreparedPass.projMatrix.transpose();



		for (size_t i = 0; i < 16; ++i)
			parent->addData((float) tmp[0][i]);

	} else if (type.compare("view") == 0) {
		//**********************View Matrix******************************************
		Matrix4 tview = parent->parent->mPreparedPass.viewMatrix;
		tmp = tview.transpose();

		for (size_t i = 0; i < 16; ++i)
			parent->addData((float) tmp[0][i]);
	} else if (type.compare("viewproj") == 0) {
		Matrix4 itview =
				parent->parent->mPreparedPass.projMatrix*parent->parent->mPreparedPass.viewMatrix;


		if (parent->parent->roundPVMat) {
//
//			itview=DSMath::roundPVmat(1024,1024,itview);
//
		}

		tmp = itview.transpose();



		for (size_t i = 0; i < 16; ++i)
			parent->addData((float) tmp[0][i]);


	} else if (type.compare("invView") == 0) {
		Matrix4 itview =
				parent->parent->mPreparedPass.viewMatrix.inverseAffine();
		tmp = itview.transpose();

		for (size_t i = 0; i < 16; ++i)
			parent->addData((float) tmp[0][i]);


	} else if (type.compare("timer") == 0) {
		float timer1 = (parent->parent->timer->getMicroseconds() % 1000000)
				/ 1000000.0;
		float timer2 = (parent->parent->timer->getMicroseconds() % 10000000)
				/ 10000000.0;

		float timer3 = ((parent->parent->timer->getMicroseconds() % 1000000))
				/ 1000000.0;
		if ((parent->parent->timer->getMicroseconds() % 2000000) > 1000000) {
			timer3 = (1000000
					- (parent->parent->timer->getMicroseconds() % 1000000))
					/ 1000000.0;
		}

		float timer4 = ((parent->parent->timer->getMicroseconds() % 3000000))
				/ 3000000.0;
		if ((parent->parent->timer->getMicroseconds() % 6000000) > 3000000) {
			timer4 = (3000000
					- (parent->parent->timer->getMicroseconds() % 3000000))
					/ 3000000.0;
		}

		parent->addData(timer1);
		parent->addData(timer2);
		parent->addData(timer3);
		parent->addData(timer4);
	} else if (type.compare("clipping") == 0) {
		parent->addData(parent->curcam->getNearClipDistance());
		parent->addData(parent->curcam->getFarClipDistance());
		parent->addData(parent->curcam->getLastViewport()->getActualWidth());
		parent->addData(parent->curcam->getLastViewport()->getActualHeight());

		Vector3 wspc = parent->curcam->getWorldSpaceCorners()[4];

		parent->addData((parent->curcam->getViewMatrix(true) * (wspc)).x);
		parent->addData(
				(parent->curcam->getViewMatrix(true) * (wspc)).y * flip);

		parent->addData((parent->curcam->getViewMatrix(true) * wspc).z);
		parent->addData(0);
	} else if (type.compare("textureflip") == 0) {
		Vector4 test = parent->parent->mPreparedPass.viewMatrix.transformAffine(
				Vector4(1, 0, 0, 0));

		parent->addData(parent->parent->mPreparedPass.viewMatrix[0][0]);
		parent->addData(parent->parent->mPreparedPass.viewMatrix[0][1]);
		parent->addData(parent->parent->mPreparedPass.viewMatrix[0][2]);
		parent->addData(parent->parent->mPreparedPass.viewMatrix[0][3]);

	} else if (type.compare("camera") == 0) {
		//vec2 depthRange;

		Real fNear, fFar;

		fFar = parent->curcam->getFarClipDistance();
		fNear = parent->curcam->getNearClipDistance();

		const Real depthRange = fFar - fNear;

		parent->addData(fNear);
		parent->addData(fFar);
		parent->addData(1.0f / depthRange);
		parent->addData(depthRange);
	} else if (type.compare("test") == 0) {
		parent->addData(parent->parent->testdata.x);
		parent->addData(parent->parent->testdata.y);
		parent->addData(parent->parent->testdata.z);
		parent->addData(parent->parent->testdata.w);
	}

}

} /* namespace Ogre */

