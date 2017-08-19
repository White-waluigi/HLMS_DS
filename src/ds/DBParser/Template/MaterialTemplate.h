/*
 * MaterialTemplate.h
 *
 *  Created on: Dec 11, 2016
 *      Author: marvin
 */

#ifndef SRC_DBPARSER_TEMPLATE_MATERIALTEMPLATE_H_
#define SRC_DBPARSER_TEMPLATE_MATERIALTEMPLATE_H_
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>

#include "DBParser/Template/MTMultiData.h"

namespace Ogre {

class MaterialTemplate {
public:
	MaterialTemplate();
	MaterialTemplate(MaterialTemplate *);


	MT_MultiData *  data=NULL;

	void createTemplate(const rapidjson::Value& json);
	MT_MultiData * addToList(const rapidjson::Value& json, int depth, Ogre::String key,MT_MultiData * existing);

	void addToListF(const rapidjson::Value& json,MT_MultiData * data);
	void addToListS(const rapidjson::Value& json,MT_MultiData * data);
	void addToListO(const rapidjson::Value& json,rapidjson::Value::ConstMemberIterator itr,int depth);


	void addObjectToList(MT_MultiData * data ,rapidjson::Value::ConstMemberIterator itr,int depth);
	MaterialTemplate(const rapidjson::Value& json);

	void RemoveMeta();

	bool Merge(MaterialTemplate* templ);
	bool Merge(MT_MultiData *mtf,MT_MultiData *mtt);
	bool MergeFlt(MT_MultiData *mtf,MT_MultiData *mtt);
	bool MergeStr(MT_MultiData *mtf,MT_MultiData *mtt);
	bool MergeObj(MT_MultiData *mtf,MT_MultiData *mtt);



	virtual ~MaterialTemplate();
};

} /* namespace Ogre */

#endif /* SRC_DBPARSER_TEMPLATE_MATERIALTEMPLATE_H_ */
