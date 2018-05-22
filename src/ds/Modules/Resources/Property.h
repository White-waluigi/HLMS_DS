/*
 * Property.h
 *
 *  Created on: Jan 24, 2018
 *      Author: marvin
 *
 *  Manages an Integer based Setting (similar to #defines)
 *  for the shader code, which are often used to set which parts
 *  of the main codebased are used to assemble the final Source code
 *
 *
 */
#ifndef SRC_DS_MODULES_RESOURCES_PROPERTY_H_
#define SRC_DS_MODULES_RESOURCES_PROPERTY_H_

#include "DSResource.h"

namespace Ogre {

class Property: public DSResource {

	//Module name is used as keyword

	//Value of setting
	int32 mValue=-1;
public:
	//No Construction necessary
	Property(reflist l);


	//None necessary
	virtual DSResource::ShaderPiece::shaderList genShaderPieces();
	//Simply return properties as list
	virtual DSResource::PropertyList genProperties();

	//No data uploaded, therefore 0
	virtual int getBufferSize();

	//not strictly necessary
	virtual void init();

	//no upload is happening
	virtual void upload(GPUPointer * ptr);
	//return set value
	int32 getValue() const;
	//set value
	void setValue(int32 value);

	//Clean up
	virtual ~Property();

};

} /* namespace Ogre */

#endif /* SRC_DS_MODULES_RESOURCES_PROPERTY_H_ */
