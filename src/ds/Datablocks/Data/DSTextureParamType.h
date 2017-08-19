/*
 * DSTextureParamType.h
 *
 *  Created on: 03.03.2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSTEXTUREPARAMTYPE_H_
#define SRC_DATABLOCKS_DATA_DSTEXTUREPARAMTYPE_H_
     #include <cstddef>
#include "OgreHlmsSamplerblock.h"
#include "Datablocks/DSDatablock.h"
namespace Ogre {
class DSTextureParamType {




public:


	Ogre::String paramName;
	HlmsSamplerblock samplerBlock;
	DSDatablock::DSTextureType textureType;
	HlmsTextureManager::TextureMapType textureMapType;

	Ogre::String postFix;
	void generateMat4(Vector3 trans,Vector3 scale,Vector3 rot);
	void updateMat4();
	DSTextureParamType();
	virtual ~DSTextureParamType();
};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSTEXTUREPARAMTYPE_H_ */
