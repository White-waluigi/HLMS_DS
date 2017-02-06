/*
 * MTMultiData.cpp
 *
 *  Created on: Dec 16, 2016
 *      Author: marvin
 */

#include "MTMultiData.h"
#include <stdio.h>
#include <iostream>

namespace Ogre {

MT_MultiData::retValMD MT_MultiData::getData() {
	return getData(0);
}

MT_MultiData::retValMD MT_MultiData::getData(int i) {
	checksize(i + 1);
	if (type == MT_MultiData::sString) {
		return MT_MultiData::retValMD(str.at(i));
	}
	if (type == MT_MultiData::sFloat) {
		return MT_MultiData::retValMD(flt.at(i));
	}
	if (type == MT_MultiData::sObject) {
		int skip=0;
		for(std::map<Ogre::String,MT_MultiData *>::iterator iterator = obj.begin(); iterator != obj.end(); iterator++) {
			if(skip==i){
				return MT_MultiData::retValMD(iterator->second);
			}
			skip++;
		}


	}
}

MT_MultiData::retValMD MT_MultiData::getObject(Ogre::String allocator) {
	checksize(allocator);


	return retValMD(obj.find(allocator)->second);
	//return getSubData(allocator);
}

MT_MultiData::retValMD MT_MultiData::getData(Ogre::String allocator, int i) {
	checksize(allocator);
	return getObject(allocator).o->getData(i);
}

int MT_MultiData::size() {
	if (type == sFloat) {
		return flt.size();
	}
	if (type == sString) {
		return str.size();
	}
	if (type == sObject) {
		return obj.size();
	}
	throw new Exception(123, "No Elements in Object "+key,
			"MT_MultiData::size()");
}

MT_MultiData::retValMD MT_MultiData::getData(Ogre::String allocator) {
	MT_MultiData * test=getObject(allocator).o;
	assert(test!=NULL);
	return test->getData();

}

void MT_MultiData::checksize(int size) {
	if ((flt.size() < size && str.size() < size && obj.size() < size)
			|| size < 0) {
		throw new Exception(123, "Element does not exist in JSON Array in Object "+key,
				"MT_MultiData::checksize(int size)");
	}
}

int MT_MultiData::size(String s) {
	checksize(s);
	return obj.find(s)->second->size();

}

bool MT_MultiData::has(String has) {
	return (obj.find(has) != obj.end());
}

MT_MultiData::MT_MultiData() {
}
MT_MultiData::MT_MultiData(Ogre::String str) {
	key=str;
}
void MT_MultiData::print(int depth) {
	Ogre::String tab = "";
	for (int i = 0; i < depth; i++) {
		tab += "\t";
	}

	std::cout <<"\n"<< tab << key;


	if (type == sObject) {
		std::cout << "\n"<<tab<<"{";
		for(std::map<Ogre::String,MT_MultiData *>::iterator iterator = obj.begin(); iterator != obj.end(); iterator++) {
			iterator->second->print(depth+1);

		}

		std::cout <<"\n"<< tab<< "}";

	} else {
		std::cout << "\t:[";

		if (type == sFloat) {

			for (int i = 0; i < flt.size(); i++) {
				std::cout << flt.at(i) << (flt.size()==1?"":",");
			}

		}
		if (type == sString) {
			for (int i = 0; i < str.size(); i++) {
				std::cout <<"\"" <<str.at(i) <<"\""<<( str.size()==1?"":",");
			}
		}
		std::cout << "]";
	}

}

void MT_MultiData::Remove(Ogre::String s) {
	checksize(s);
	obj.erase(s);
}

void MT_MultiData::checksize(Ogre::String allocator) {
	if((obj.size()>0)&&(type!=sObject)){

		throw new Exception(123, "Element has wrong Datatype Array  (not sObject)",
			"void MT_MultiData::checksize(Ogre::String allocator)");
	}
	if(flt.size()>0&&type!=sFloat){

		throw new Exception(123, "Element has wrong Datatype Array  (not sFloat)",
			"void MT_MultiData::checksize(Ogre::String allocator)");
	}
	if(str.size()>0&&type!=sString){

		throw new Exception(123, "Element has wrong Datatype Array (not SString)",
			"void MT_MultiData::checksize(Ogre::String allocator)");
	}

	if (obj.find(allocator) == obj.end()) {
		throw new Exception(123, "Element " +allocator+" not found in Object "+key,
				"MT_MultiData::checksize(Ogre::String allocator)");
	}

}
MT_MultiData::retValMD* MT_MultiData::getDataArray(Ogre::String allocator) {
	retValMD* array=new retValMD[getObject(allocator).o->size()];

	for(int i=0;i<getObject(allocator).o->size();i++){
		array[i]=getData(allocator,i);
	}
	return array;
}
MT_MultiData::retValMD MT_MultiData::getDataArrayAsString(Ogre::String allocator) {

	Ogre::String retval="";
	for(int i=0;i<this->getObject(allocator).o->size();i++){
		retval+=this->getObject(allocator).o->getData(i).s;
		retval+="\n";
	}

	retval=Ogre::StringUtil::replaceAll(retval,"$[at]","@");

	return retval;
}
bool MT_MultiData::getBool(Ogre::String allocator) {
	if(has(allocator)){
		return getData(allocator).f==1.0;
	}
	return false;
}
MT_MultiData::retValMD MT_MultiData::getDataD(Ogre::String allocator, float f) {
	if(has(allocator)){
		return getData(allocator);
	}
	return retValMD(f);
}

MT_MultiData::retValMD MT_MultiData::getDataD(Ogre::String allocator,
		Ogre::String allocator1) {
	if(has(allocator)){
		return getData(allocator);
	}
	return retValMD(allocator1);

}
MT_MultiData::MT_MultiData(Ogre::String key, Ogre::String str) {
	this->key=key;
	this->str.push_back(str);
	s();
}

MT_MultiData::MT_MultiData(Ogre::String key, float f) {
	this->key=key;
	this->flt.push_back(f);
	s();
}

MT_MultiData::MT_MultiData(Ogre::String key, MT_MultiData* mt) {
	this->key=key;
	this->obj[mt->key]=(mt);
	s();
}
MT_MultiData::MT_MultiData(Ogre::String key, Ogre::String strvec, float def) {
	//Format:
	//"type=time,time=2000,pulse=on"
	this->key=key;
	String string = strvec;
	string.erase(remove(string.begin(), string.end(), '\''), string.end());


	vector<String>::type 				split1;
	vector<vector<String>::type>::type 	split2;

	split1=StringUtil::split(string,",");
	for(size_t i=0; i<split1.size();i++){
		Ogre::String s=split1.at(i);
		split2.push_back(StringUtil::split(s,"="));
	}
	for(size_t i=0; i<split2.size();i++){
		this->ao(split2.at(i).at(0),split2.at(i).at(1));

	}


}
void MT_MultiData::as(Ogre::String str) {
	this->str.push_back(str);
	s();
}

void MT_MultiData::af(float f) {
	this->flt.push_back(f);
	s();
}

void MT_MultiData::ao(MT_MultiData* mt) {
	this->obj[mt->key]=mt;
	s();
}
void MT_MultiData::s() {
	if(obj.size()!=0){
		this->type=sObject;
	}
	if(str.size()!=0){
		this->type=sString;
	}
	if(flt.size()!=0){
		this->type=sFloat;
	}
}
void MT_MultiData::ao(Ogre::String key, Ogre::String str) {
	ao(new MT_MultiData(key,str));
}

void MT_MultiData::ao(Ogre::String key, float flt) {
	ao(new MT_MultiData(key,flt));
}
bool MT_MultiData::isString() {
	return type== sString;
}

bool MT_MultiData::isFloat() {
	return type== sFloat;
}

bool MT_MultiData::isObject() {
	return type== sObject;
}


} /* namespace Ogre */

