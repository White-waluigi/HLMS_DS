/*
 * DSModule.cpp
 *
 *  Created on: Jan 21, 2018
 *      Author: marvin
 */

#include "DSModule.h"

namespace Ogre {

DSModule::DSModule(std::map<Ogre::IdString, ModuleReference*> refs) {
	this->mRefs=refs;
}

DSModule::~DSModule() {
	// TODO Auto-generated destructor stub
}













refreq* Ogre::DSModule::getRequiredRefs() {
	return new refreq();
}

ModuleReference* DSModule::getReference(IdString str) {
	return mRefs[str];
}
bool checkRefs(){

}

} /* namespace Ogre */

