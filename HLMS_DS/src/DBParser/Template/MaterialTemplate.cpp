/*
 * MaterialTemplate.cpp
 *
 *  Created on: Dec 11, 2016
 *      Author: marvin
 */

#include "MaterialTemplate.h"
#include "MTMultiData.h"

namespace Ogre {

MaterialTemplate::MaterialTemplate(const rapidjson::Value& json) {
	createTemplate(json);
}

bool MaterialTemplate::Merge(MaterialTemplate *templ) {
	Merge(templ->data, this->data);

}

void MaterialTemplate::createTemplate(const rapidjson::Value& json) {

	this->data = addToList(json, 0, "main", this->data);

}

MT_MultiData * MaterialTemplate::addToList(const rapidjson::Value& json,
		int depth, Ogre::String key, MT_MultiData * existing) {
	if (depth > 999) {
		throw new Ogre::Exception(10, "JSON depth too high",
				"MT_MultiData * MaterialTemplate::addToList(const rapidjson::Value& json,int depth)");
	}
	MT_MultiData * data = existing == NULL ? new MT_MultiData() : existing;
	assert(data!=NULL);

	data->key = key;
	int size = json.IsArray() ? json.Size() : 1;

	if (json.IsArray()) {
		for (int i = 0; i < json.Size(); i++) {

			if (json[i].IsString()) {
				if(i==0){data->str.clear();}
				this->addToListS(json[i], data);
			}
			if (json[i].IsBool() || json[i].IsNumber()) {
				if(i==0){data->flt.clear();}
				this->addToListF(json[i], data);

			}
			if (json[i].IsObject()) {
				for (rapidjson::Value::ConstMemberIterator itr =
						json[i].MemberBegin(); itr != json[i].MemberEnd(); ++itr) {

					addObjectToList(data, itr, depth);

				}
			}

		}
	}
	if (json.IsString()) {
		data->str.clear();
		this->addToListS(json, data);
	}
	if (json.IsBool() || json.IsNumber()) {
		data->flt.clear();
		this->addToListF(json, data);

	}
	if (json.IsObject()) {
		for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin();
				itr != json.MemberEnd(); ++itr) {

			addObjectToList(data, itr, depth);

		}
	}

	return data;
}

void MaterialTemplate::addToListF(const rapidjson::Value& json,
		MT_MultiData * data) {

	data->type = MT_MultiData::sFloat;

	float flt = 0;
	if (json.IsBool()) {
		flt = json.GetBool();
	} else {
		flt = json.GetDouble();
	}

	data->flt.push_back(flt);

}

void MaterialTemplate::addToListS(const rapidjson::Value& json,
		MT_MultiData * data) {
	data->type = MT_MultiData::sString;
	Ogre::String str;

	str = json.GetString();

	data->str.push_back(str);
}

void MaterialTemplate::addToListO(const rapidjson::Value& json,
		rapidjson::Value::ConstMemberIterator itr, int depth) {
	addObjectToList(data, itr, depth);
}
void MaterialTemplate::addObjectToList(MT_MultiData * data,
		rapidjson::Value::ConstMemberIterator itr, int depth) {

	data->type = MT_MultiData::sObject;
	MT_MultiData * existing = NULL;
	if (data->obj.find(itr->name.GetString()) == data->obj.end()) {
		existing = data->obj[itr->name.GetString()];
	}

	data->obj[itr->name.GetString()] = addToList(itr->value, depth + 1,
			itr->name.GetString(), existing);
}

bool MaterialTemplate::Merge(MT_MultiData* mtf, MT_MultiData* mtt) {
	mtt->key = mtf->key;
	MergeFlt(mtf, mtt);
	MergeStr(mtf, mtt);
	MergeObj(mtf, mtt);
}

void MaterialTemplate::RemoveMeta() {
	data->Remove("meta");
}

MaterialTemplate::~MaterialTemplate() {
	// TODO Auto-generated destructor stub
}
MaterialTemplate::MaterialTemplate() {
}

MaterialTemplate::MaterialTemplate(MaterialTemplate* t) {
	this->data = new MT_MultiData();
	Merge(t);
}
bool MaterialTemplate::MergeFlt(MT_MultiData* mtf, MT_MultiData* mtt) {
	mtt->flt.clear();

	for (int i = 0; i < mtf->flt.size(); i++) {
		mtt->type = MT_MultiData::sFloat;
		//you have to overwrite string and float arrays!
		mtt->flt.push_back(mtf->flt.at(i));
	}
}

bool MaterialTemplate::MergeStr(MT_MultiData* mtf, MT_MultiData* mtt) {
	mtt->str.clear();

	for (int i = 0; i < mtf->str.size(); i++) {
		mtt->type = MT_MultiData::sString;
		//you have to overwrite string and float arrays!
		mtt->str.push_back(mtf->str.at(i));
	}
}

bool MaterialTemplate::MergeObj(MT_MultiData* mtf, MT_MultiData* mtt) {
	mtt->key=mtf->key;
	for (std::map<Ogre::String, MT_MultiData *>::iterator iterator =
			mtf->obj.begin(); iterator != mtf->obj.end(); iterator++) {
		mtt->type = MT_MultiData::sObject;

		if (mtt->obj.find(iterator->first) == mtt->obj.end()) {

			mtt->obj[iterator->first] = new MT_MultiData();

			Merge(iterator->second, mtt->obj[iterator->first]);
		} else {

			Merge(iterator->second, mtt->obj[iterator->first]);
		}

	}
}

} /* namespace Ogre */

