/*
 * WorldMapIR.h
 *
 *  Created on: Mar 22, 2018
 *      Author: marvin
 *      
 *  Uploads the Position, Scale and Rotation of
 *  a given Renderable. 
 */

#ifndef SRC_DS_HLMSMANAGERS_INSTANCERES_WORLDMATIR_H_
#define SRC_DS_HLMSMANAGERS_INSTANCERES_WORLDMATIR_H_

#include "InstanceRes.h"

namespace Ogre {

class WorldMatIR: public InstanceRes {
public:

	//Uploads 16 bytes for World Matrix
	virtual int getSize();
	
	//Does nothing
	WorldMatIR(reflist r);

	//Does nothing
	virtual ~WorldMatIR();

	//Upload World Matrix to Instance Buffer
	void upload(const QueuedRenderable& queuedRenderable,GPUPointer* data);
};

} /* namespace Ogre */

#endif /* SRC_DS_HLMSMANAGERS_INSTANCERES_WORLDMATIR_H_ */
