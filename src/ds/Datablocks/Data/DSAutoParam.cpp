/*
 * DSAutoParam.cpp
 *
 *  Created on: 31.03.2016
 *      Author: marvin
 */

#include "DSAutoParam.h"
#include "string"

#include "OgreException.h"
namespace Ogre {

Ogre::String DSAutoParam::defaultDatatypes[(int)(Custom)]={"mat4","vec4","float","uvec4","uint"};
DSAutoParam::DSAutoParam() {
	size=0;
	type=Ogre::DSAutoParam::_float;
}

DSAutoParam::~DSAutoParam() {
}



Ogre::String DSAutoParam::getParamString(int index) {

	Ogre::String datatype;


	if(!(type==Custom)){
		datatype=defaultDatatypes[type];
	}else {
		throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,"Error, must implement getParamString if custom Datatype is used!","DSAutoParam::getParamString");
	}

	stringstream s;

	s << index;

	Ogre::String converted(s.str());

	return datatype+" autoparam"+ converted+";";


}

//void DSAutoParam::uploadParamData(GPUPointer *g) {
//	update();
//
//	if(!(type==Custom)){
//		for(size_t i=0;i<getSize();i++){
//			*g->passBufferPtr++= data[i];
//		}
//	}else {
//		throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,"Error, must implement uploadParamData if custom Datatype is used!","DSAutoParam::uploadParamData");
//	}
//}

void DSAutoParam::initialize(const HlmsParamVec &params) {
	throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,"Error, must implement initialize for AutoParam!","DSAutoParam::initialize");

}
void DSAutoParam::update() {
	throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,"Error, must implement update for AutoParam!","DSAutoParam::update");

}
size_t DSAutoParam::getSize() {
	if(type==Matrix4x4){
		return sizeof(float)*4*4;
	}else if(type==Vec4){
		return sizeof(float)*4;
	}else if(type==_float){
		return sizeof(float);
	}else if(type==UVec4){
		return sizeof(uint32)*4;
	}else if(type==Uint){
		return sizeof(uint32);
	}else if(type==Custom){
		throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,"Error, must implement getSize if custom Datatype is used!","DSAutoParam::getSize");
	}
	return 0;

}

} /* namespace Ogre */
