/*
 * DBParser.h
 *
 *  Created on: Nov 17, 2016
 *      Author: marvin
 */
#if !OGRE_NO_JSON
#ifndef DBPARSER_DBPARSER_H_
#define DBPARSER_DBPARSER_H_
#include <OgreStableHeaders.h>
#include <OgreArchive.h>
#include <math.h>
#include <OgreHlmsJson.h>
#include <OgreHlmsDatablock.h>

namespace Ogre {

class DBParser{
public:

	Ogre::HlmsParamVec params;
	HlmsDatablock *hdb;

	virtual void parse(HlmsManager *hlmsManager);
	DBParser();
	virtual ~DBParser();

	Ogre::HlmsParamVec getData() {
		return params;
	}

	void setData(const HlmsParamVec datas,HlmsDatablock * db) {
		params = datas;
		this->hdb=db;
	}
};

} /* namespace Ogre */

#endif /* DBPARSER_DBPARSER_H_ */
#endif
