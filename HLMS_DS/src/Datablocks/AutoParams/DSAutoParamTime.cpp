/*
 * DSAutoParamTime.cpp
 *
 *  Created on: Aug 21, 2016
 *      Author: marvin
 */

#include "../../Datablocks/AutoParams/DSAutoParamTime.h"
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

DSAutoParamTime::DSAutoParamTime() {
	// TODO Auto-generated constructor stub

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

DSAutoParamTime::~DSAutoParamTime() {
	// TODO Auto-generated destructor stub
}

void DSAutoParamTime::update() {
	if(!pulse){
		data[0]= float((timer->getMilliseconds()%timeLenght)/(timeLenght*1.0f));
	}else
	{
		data[0]=((timer->getMilliseconds()%timeLenght))/(timeLenght*1.0f);
		if((timer->getMilliseconds()%(timeLenght*2))>=(timeLenght)){
			data[0]=(timeLenght-(timer->getMilliseconds()%timeLenght))/(timeLenght*1.0f);
		}
	}
	data[1]=0.0;
	data[2]=1.0;
	data[3]=1/data[0];
}

} /* namespace Ogre */
