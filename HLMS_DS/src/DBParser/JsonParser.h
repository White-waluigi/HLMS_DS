/*
 * JsonParser.h
 *
 *  Created on: Nov 17, 2016
 *      Author: marvin
 */

#ifndef DBPARSER_JSONPARSER_H_
#define DBPARSER_JSONPARSER_H_

#include "DBParser.h"
#include "OgreHlmsJson.h"
#include "OgreHeaderPrefix.h"
#include "OgreHlmsJson.h"
#include "OgreHeaderPrefix.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "Template/TemplateParams.h"
#include "Template/MTMultiData.h"
#include "Template/MaterialTemplate.h"

#include "../Datablocks/DSDatablock.h"
namespace Ogre {
class DSDatablock;
class JSONMaterialCreator;
class JsonParser: public DBParser {
public:
	class QueueItem{
	public:
		MaterialTemplate * templ;
		DSDatablock* datablock;
		HlmsJson::NamedBlocks blocks;
	};

	std::map<Ogre::IdString,QueueItem *> QueuedTemplates;
	std::map<Ogre::IdString,QueueItem *> MergedTemplates;

	rapidjson::Document json;
	rapidjson::Value jsonv;
	HlmsJson::NamedBlocks blocks;
	HlmsManager *mHlmsManager;
	HlmsDS * parent;

	DSDatablock* db;



	virtual void parse(HlmsManager *hlmsManager);
	void loadMaterial(const rapidjson::Value &json,
			const HlmsJson::NamedBlocks &blocks, HlmsDatablock *datablock);
	void saveMaterial(const HlmsDatablock *datablock, String &outString);

	static void collectSamplerblocks(const HlmsDatablock *datablock,
			set<const HlmsSamplerblock*>::type &outSamplerblocks);

	void setData( rapidjson::Value json,	HlmsJson::NamedBlocks blocks) {
		//this->json=json;
		this->blocks=blocks;
	}
	void printValue(const rapidjson::Value &jsonValue,int indent);
	void createMaterial(MaterialTemplate *,QueueItem* item,const HlmsJson::NamedBlocks& blocks);

	JsonParser(HlmsManager *mHlmsManager,HlmsDS * ds);
	virtual ~JsonParser();
};

} /* namespace Ogre */

#endif /* DBPARSER_JSONPARSER_H_ */
