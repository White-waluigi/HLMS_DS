/*
 * PassBufferSlot.h
 *
 *  Created on: Apr 16, 2017
 *      Author: marvin
 */

#ifndef SRC_HLMSHELPER_PASSBUFFER_PASSBUFFERSLOT_H_
#define SRC_HLMSHELPER_PASSBUFFER_PASSBUFFERSLOT_H_

#include <Ogre.h>

namespace Ogre {
class PassBuffer;


class PassBufferSlot {
public:
	Ogre::String type;

	PassBuffer * parent;



	int size;

	int getSize();
	int WhatSize();

	void bindData();

	PassBufferSlot(Ogre::String,	PassBuffer * );
	virtual ~PassBufferSlot();
};

} /* namespace Ogre */

#endif /* SRC_HLMSHELPER_PASSBUFFER_PASSBUFFERSLOT_H_ */
