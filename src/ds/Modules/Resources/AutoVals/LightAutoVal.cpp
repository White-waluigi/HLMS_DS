/*
 * LightAutoParams.cpp
 *
 *  Created on: Feb 15, 2018
 *      Author: marvin
 */

#include "../AutoVals/LightAutoVal.h"

namespace Ogre {

refreq* LightAutoVal::getRequiredRefs() {
	refreq * r = Value::getRequiredRefs();
	r->push_back("dsLight");
	return r;
}

LightAutoVal::LightAutoVal(reflist l) :
		Value(l) {

}

LightAutoVal::~LightAutoVal() {
	// TODO Auto-generated destructor stub
}

void LightAutoVal::upload(GPUPointer* ptr) {
	assert(ptr);
}

int LightAutoVal::getBufferSize() {
}

} /* namespace Ogre */
