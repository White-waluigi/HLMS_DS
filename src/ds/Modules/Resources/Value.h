/*
 * Value.h
 *
 *  Created on: Jan 21, 2018
 *      Author: marvin
 *
 *  Used to upload and manage data for the Shader Code to the
 *  GPU.
 *  Can use predefined Datatypes or a custom length array of floats or ints.
 */

#ifndef SRC_DS_MODULES_RESOURCES_VALUE_H_
#define SRC_DS_MODULES_RESOURCES_VALUE_H_
#include "DSResource.h"
namespace Ogre {

class Value : public DSResource{

public:


protected:
	dataType mType=VEC4;


	int mArraysize=1;
	//Pointer to Value held for upload to GPU
	float * mValue=0;


public:

	//reservice size
	Value(reflist l);


	//Generate shader definitions for data using name
	virtual DSResource::ShaderPiece::shaderList genShaderPieces();
	//Not necessary
	virtual DSResource::PropertyList genProperties();


	//set data
	virtual void set(float * data);
	//Buffer Size calculation using datatype
	virtual int getBufferSize();
	//Only used for AutoVals, never call for simple Value
	virtual int getCustomSize();
	//reserve space
	virtual void initData();


	//upload stored mValue
	virtual void upload(GPUPointer * ptr);



	//get size of mValue;
	int getArraysize() const;
	//Arraysize can only be modified before Datablock is locked
	void setArraysize(int arraysize = 1);
	//get Datatype of Value
	dataType getType() const;
	//set Datatype of Value
	void setType(dataType type = VEC4);

	//Clean up, delete Value
	virtual ~Value();
};

} /* namespace Ogre */

#endif /* SRC_DS_MODULES_RESOURCES_VALUE_H_ */
