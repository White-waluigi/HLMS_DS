/*
 * ModuleReference.h
 *
 *  Created on: Jan 21, 2018
 *      Author: marvin
 *
 *  Module References are used by Modules as a context connection to the rest of the system
 *  and their local environment (Mangers, Datablocks).
 *  This is mostly a baseclass used for save typing
 */

#ifndef SRC_DS_MODULES_MODULEREFERENCE_H_
#define SRC_DS_MODULES_MODULEREFERENCE_H_

namespace Ogre {

class ModuleReference {
	//Keeping track of how many Modules link to this reference
	int mLinks=0;
public:
	//Increase link counter
	void link();
	//Does nothing
	ModuleReference();
	//Does nothing;
	virtual ~ModuleReference();
};

} /* namespace Ogre */

#endif /* SRC_DS_MODULES_MODULEREFERENCE_H_ */
