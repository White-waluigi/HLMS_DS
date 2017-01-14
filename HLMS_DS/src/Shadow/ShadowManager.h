/*
 * ShadowManager.h
 *
 *  Created on: Jul 14, 2016
 *      Author: marvin
 */

#ifndef SRC_SHADOW_SHADOWMANAGER_H_
#define SRC_SHADOW_SHADOWMANAGER_H_
#include "../Datablocks/Data/DSVec4Param.h"
#include "../Datablocks/Data/DSMaterialParamType.h"
#include "../Datablocks/Data/DSTextureParam.h"
#include "../Datablocks/Data/DSTextureParamType.h"

#include "../OgreHlmsDS.h"

namespace Ogre {
class ShadowManager {

public:
	class Config{
	public:
		bool enableTransperancyBlending=false;

	};
	std::vector<DSTextureParamType> *textureParams;
	std::vector<DSMaterialParamType> *vec4Params;
	std::vector<DSPropertyParam> *propertyParams;
	std::vector<DSPieceParam> *pieceParams;


	Config * config;
	HlmsDS * hlmsman;


	ShadowManager(Config * config,HlmsDS * hlmsman );
	void generateProperties();
	ConstBufferPacked * generateProperties(DSDatablock * db);
	void bindTextures(CommandBuffer* cb, int texUnit);

	virtual ~ShadowManager();
};

} /* namespace Ogre */

#endif /* SRC_SHADOW_SHADOWMANAGER_H_ */
