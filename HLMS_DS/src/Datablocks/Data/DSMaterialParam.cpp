/*
 * DSMaterialParam.cpp
 *
 *  Created on: Aug 18, 2016
 *      Author: marvin
 */
#include "../../Datablocks/Data/DSMaterialParamType.h"
#include <math.h>
#include "../../Datablocks/DSDatablock.h"

#include "OgreException.h"
#include "OgreStringConverter.h"

namespace Ogre {

Ogre::String DSMaterialParam::defaultDatatypes[(int) (CUSTOM)] = { "float",
		"vec4", "mat4", "uint", "uvec4" };

float* DSMaterialParam::getData() const {
	return data;
}

DSMaterialParam::DSMaterialParam(float* data, DSMType type, bool array,
		size_t arraySize) {
	this->data = new float[getSize()];
	this->type = type;
	if (array) {
		this->array = array;
		this->arraySize = arraySize;

	}
}

size_t DSMaterialParam::getSize() {
	size_t size;
	if (this->type == SINGLEFLOAT) {
		//Can't be smaller than 4
		size = 4;
	} else {
	}
	switch (this->type) {
	case SINGLEFLOAT:
		size = 4;
		break;
	case UINT:
		size = 4;
		break;
	case VEC4:
		size = 4;
		break;
	case UVEC4:
		size = 4;
		break;

	case MAT4:
		size = 16;
		break;

	case CUSTOM:
		size = getCustomSize();
		break;

	}
	return size * arraySize;
}

void DSMaterialParam::map(GPUPointer* pointer) {
	pointer->map(this->data, getSize());
}

void DSMaterialParam::update() {
}

size_t DSMaterialParam::getCustomSize() {
	throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,
			"Error, must implement update for getSize for custom datatype!",
			"DSMaterialParam::getSize");
}

void DSMaterialParam::setData(float* data) {
	this->data = data;
}

DSMaterialParam::~DSMaterialParam() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */

const Ogre::String& Ogre::DSMaterialParam::getParamName() const {
	return paramName;
}

void Ogre::DSMaterialParam::setParamName(const Ogre::String& paramName) {
	this->paramName = paramName;
}

const Ogre::String& Ogre::DSMaterialParam::getPostFix() const {
	return postFix;
}

int Ogre::DSMaterialParam::getNumVec() const {
	return numVec;
}

void Ogre::DSMaterialParam::setNumVec(int numVec) {
	this->numVec = numVec;
}

void Ogre::DSMaterialParam::setPostFix(const Ogre::String& postFix) {
	this->postFix = postFix;
}

Ogre::DSMaterialParam::DSMaterialParam() {
}

Ogre::DSMaterialParam::DSMaterialParam(Ogre::DSMaterialParamType* type) {
	this->postFix = type->postFix;
	this->numVec = type->numVec;
	this->paramName = type->paramName;
	this->defaultVal = type->defaultVal;
	this->array = type->array;
	this->type = type->type;

	this->numVec4 = 0;
	initData();
}

void Ogre::DSMaterialParam::initialize(const HlmsParamVec& params) {
}

Ogre::String Ogre::DSMaterialParam::getParamString(int index) {

	Ogre::String datatype;

	if (!(type == CUSTOM)) {
		datatype = defaultDatatypes[type];
	} else {
		throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,
				"Error, must implement getParamString if custom Datatype is used!",
				"DSAutoParam::getParamString");
	}

	stringstream s;
	int i;

	s << index;

	Ogre::String converted(s.str());

	return datatype + " autoparam" + converted + ";";

}

void Ogre::DSMaterialParam::initData() {
	if (getSize() > 0) {
		this->data = new float[getSize()];
	}
}

Ogre::String Ogre::DSMaterialParam::getTypeString(int index) {
	Ogre::String type;
	if (this->type == SINGLEFLOAT) {
		//Can't be smaller than 4, so we're forced to use vec4
		type = "vec4";
	} else {
	}
	switch (this->type) {
	case SINGLEFLOAT:
		type = "vec4";
		break;
	case UINT:
		type = "uvec4";
		break;
	case VEC4:
		type = "vec4";
		break;
	case UVEC4:
		type = "uvec4";
		break;

	case MAT4:
		type = "mat4";
		break;

	case CUSTOM:
		type = "vec4";
		break;

	}
	return type;
}

Ogre::String Ogre::DSMaterialParam::getArrayString(int index) {
	if (array) {
		stringstream s;
		s << "[" << arraySize << "]";
		return s.str();

	}
	return "";
}

void Ogre::DSMaterialParam::setData(Ogre::String paramVal, int index = -1) {

	StringVector splitstr = StringUtil::tokenise(paramVal, ";");
	int iters = splitstr.size();

	assert(iters > 0);
	arraySize = iters;
	initData();
	for (int i = 0; i < iters; i++) {
		String CparamVal = splitstr.at(i);

//			Vector4 val = StringConverter::parseVector4(CparamVal,
//					Vector4::ZERO);

		float * val;

		switch (this->type) {
		case SINGLEFLOAT: {

			float vf = StringConverter::parseReal(CparamVal, 0);
			val=new float;
			val[0] = vf;

			break;
		}
		case UINT: {
			float iv = StringConverter::parseInt(CparamVal, 0);

			typedef union {
				int i;
				float f;
			} f2i;
			f2i u;
			u.i = iv;
			val=new float;
			val[0] = u.f;

			break;
		}
		case VEC4: {
			Vector4 vec = StringConverter::parseVector4(CparamVal,
					Vector4::ZERO);

			val = new float[4];
			val[0] = vec.x;
			val[1] = vec.y;
			val[2] = vec.z;
			val[3] = vec.w;

			break;
		}
		case UVEC4: {
			Vector4 vec = StringConverter::parseVector4(CparamVal,
					Vector4::ZERO);

			int x[4];

			x[0]=(int)vec.x;
			x[1]=(int)vec.y;
			x[2]=(int)vec.z;
			x[3]=(int)vec.w;

			val[0] =*reinterpret_cast<float*>(&x[0]) ;
			val[1] =*reinterpret_cast<float*>(&x[1]) ;
			val[2] =*reinterpret_cast<float*>(&x[2]) ;
			val[3] =*reinterpret_cast<float*>(&x[3]) ;

			break;
		}
		case MAT4: {
			Matrix4 mat = StringConverter::parseMatrix4(CparamVal,
					Matrix4::ZERO).transpose();


			val = new float[16];

			for (int i = 0; i < 16; i++) {
				val[i] = mat[0][i];
			}
			break;

		}
		}

		int dtsize = 0;
		int dtsizeinc = -1;

		switch (this->type) {
		case SINGLEFLOAT:
			dtsize = 4;
			dtsizeinc = 1;
			break;
		case VEC4:
			dtsize = 4;
			break;
		case UVEC4:
			dtsize = 4;
			break;

		case MAT4:
			dtsize = 16;
			break;

		}
		if (dtsizeinc == -1) {
			dtsizeinc = dtsize;
		}
		int z = i * dtsize;

		for (int ii = 0; ii < dtsizeinc; ii++) {
			data[z + ii] = val[ii];

		}

	}


}

