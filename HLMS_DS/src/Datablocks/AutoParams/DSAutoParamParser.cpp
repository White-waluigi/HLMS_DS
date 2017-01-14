/*
 * DSAutoParamParser.cpp
 *
 *  Created on: 01.04.2016
 *      Author: marvin
 */
#include "OgrePrerequisites.h"

#include "../Data/DSMaterialParam.h"
#include "DSAutoParamParser.h"
#include "DSAutoParamTime.h"
#include "OgreTimer.h"
#include "OgreHardwareVertexBuffer.h"
#include "OgreException.h"
#include "OgreHlms.h"
#include "../../Light/StringValueUtils.h"
namespace Ogre {

DSAutoParamParser::DSAutoParamParser() {
	// TODO Auto-generated constructor stub

}

DSAutoParamParser::~DSAutoParamParser() {
	// TODO Auto-generated destructor stub
}

DSMaterialParam *DSAutoParamParser::getAutoParam(Ogre::String string) {
	HlmsParamVec vector=StringValueUtils::stringToParamVec(string);
	Ogre::String paramVal;

	DSMaterialParam * retval=NULL;


	if (Hlms::findParamInVec(vector,IdString("type"), paramVal)) {


		if(paramVal.compare( DSAutoParamTime::getParamName())==0){
			//HlmsParamVec vector=stringToParamVec(paramVal);
			retval=new  DSAutoParamTime();
			retval->initialize(vector);
		}


	}


	return retval;
}

} /* namespace Ogre */
