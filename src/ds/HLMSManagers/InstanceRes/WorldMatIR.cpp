/*
 * WorldMapIR.cpp
 *
 *  Created on: Mar 22, 2018
 *      Author: marvin
 */

#include "WorldMatIR.h"

#include "../../OgreHlmsDS.h"

namespace Ogre {



WorldMatIR::~WorldMatIR() {

}
int Ogre::WorldMatIR::getSize() {
	//Size of 4*4 float matrix
	return 16;
}

WorldMatIR::WorldMatIR(reflist r):InstanceRes(r) {

}


void WorldMatIR::upload(const QueuedRenderable& queuedRenderable,GPUPointer* data) {
	//Get The Orientation and Position of the Object
	const Matrix4 &worldMat =
			queuedRenderable.movableObject->_getParentNodeFullTransform();

	//Create WorldViewProjection Matrix to be uploaded
	Matrix4 tmp =mParentIMgr->getParent()
			->getPreparedPass()
			.viewProjMatrix[0] * worldMat;

	//upload said data
	data->map(tmp);



}

} /* namespace Ogre */

