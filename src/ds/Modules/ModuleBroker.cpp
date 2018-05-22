/*
 * ModuleBroker.cpp
 *
 *  Created on: Jan 21, 2018
 *      Author: marvin
 */

#include "ModuleBroker.h"
#include "Resources/Piece.h"
#include "Resources/Property.h"
#include "Resources/DSTexture.h"
#include "../Datablock/Parsers/Parser.h"
#include "../HLMSManagers/InstanceRes/WorldMatIR.h"
#include "Resources/AutoVals/PassBufferDefaultVal.h"
#include "Resources/AutoVals/LightAutoVal.h"

namespace Ogre {

ModuleBroker::ModuleBroker() {
	registerDefaultModules();
}



void ModuleBroker::registerModule(IdString name, Creator* crt) {

	this->mModuleCrts[name]=(crt);
}

void ModuleBroker::registerDefaultModules() {

	//Dabablock Resources
	registerModule("Value",new CreatorImpl<Value>());
	registerModule("Property",new CreatorImpl<Property>());
	registerModule("Piece",new CreatorImpl<Piece>());
	registerModule("DSTexture",new CreatorImpl<DSTexture>());


	//Mappingblocks

	//Mappingblocks UVModulators

	//AutoVals
	registerModule("PassBufferDefaultVal",new CreatorImpl<PassBufferDefaultVal>());
	registerModule("LightAutoVal",new CreatorImpl<LightAutoVal>());

	//Autotextures


	//Instance Resources
	registerModule("WorldMapIR",new CreatorImpl<WorldMatIR>());

	//Parsers
}

DSModule *  ModuleBroker::loadModule(IdString name,
		reflist params) {
	if(this->mModuleCrts.find(name)==this->mModuleCrts.end()){
		OGRE_EXCEPT(Exception::ERR_FILE_NOT_FOUND,"Module "+name.getFriendlyText()+" not found","DSModule *  ModuleBroker::loadModule(IdString name,reflist params) {");
	}
	return this->mModuleCrts[name]->create(params);
}

ModuleBroker::~ModuleBroker() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */
