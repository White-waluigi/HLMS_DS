/*
 * Creator.h
 *
 *  Created on: Jan 21, 2018
 *      Author: marvin
 *
 * Template class used to construct Modules
 */

#ifndef SRC_DS_MODULES_CREATOR_H_
#define SRC_DS_MODULES_CREATOR_H_
#include "DSModule.h"
#include "ModuleReference.h"
#include "Ogre.h"

namespace Ogre {


//Simple baseclass for CreatorImpl, does nothing
class Creator {
public:
	Creator();
	virtual ~Creator();
	//Needs a list of references to create Module
	virtual DSModule* create(reflist l)=0;
};


template <class T>
class CreatorImpl : public Creator
{
public:

	//Create templated Module using list of references
   virtual DSModule* create(reflist l) {

	   //Every Module has a list of required references
	   refreq *res= T::getRequiredRefs();


	   //Check if every required ModuleReference is available
	   for (refreq::const_iterator iterator = res->begin(), end = res->end(); iterator != end; ++iterator) {



		   if(l.find(*iterator)==l.end()){
			   OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,"Module could not be constructed. Required Module Reference "+iterator->getFriendlyText()+" not found","   virtual DSModule* create(reflist l) {");
		   }
	   }

	   //Increase Link Counter
	   for (reflist::const_iterator iterator = l.begin(), end = l.end(); iterator != end; ++iterator) {
		   iterator->second->link();
	   }

	   //return newly constructed Module
	   return new T(l);

   }

};

} /* namespace Ogre */

#endif /* SRC_DS_MODULES_CREATOR_H_ */
