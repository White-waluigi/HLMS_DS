/*
 * DSAutoTime.cpp
 *
 *  Created on: 01.04.2016
 *      Author: marvin
 */
#include "OgreHardwareVertexBuffer.h"
#include "OgreArchive.h"
#include "DSAutoTime.h"
#include "OgreTimer.h"
#include "OgreStringConverter.h"
#include "OgrePrerequisites.h"
#include "OgreHlms.h"
#include "DSAutoParamParser.h"
#include "DSAutoTime.h"
#include "OgreTimer.h"
#include "OgreException.h"
#include "OgreHlms.h"

namespace Ogre {

DSAutoTime::DSAutoTime() {
	type=_float;
}

DSAutoTime::~DSAutoTime() {
}

void DSAutoTime::update() {
	if(!pulse){
		data[0]= float((timer->getMilliseconds()%timeLenght)/(timeLenght*1.0f));
	}else
	{
		data[0]=((timer->getMilliseconds()%timeLenght))/(timeLenght*1.0f);
		if((timer->getMilliseconds()%(timeLenght*2))>=(timeLenght)){
			data[0]=(timeLenght-(timer->getMilliseconds()%timeLenght))/(timeLenght*1.0f);
		}
	}
}

void DSAutoTime::initialize(const HlmsParamVec &params) {

	String paramVal;
	if (Hlms::findParamInVec(params, IdString("time"), paramVal)) {
		timeLenght=StringConverter::parseUnsignedInt(paramVal);
	}
	if (Hlms::findParamInVec(params, IdString("pulse"), paramVal)) {
		pulse=StringConverter::parseBool(paramVal);
	}
	timer=new Ogre::Timer();
}

} /* namespace Ogre */
