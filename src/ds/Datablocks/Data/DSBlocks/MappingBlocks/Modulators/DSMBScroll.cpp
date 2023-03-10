/*
 * DSMBScroll.cpp
 *
 *  Created on: Jan 4, 2017
 *      Author: marvin
 */

#include "Datablocks/Data/DSBlocks/MappingBlocks/Modulators/DSMBScroll.h"

namespace Ogre {

DSMBScroll::DSMBScroll(DSMappingBlock * parent,MT_MultiData * params): DSMBModulator(parent,params) {
	this->speed.x=params->getDataD("speedx",1.0).f;
	this->speed.y=params->getDataD("speedy",1.0).f;

	this->timeFrame.push_back(this->speed.x);
	this->timeFrame.push_back(this->speed.y);
}

DSMBScroll::~DSMBScroll() {
}

void DSMBScroll::modulate() {
	DSMBModulator::modulate();
	Vector2 v=Vector2::UNIT_SCALE;
	v.x=elapsed[0];
	v.y=elapsed[1];

	parent->matOffset.makeTrans(v.x,v.y,1.0);
}

} /* namespace Ogre */
