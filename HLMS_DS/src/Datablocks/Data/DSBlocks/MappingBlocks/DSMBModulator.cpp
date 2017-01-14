/*
 * DSMBModulator.cpp
 *
 *  Created on: Jan 4, 2017
 *      Author: marvin
 */

#include "DSMBModulator.h"

namespace Ogre {

DSMBModulator::DSMBModulator(DSMappingBlock * parent,MT_MultiData * params) {
	this->timeScale= params->getDataD("timescale",1.0).f;
	this->parent=parent;
	this->name=params->key;
}


void DSMBModulator::modulate() {

	float sec=1000*1000;
	for(int i=0;i<timeFrame.size();i++){
		float ftime=(parent->getTimer().getMicroseconds()/sec)*this->timeFrame[i];

		float time=fmod( ftime ,this->timeScale);

		if(pulse){


			if(fmod(ftime,2)>=(1)){
				time=1-time;
			}
		}
		float el=time/this->timeScale;
		if(elapsed.size()<=i){
			elapsed.push_back(el);
		}else{
			elapsed[i]=el;
		}
	}

}
DSMBModulator::~DSMBModulator() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */
