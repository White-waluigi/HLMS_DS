/*
 * GPUPointer.h
 *
 *  Created on: Jun 26, 2017
 *      Author: marvin
 */

#ifndef SRC_HELPER_GPUPOINTER_H_
#define SRC_HELPER_GPUPOINTER_H_
#include <OgreMatrix4.h>
#include <OgreVector4.h>

namespace Ogre {


class GPUPointer{
	float *startp;
	float *passBufferPtr;
public:

		void start(float *start){
			this->startp=start;
			passBufferPtr=start;
		}

		void map(float data, int size=1 ){
			for(int i=0;i<size;i++){
				*passBufferPtr++ =data;
			}

		}
		void map(uint data, int size=1 ){
			typedef union {
			  uint i;
			  float f;
			 } u;
			 u u1;
			 u1.i = data;


			for(int i=0;i<size;i++){
				*passBufferPtr++ =u1.f;
			}

		}
		void map(int data, int size=1 ){
			typedef union {
			  int i;
			  float f;
			 } u;
			 u u1;
			 u1.i = data;


			for(int i=0;i<size;i++){
				*passBufferPtr++ =u1.f;
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
		void map(Ogre::Vector4 data, int size=4 ){
			for(int i=0;i<size;i++){
				*passBufferPtr++ =((float)data[i]);
			}
		}

	float* getPassBufferPtr() const {
		return passBufferPtr;
	}

	void setPassBufferPtr(float* passBufferPtr) {
		this->passBufferPtr = passBufferPtr;
	}

	float* getStart() const {
		return startp;
	}

	void setStart(float* start) {
		this->startp = start;
	}
	int getSize(){
		return passBufferPtr-startp;
	}
};


} /* namespace Ogre */

#endif /* SRC_HELPER_GPUPOINTER_H_ */
