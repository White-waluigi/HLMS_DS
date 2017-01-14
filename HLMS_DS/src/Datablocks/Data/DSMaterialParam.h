/*
 * DSMaterialParam.h
 *
 *  Created on: Aug 18, 2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSMATERIALPARAM_H_
#define SRC_DATABLOCKS_DATA_DSMATERIALPARAM_H_
#include "stddef.h"
#include "OgreString.h"
#include "../../OgreHlmsDSPrerequisites.h"
#include "OgreHlmsDatablock.h"
#include "OgreHlmsTextureManager.h"
#include "OgreConstBufferPool.h"
#include "OgreHeaderPrefix.h"
#include "OgreMatrix4.h"

namespace Ogre {


class GPUPointer{
public:
		float *passBufferPtr;
		void map(float data, int size=1 ){
			for(int i=0;i<size;i++){
				*passBufferPtr++ =data;
			}

		}
		void map(float *data, int size=4 ){
			for(int i=0;i<size;i++){
				*passBufferPtr++ =data[i];
			}
		}
		void map(Ogre::Matrix4 data, int size=16 ){
			for(int i=0;i<size;i++){
				*passBufferPtr++ =((float)data[0][i]);
			}
		}
};

class DSMaterialParamType;
class DSMaterialParam {

public:
	float * data;
	int numVec4;
	enum DSMType{
		SINGLEFLOAT,
		VEC4,
		MAT4,
		UINT,
		UVEC4,
		CUSTOM

	}  type;

	static Ogre::String defaultDatatypes[(size_t)CUSTOM];

	bool noPreFix=false;
	size_t getSize();
	bool array=false;
	size_t arraySize=1;

	Ogre::String postFix;
	Ogre::String paramName;
	int numVec;
	Vector4 defaultVal;




	DSMaterialParam(float * data,DSMType type,bool array=false,size_t arraySize=0);
	DSMaterialParam();
	DSMaterialParam(Ogre::DSMaterialParamType *);

	virtual void map(GPUPointer * pointer);
	virtual void update();
	virtual size_t getCustomSize();


	//must be overriden
	virtual void initialize(const HlmsParamVec &params);
	virtual ~DSMaterialParam();
	float* getData() const;
	void setData(float* data);
	const Ogre::String& getParamName() const;
	void setParamName(const Ogre::String& paramName);
	const Ogre::String& getPostFix() const;
	void setPostFix(const Ogre::String& postFix);
	int getNumVec() const;
	void setNumVec(int numVec);
	void initData();
	Ogre::String getParamString(int index);
	Ogre::String getTypeString(int index);
	Ogre::String getArrayString(int index);


	virtual void setData(Ogre::String,int);

	bool isNoPreFix() const {
		return noPreFix;
	}

	void setNoPreFix(bool noPreFix = false) {
		this->noPreFix = noPreFix;
	}
};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSMATERIALPARAM_H_ */
