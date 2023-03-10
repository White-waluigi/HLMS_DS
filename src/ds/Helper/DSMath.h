/*
 * DSMath.h
 *
 *  Created on: Apr 21, 2017
 *      Author: marvin
 */

#ifndef SRC_HELPER_DSMATH_H_
#define SRC_HELPER_DSMATH_H_
#include <Ogre.h>
namespace Ogre {

class DSMath {
public:

	static Matrix4 roundPVmat(int resx, int resy, Matrix4 pVMat);
	static Vector4 RainbowPhase(float f);
	DSMath();
	virtual ~DSMath();
};

} /* namespace Ogre */

#endif /* SRC_HELPER_DSMATH_H_ */
