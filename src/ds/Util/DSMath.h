/*
 * DSMath.h
 *
 *  Created on: Jan 22, 2018
 *      Author: marvin
 *
 *  Toolkit of nifty functions used by various different classes,
 *  guaranteed to have zero side effects (aside from changing the random seed)
 */

#ifndef SRC_DS_UTIL_DSMATH_H_
#define SRC_DS_UTIL_DSMATH_H_

#include "Ogre.h"

namespace Ogre {



class DSMath {
public:
	DSMath();
	virtual ~DSMath();

	//Takes Color from a rainbow at a certain position (very useful for checking data integrity visually)
	static Vector4 rainbow(float x);
	//random float 1-0 (how is this not built in?)
	static float randomf();
	//random vector 1-0
	static Vector4 random4();
	//random vector 1-0
	static Vector3 random3();

	//same as above using seed
	static float randomf(unsigned seed);
	//same as above using seed
	static Vector4 random4(unsigned seed);
	//same as above using seed
	static Vector3 random3(unsigned seed);

	//generate a quick float pointer to array with size x
	static float* quickArray(int size);
	//generate a quick float pointer to an array filled with Vector d
	static float* quickArray(Vector4 d);
	//generate a quick float pointer to an array filled with Vector d
	static float* quickArray(Matrix4 d);
};

} /* namespace Ogre */

#endif /* SRC_DS_UTIL_DSMATH_H_ */
