/*
 * DSMath.cpp
 *
 *  Created on: Jan 22, 2018
 *      Author: marvin
 */

#include "DSMath.h"

namespace Ogre {

DSMath::DSMath() {
	// TODO Auto-generated constructor stub

}

DSMath::~DSMath() {
	// TODO Auto-generated destructor stub
}
Vector4 Ogre::DSMath::rainbow(float progress) {

	/*
	 * Calucluates a rainbow color cycle
	 * Works like this:
	 * - cap the progress inbetween 0 and 1
	 * - divide progress into 6 equally long phases
	 * - Inside those phases create a variable that gets bigger the closer we are to the next phase
	 * - Create a variable that descends as we get closer
	 * - then shift these variables on different color channels depending on the phase
	*/

	//truncate progress variable in between 0 1 and multiply by 6
    float div = ((fmod(progress ,1.0)) * 6.0);
    //Get phase number
    int phase=(int)div;

    //Create an ascending and a descending variable
    float ascending = (fmod(div , 1.0));
    float descending = 1.0 - ascending;



    //Shift the variables depending on the needed phase

    if(phase==0){
    	return Vector4( 1.0, ascending, 0,			1);
    }else if(phase==1){
    	return Vector4( descending, 1.0, 0,			1);
    }else if(phase==2){
    	return Vector4( 0, 1.0, ascending,			1);
    }else if(phase==3){
    	return Vector4( 0, descending, 1.0,			1);
    }else if(phase==4){
    	return Vector4( ascending, 0, 1.0,			1);
    }else if(phase==5){
    	return Vector4( 1.0, 0, descending,			1);
    }else{
    	//Should never happen
    	return Vector4(0.1,0.5,0.5,0);
    }



}

float Ogre::DSMath::randomf() {
	//Ogre::Random is not used because Real is not guaranteed to be a float
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

Vector4 Ogre::DSMath::random4() {
	//Just get 4 random floats
	return Vector4(randomf(),randomf(),randomf(),randomf());
}

Vector3 Ogre::DSMath::random3() {
	//Get 3 random floats
	return Vector3(randomf(),randomf(),randomf());
}

float* DSMath::quickArray(int size) {

	//get a quick array reserved
	float* ret=new float[size];
	//fill it with some kind of consistent data that it not always 0
	for(int i=0;i<size;i++){
		ret[i]=1.0/size;
	}

	return ret;
}

float* DSMath::quickArray(Vector4 d) {

	//get a quick flaot array filled with data from Vector
	//should be part of OGRE::Vector4 imo

	float * f=new float[4];
	for(int i=0;i<4;i++){
		f[i]=d[i];
	}
	return f;
}

float* DSMath::quickArray(Matrix4 d) {
	//get a quick flaot array filled with data from Matrix
	//should be part of OGRE::Matrix4 imo

	float * f=new float[16];
	for(int i=0;i<16;i++){
		f[i]=*(d[i]);
	}
	return f;
}
float DSMath::randomf(unsigned int seed) {
	//same as above but with seed
	return static_cast <float> (rand_r(&seed)) / static_cast <float> (RAND_MAX);

}

Vector4 DSMath::random4(unsigned int seed) {
	//same as above but with seed
	return Vector4(randomf(),randomf(),randomf(),randomf());

}

Vector3 DSMath::random3(unsigned int seed) {
	//same as above but with seed
	return Vector3(randomf(),randomf(),randomf());
}
} /* namespace Ogre */


