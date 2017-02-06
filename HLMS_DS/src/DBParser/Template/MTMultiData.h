/*
 * MTMultiData.h
 *
 *  Created on: Dec 16, 2016
 *      Author: marvin
 */

#ifndef SRC_DBPARSER_TEMPLATE_MTMULTIDATA_H_
#define SRC_DBPARSER_TEMPLATE_MTMULTIDATA_H_
#include "Ogre.h"
#include "exception"

namespace Ogre {

class MT_MultiData {
public:
	bool safe=true;
	Ogre::String key;

	std::vector<float> flt;
	std::vector<Ogre::String> str;
	std::map<Ogre::String,MT_MultiData *> obj;

	class MyException : public Exception {
	   const char * what () const throw () {
	      return "C++ Exception";
	   }
	};

	enum type {
		sFloat, sString, sObject
	} type = sFloat;
	struct retValMD{
		retValMD(){

		}

		float f=-1;
		Ogre::String s;
		MT_MultiData *o=NULL;
		retValMD(String s){
			this->s=s;
		}
		retValMD(float f){
			this->f=f;
		}
		retValMD(MT_MultiData *o){
			this->o=o;
		}
	};

	MT_MultiData();
	MT_MultiData(Ogre::String key);
	MT_MultiData(Ogre::String key,Ogre::String str);
	MT_MultiData(Ogre::String key,float f);
	MT_MultiData(Ogre::String key,MT_MultiData * mt);
	MT_MultiData(Ogre::String key,Ogre::String strvec,float def);

	int size();
	int size(String s);
	retValMD getData();
	retValMD getData(int i);
	retValMD getObject(Ogre::String);
	retValMD *getDataArray(Ogre::String);

	retValMD getData(Ogre::String);
	retValMD getDataArrayAsString(Ogre::String);
	retValMD getData(Ogre::String,int i);

	retValMD getDataD(Ogre::String,float f);
	retValMD getDataD(Ogre::String,Ogre::String);

	void as(Ogre::String str);
	void af(float f);
	void ao(MT_MultiData *mt);
	void ao(Ogre::String key,Ogre::String str);
	void ao(Ogre::String key,float flt);


	bool getBool(Ogre::String);
	void s();

	void Remove(Ogre::String s);

	void print(int depth);

	bool has(String has);
	void checksize(int size);
	void checksize(Ogre::String);

	bool isString();
	bool isFloat();
	bool isObject();

};

} /* namespace Ogre */

#endif /* SRC_DBPARSER_TEMPLATE_MTMULTIDATA_H_ */
