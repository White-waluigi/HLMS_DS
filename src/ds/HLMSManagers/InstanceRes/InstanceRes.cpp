/*
 * InstanceRes.cpp
 *
 *  Created on: Mar 22, 2018
 *      Author: marvin
 */

#include "InstanceRes.h"
namespace Ogre{

InstanceRes::InstanceRes(reflist r) :DSModule(r){

	//Needs an instance Manager to work
	this->mParentIMgr=dynamic_cast<InstanceManager*> ( getReference("instanceMgr"));

}

refreq* InstanceRes::getRequiredRefs() {
	refreq *r=DSModule::getRequiredRefs();

	//Backlink to the Hlms System
	r->push_back("hlms");
	//Mandatory init data needed for working Resource
	r->push_back("instanceMgr");
	return r;
}

}
