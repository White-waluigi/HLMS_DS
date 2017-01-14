/*
 * DSAutoParam.h
 *
 *  Created on: 31.03.2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSAUTOPARAM_H_
#define SRC_DATABLOCKS_DATA_DSAUTOPARAM_H_

#include "stddef.h"
#include "OgreString.h"
#include "../../OgreHlmsDSPrerequisites.h"
#include "OgreHlmsDatablock.h"
#include "OgreHlmsTextureManager.h"
#include "OgreConstBufferPool.h"
#include "OgreHeaderPrefix.h"
#include "OgreMatrix4.h"
#define MAX_AUTOPARAMDATA		128




namespace Ogre {

class DSAutoParam {
public:
	DSAutoParam();
	virtual ~DSAutoParam();

	//AutoParam might be more complex so I allowed Variable Datatypes here
	enum DataType{
		Matrix4x4,
		Vec4,
		_float,
		UVec4,
		Uint,
		Custom
	} type;

	float * data=new float[MAX_AUTOPARAMDATA];
	size_t size;


	static Ogre::String defaultDatatypes[(size_t)Custom];
	Ogre::String DataType;


	//must be overriden if custom
	virtual Ogre::String getParamString(int index);
	size_t getSize();
	//virtual void uploadParamData(GPUPointer *g);


	//must be overriden
	virtual void initialize(const HlmsParamVec &params);
	virtual void update();




};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSAUTOPARAM_H_ */
