/*
 * DSMBRotate.cpp
 *
 *  Created on: Jan 4, 2017
 *      Author: marvin
 */

#include "Datablocks/Data/DSBlocks/MappingBlocks/Modulators/DSMBRotate.h"

namespace Ogre {

DSMBRotate::DSMBRotate(DSMappingBlock * parent,MT_MultiData * params) : DSMBModulator(parent,params) {
	this->speed=params->getDataD("speed",1.0).f;
	this->center.x=params->getDataD("centerx",1.0).f;
	this->center.y=params->getDataD("centery",1.0).f;
	this->timeFrame.push_back( speed);

}

DSMBRotate::~DSMBRotate() {
}

void DSMBRotate::modulate() {
	DSMBModulator::modulate();

	Ogre::Quaternion qt;
	Radian a=Radian(elapsed[0]*Ogre::Math::PI*2);
	const Ogre::Vector3 b=Ogre::Vector3(0,1,1);

	qt.FromAngleAxis(a,b);


	parent->matOffset.makeTransform(Vector3(center.x,center.y,0),Vector3::UNIT_SCALE,qt);
	parent->matOffset.setTrans(Vector3(center.x,center.y,0));

	//Not done yet

}

} /* namespace Ogre */
