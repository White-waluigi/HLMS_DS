/*
 * DSMBFrameAnimation.cpp
 *
 *  Created on: Jan 4, 2017
 *      Author: marvin
 */

#include "DSMBFrameAnimation.h"

namespace Ogre {

DSMBFrameAnimation::DSMBFrameAnimation(DSMappingBlock * parent,MT_MultiData * params) : DSMBModulator(parent,params) {
	this->fps=params->getDataD("fps",24.0).f;
	this->numUV=(int)params->getDataD("numUV",1).f;
	this->pulse=params->getBool("pulse");

	this->timeFrame.push_back((1/(numUV*1.0))*this->fps);
}

DSMBFrameAnimation::~DSMBFrameAnimation() {
}

void DSMBFrameAnimation::modulate() {
	DSMBModulator::modulate();
	float final=elapsed[0]*numUV;
	parent->UVindexOffset=((int)final)%numUV;
}

} /* namespace Ogre */
