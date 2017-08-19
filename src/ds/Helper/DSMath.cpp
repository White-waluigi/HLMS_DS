/*
 * DSMath.cpp
 *
 *  Created on: Apr 21, 2017
 *      Author: marvin
 */

#include "Helper/DSMath.h"

namespace Ogre {

DSMath::DSMath() {
	// TODO Auto-generated constructor stub

}
Vector4 DSMath::RainbowPhase(float f) {
	int level = (int)(f*3.0)%3;


	float x = fmod(f*3.0, 1.0);
	float y = 1 - fmod(f*3.0, 1.0);

	float r, g, b;
	if (level ==0) {
		r = y;
		g = x;
		b = 0;
	} else if (level ==1) {
		r = 0;
		g = y;
		b = x;
	} else if (level==2) {
		r = x;
		g = 0;
		b = y;
	} else {
		r = 1;
		g = 1;
		b = 1;
	}

	return Vector4(r, g, b, 0.0);
}
Matrix4 DSMath::roundPVmat(int resx, int resy, Matrix4 pVMat) {
		// xShadowMatrix is the light view projection matrix
		Ogre::Vector3 ptOriginShadow(0, 0, 0);

		ptOriginShadow = pVMat * ptOriginShadow;
		// Find nearest shadow map texel. The 0.5f is because x,y are in the
		// range -1 .. 1 and we need them in the range 0 .. 1
		float texCoordX = ptOriginShadow.x * resx * 0.5f;
		float texCoordY = ptOriginShadow.y * resy * 0.5f;

		// Round to the nearest 'whole' texel
		float texCoordRoundedX = round(texCoordX);
		float texCoordRoundedY = round(texCoordY);

		// The difference between the rounded and actual tex coordinate is the
		// amount by which we need to translate the shadow matrix in order to
		// cancel sub-texel movement
		float dx = texCoordRoundedX - texCoordX;
		float dy = texCoordRoundedY - texCoordY;

		// Transform dx, dy back to homogenous light space
		dx /= resx * 0.5f;
		dy /= resy * 0.5f;

		Matrix4 xRounding=Matrix4::IDENTITY;
		xRounding.makeTrans(Ogre::Vector3(dx, dy, 0));

//		std::cout << "\ndx,dy:   \t" << dx<<" "<<dy;
//
//		std::cout << "\nbefore:   \t" << pVMat;
//
//		std::cout << "\nround:   \t" << xRounding;

		Matrix4 retVal=pVMat*xRounding;
//		std::cout << "\nafter:   \t" << pVMat;
//		std::cout << "\n";

		return retVal;
}

DSMath::~DSMath() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */
