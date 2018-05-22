/*
 * GPUPointer.h
 *
 *  Created on: Jun 26, 2017
 *      Author: marvin
 *
 *  Class for comfortably and safely uploading data to the GPU
 *  by using wrapper functions and a destination adress
 */

#ifndef SRC_HELPER_GPUPOINTER_H_
#define SRC_HELPER_GPUPOINTER_H_
#include <OgreMatrix4.h>
#include <OgreVector4.h>

#include <OgreConstBufferPacked.h>


namespace Ogre {


class GPUPointer{

	//Buffer to upload data to
	ConstBufferPacked * cbp;
	//starting adress (for calculating size)
	float *startp;
	//Current adress position
	float *mPassBufferPtr;
public:

		//Begin uploading process using packed, initializing adress
		void start(ConstBufferPacked * start,int size){
			this->cbp=start;
			this->startp=reinterpret_cast<float*>(start->map(0,size));
			mPassBufferPtr=this->startp;
		}
		//Begin uploading process using ints
		void start(uint32 * start){
			this->startp=reinterpret_cast<float*>(start);
			mPassBufferPtr=this->startp;
		}
		//Begin uploading process using floats
		void start(float * start){
			this->startp=start;
			mPassBufferPtr=this->startp;
		}

		//Simply fill an array with given size with copies of data
		void map(float data, int size=1 ){
			for(int i=0;i<size;i++){
				*mPassBufferPtr++ =data;
			}

		}
		//Simply fill an array with given size with copies of data (uint)
		void map(uint data, int size=1 ){
			typedef union {
			  uint i;
			  float f;
			 } u;
			 u u1;
			 u1.i = data;


			for(int i=0;i<size;i++){
				*mPassBufferPtr++ =u1.f;
			}

		}

		//Simply fill an array with given size with copies of data (int)
		void map(int data, int size=1 ){
			typedef union {
			  int i;
			  float f;
			 } u;
			 u u1;
			 u1.i = data;


			for(int i=0;i<size;i++){
				*mPassBufferPtr++ =u1.f;
			}

		}

		//read from pointer until size is reach and upload to gpu (uint)
		void map(uint *data, int size=4 ){
			for(int i=0;i<size;i++){
				typedef union {
				  uint i;
				  float f;
				 } u;
				 u u1;
				 u1.i = data[i];


				*mPassBufferPtr++ =u1.f;
			}
		}
		//read from pointer until size is reach and upload to gpu (int)
		void map(int *data, int size=4 ){
			for(int i=0;i<size;i++){
				typedef union {
				  int i;
				  float f;
				 } u;
				 u u1;
				 u1.i = data[i];


				*mPassBufferPtr++ =u1.f;
			}
		}

		//read from pointer until size is reach and upload to gpu
		void map(float *data, int size=4 ){
			for(int i=0;i<size;i++){
				*mPassBufferPtr++ =data[i];
			}
		}

		//upload matrix to gpu
		void map(Ogre::Matrix4 data, int size=16 ){
			for(int i=0;i<size;i++){
				*mPassBufferPtr++ =((float)data[0][i]);
			}
		}
		//upload vector to gpu
		void map(Ogre::Vector4 data, int size=4 ){
			for(int i=0;i<size;i++){
				*mPassBufferPtr++ =((float)data[i]);
			}
		}

	//get mPassBufferPtr
	float* getPassBufferPtr() const {
		return mPassBufferPtr;
	}
	//set mPassBufferPtr
	void setPassBufferPtr(float* passBufferPtr) {
		this->mPassBufferPtr = passBufferPtr;
	}

	//get start adress
	float* getStart() const {
		return startp;
	}

	//set start adress
	void setStart(float* start) {
		this->startp = start;
	}

	//Get size by subtracing start from position
	int getSize(){
		return mPassBufferPtr-startp;
	}

	//end uploading process
	void quit(){
		cbp->unmap(UO_KEEP_PERSISTENT);
	}
};


} /* namespace Ogre */

#endif /* SRC_HELPER_GPUPOINTER_H_ */
