/*
 * ModuleBroker.h
 *
 *  Created on: Jan 21, 2018
 *      Author: marvin
 *
 *  Manages registered Modules and creates them,
 *  can be extended by the User
 */

#ifndef SRC_DS_MODULES_MODULEBROKER_H_
#define SRC_DS_MODULES_MODULEBROKER_H_
#include "Ogre.h"
#include "OgreIdString.h"
#include "DSModule.h"
#include "Creator.h"


namespace Ogre {

class ModuleBroker {

	//All lists are typedefed for convenience
	typedef std::map<Ogre::IdString ,Creator *> ModuleCreatorList;


	//List of Module Creators
	ModuleCreatorList mModuleCrts;



	public:
	//register all default Modules
	ModuleBroker();

	//register custom or runtime Module
	void registerModule(IdString name, Creator * crt );

	//register all Modules necessary for a full Deferred Shading Pipeline
	void registerDefaultModules();

	//Load Module using its name and
	DSModule *  loadModule(IdString name, 	reflist params);

	//Clean up, delete all Creators
	virtual ~ModuleBroker();
};

} /* namespace Ogre */

#endif /* SRC_DS_MODULES_MODULEBROKER_H_ */
