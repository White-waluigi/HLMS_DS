/*
 * DSTextureParamType.cpp
 *
 *  Created on: 03.03.2016
 *      Author: marvin
 */

#include "DSTextureParamType.h"

namespace Ogre {

DSTextureParamType::DSTextureParamType() {
	// TODO Auto-generated constructor stub
	this->samplerBlock.mU=Ogre::TAM_WRAP;
	this->samplerBlock.mV=Ogre::TAM_WRAP;
	this->samplerBlock.mW=Ogre::TAM_WRAP;

	this->textureMapType=HlmsTextureManager::TEXTURE_TYPE_DIFFUSE;
}

DSTextureParamType::~DSTextureParamType() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */

