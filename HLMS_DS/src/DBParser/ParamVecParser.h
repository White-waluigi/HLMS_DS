/*
 * ParamVecParser.h
 *
 *  Created on: Nov 17, 2016
 *      Author: marvin
 */

#ifndef DBPARSER_PARAMVECPARSER_H_
#define DBPARSER_PARAMVECPARSER_H_

#include "DBParser.h"
//Used to parse simple ParamVec Materials, very usefull for quick and simple test Materials, although a bit deprecated
namespace Ogre {
class DSMaterialParam;
class DSTextureParamType;
class ParamVecParser: public DBParser {
public:
	ParamVecParser();

	virtual void parse(HlmsManager *hlmsManager);
	void initParam(String paramVal, DSMaterialParam* param);
	virtual ~ParamVecParser();
	void initTextureUnit(String paramValD, DSTextureParamType * type,
			HlmsManager *hlmsManager) ;
};

} /* namespace Ogre */

#endif /* DBPARSER_PARAMVECPARSER_H_ */
