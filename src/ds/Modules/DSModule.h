/*
 * DSModule.h
 *
 *  Created on: Jan 21, 2018
 *      Author: marvin
 *
 * Module System
 * =============
 *
 * HlmsDS contains a simple Module System to allow for
 * Users to simply extend its functionality with custom Modules
 * and the easily create string based parsing of Datablocks
 * Modules are used by Datablocks, Lightmanager, InstanceManager, PassBufferManager, JSON Parser and
 * ParamVecParser
 * They require an individually specified list of ModuleReferences to be initialized and are constructed using
 * templated constructor Objects
 */

#ifndef SRC_DS_MODULES_DSMODULE_H_
#define SRC_DS_MODULES_DSMODULE_H_

#include "Ogre.h"
#include "ModuleReference.h"


namespace Ogre {
//All lists are typedefed for readability
typedef std::map<Ogre::IdString, ModuleReference*> reflist;
//All lists are typedefed for readability
typedef std::list<IdString> refreq;

class DSModule : public ModuleReference{

	//list of ModuleReferences
	reflist mRefs;
public:
	//Sets mRefs
	DSModule(reflist refs);

	//return new required refernce list, used as basis by all subclasses
	static refreq* getRequiredRefs();

	//Get registered reference
	ModuleReference* getReference(IdString);

	//clean up DO NOT DELETE mRefs
	virtual ~DSModule();

};

} /* namespace Ogre */

#endif /* SRC_DS_MODULES_DSMODULE_H_ */
