/*
 * StringValueUtils.cpp
 *
 *  Created on: 22.10.2015
 *      Author: marvin
 */

#include <OgreString.h>
#include <OgreMatrix3.h>
#include <OgreMatrix4.h>
#include "StringValueUtils.h"

#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string
#include "OgreVector3.h"
#include "stdlib.h"
namespace Ogre {

StringValueUtils::StringValueUtils() {
	// TODO Auto-generated constructor stub

}
HlmsParamVec StringValueUtils::stringToParamVec(Ogre::String s) {

	String string = s;
	string.erase(remove(string.begin(), string.end(), '\''), string.end());

	HlmsParamVec params;

	vector<String>::type 				split1;
	vector<vector<String>::type>::type 	split2;

	split1=StringUtil::split(string,",");
	for(size_t i=0; i<split1.size();i++){
		Ogre::String s=split1.at(i);
		split2.push_back(StringUtil::split(s,"="));
	}
	for(size_t i=0; i<split2.size();i++){

		params.push_back(std::make_pair(IdString(split2.at(i).at(0)),split2.at(i).at(1)));

	}
	std::sort(params.begin(),params.end());

	return params;
}
StringValueUtils::~StringValueUtils() {
	// TODO Auto-generated destructor stub
}
//TODO Make more efficent
String StringValueUtils::getColorStr(Ogre::ColourValue colourValue) {
	std::ostringstream ss;
	ss << colourValue.r*1.0f;
	ss << " ";
	ss << colourValue.g*1.0f;
	ss << " ";
	ss << colourValue.b*1.0f;
	ss << " ";
	ss << 0*1.0f;
	std::string s(ss.str());



	return s;

}

String StringValueUtils::getVectorStr(Ogre::Vector3 vector3) {
	std::ostringstream ss;
	ss << vector3.x*1.0f;
	ss << " ";

	ss << vector3.y*1.0f;
	ss << " ";
	ss << vector3.z*1.0f;
	ss << " ";
	ss << 0*1.0f;
	std::string s(ss.str());



	return s;
}
String StringValueUtils::getVectorStr(Ogre::Vector4 vector4) {
	std::ostringstream ss;
	ss << vector4.x*1.0f;
	ss << " ";

	ss << vector4.y*1.0f;
	ss << " ";
	ss << vector4.z*1.0f;
	ss << " ";
	ss << vector4.w*1.0f;
	std::string s(ss.str());



	return s;
}
float* StringValueUtils::getVectorV3(Ogre::String str) {

	Ogre::String s=String(str);
	float retval[3];
	std::string delimiter = " ";

	size_t pos = 0;
	std::string token;

	int i=0;
	int zer=0;
	while ((pos = s.find(delimiter)) != std::string::npos) {

	    token = s.substr(0, pos);
	    s.erase(0, pos + delimiter.length());
	    assert(i<3);
	   retval[i]= atof(token.c_str());
	    i++;

	}
    token = s.substr(0, s.length());

    assert(i<3);
   retval[i]= atof(token.c_str());
    i++;
    assert(i==3);

	return retval;
}
float* StringValueUtils::getVectorV4(Ogre::String str) {

	Ogre::String s=String(str);
	float retval[4];
	std::string delimiter = " ";

	size_t pos = 0;
	std::string token;

	int i=0;
	int zer=0;
	while ((pos = s.find(delimiter)) != std::string::npos) {

	    token = s.substr(0, pos);
	    s.erase(0, pos + delimiter.length());
	    assert(i<4);
	   retval[i]= atof(token.c_str());
	    i++;

	}
    token = s.substr(0, s.length());

    assert(i<4);
   retval[i]= atof(token.c_str());
    i++;
    assert(i==4);

	return retval;
}
String StringValueUtils::getMatrixStr(Matrix4 matrix4) {

	stringstream ss;
	for(int i=0;i<16;i++){

		ss<<matrix4[0][i]<<" ";
	}

	return ss.str();
}

float* StringValueUtils::getMatrixMat4x4(String allocator) {
}
} /* namespace Ogre */


