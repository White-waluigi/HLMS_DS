/*
 * DSMBRotate.cpp
 *
 *  Created on: Jan 4, 2017
 *      Author: marvin
 */

#include "DSMBRotate.h"

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


	//Not done yet

}

} /* namespace Ogre */
