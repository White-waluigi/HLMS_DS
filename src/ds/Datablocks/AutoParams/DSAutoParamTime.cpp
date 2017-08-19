/*
 * DSAutoParamTime.cpp
 *
 *  Created on: Aug 21, 2016
 *      Author: marvin
 */


#include <OgreHardwareVertexBuffer.h>
#include <OgreArchive.h>
#include <OgreTimer.h>
#include <OgreStringConverter.h>
#include <OgrePrerequisites.h>
#include <OgreHlms.h>
#include <OgreTimer.h>
#include <OgreException.h>
#include <OgreHlms.h>

#include "Datablocks/AutoParams/DSAutoParamTime.h"
#include "Datablocks/AutoParams/DSAutoParamParser.h"

namespace Ogre {

DSAutoParamTime::DSAutoParamTime() {
	timer=NULL;
}



void DSAutoParamTime::initialize(const HlmsParamVec& params) {
	String paramVal;
	this->type=VEC4;

	if (Hlms::findParamInVec(params, IdString("time"), paramVal)) {
		timeLenght=StringConverter::parseUnsignedInt(paramVal);
	}
	if (Hlms::findParamInVec(params, IdString("pulse"), paramVal)) {
		pulse=StringConverter::parseBool(paramVal);
	}
	timer=new Ogre::Timer();

	initData();

}
void Ogre::DSAutoParamTime::initialize(MT_MultiData* data) {
	//"type=time,time=2000,pulse=on"

	String paramVal;
	this->type=VEC4;

	if (data->has("time")) {
		timeLenght=(data->getData("time").f);
	}

	pulse=data->getBool("pulse");


	softpulse=data->getBool("softpulse");
	timer=new Ogre::Timer();

	initData();
}
DSAutoParamTime::~DSAutoParamTime() {
	// TODO Auto-generated destructor stub
}

void DSAutoParamTime::update() {
	if(!pulse){
		data[0]= float((timer->getMilliseconds()%timeLenght)/(timeLenght*1.0f));
	}else
	{
		data[0]=((timer->getMilliseconds()%timeLenght))/(timeLenght*1.0f);
		if((timer->getMilliseconds()%(timeLenght*2))>=(uint)(timeLenght)){
			data[0]=(timeLenght-(timer->getMilliseconds()%timeLenght))/(timeLenght*1.0f);
		}
	}

	if(softpulse){
		data[0]=sin(data[0]*Math::PI*2);
	}
	data[1]=1-data[0];
	data[2]=Math::Sqrt(1-pow(data[0],2));
	data[3]=1/data[0];
}


} /* namespace Ogre */

