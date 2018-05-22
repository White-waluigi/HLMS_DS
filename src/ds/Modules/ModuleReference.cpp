/*
 * ModuleReference.cpp
 *
 *  Created on: Jan 21, 2018
 *      Author: marvin
 */

#include "ModuleReference.h"

namespace Ogre {

ModuleReference::ModuleReference() {
	//Nothing necessary
}

ModuleReference::~ModuleReference() {
	//Nothing necessary
}

} /* namespace Ogre */

void Ogre::ModuleReference::link() {
	//count amount of Modules using this Reference
	mLinks++;
}
