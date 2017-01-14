/*
 * DSTextureParam.h
 *
 *  Created on: 03.03.2016
 *      Author: marvin
 */

#ifndef SRC_DATABLOCKS_DATA_DSTEXTUREPARAM_H_
#define SRC_DATABLOCKS_DATA_DSTEXTUREPARAM_H_
#include <cstddef>
#include "OgreVector2.h"
#include "../DSDatablock.h"
#include "OgreHlmsSamplerblock.h"
namespace Ogre {
struct uvec2{
	int x;
	int y;
	uvec2(int x,int y){
		this->x=x;
		this->y=y;
	}
};

class DSTextureParamType;
class DSMappingBlock;
class DSTextureParam {
	DSDatablock * parent;

	DSTextureParamType* type;
	Ogre::String PostFix;
	DSDatablock::DSTextureType textureType=DSDatablock::DS_TEXTURE_DIFFUSE;
	HlmsSamplerblock samplerBlock;

	int UVindex;
	int UVindexOffet=0;
	Matrix4 stat_texMat=Matrix4::IDENTITY;
	Matrix4 anim_texMat=Matrix4::IDENTITY;
	Matrix4 texMatOffset=Matrix4::IDENTITY;


	
	std::vector<DSDatablock::DSBakedTexture*> texture;

	std::vector<uvec2*>  texLocation;

	bool dynamic=false;

	int active_anim=0;
	
	bool animatedTexture=false;
	bool animatedUV=false;

	int anim_interval=0;

	Ogre::Timer * anim_timer;

	long anim_elapsed=0;

	int max_anim=0;

	DSMappingBlock * MappingBlock=NULL;

	Ogre::String name="none";

public:
	struct Animation {
		Vector2 trans;
		Vector2 scale;
		Vector2 rotate;

		float transp;
		float scalep;
		float rotatep;
		
		float transt;
		float scalet;
		float rotatet;		
	} anim;

	void addTexture(HlmsTextureManager::TextureLocation* texloc);
	DSDatablock::DSBakedTexture * getActiveTexture();
	
	uvec2  * getActiveLocation();
	
	DSDatablock::DSBakedTexture * getTexture(int i);
	size_t getNumTextures();

	uvec2  * getLocation(int i);

	bool update();
	DSTextureParam(DSTextureParamType* type,DSDatablock * db);
	DSTextureParam( HlmsSamplerblock  sb,DSDatablock * db);
	virtual ~DSTextureParam();
	const Ogre::String& getPostFix() const;
	void setPostFix(const Ogre::String& postFix);
	DSDatablock::DSTextureType getTextureType() const;
	void setTextureType(DSDatablock::DSTextureType textureType);

	
	const HlmsSamplerblock getSamplerBlock() const {
		return samplerBlock;
	}

	void setSamplerBlock(const HlmsSamplerblock samplerBlock) {
		if(this->PostFix.compare("DoomSB")==0){
			getPostFix();
		}
		this->samplerBlock = samplerBlock;
	}

	int getUVindex() const;
	void setUVindex(int vindex);

	bool isMultiTex();

	bool isDynamic() const;
	void setDynamic(bool dynamic = false);
	int getAnimInterval() const;
	void setAnimInterval(int animInterval);
	bool isAnimatedTexture() const;
	void setAnimatedTexture(bool animatedTexture);
	int getMaxAnim() const;
	void setMaxAnim(int maxAnim);
	Matrix4 getTextMat() const;
	void setTextMat(Matrix4 textMat);
	bool isAnimatedUv() const;
	void setAnimatedUv(bool animatedUv);
	Matrix4 getTexMatOffset() const;
	void setTexMatOffset(Matrix4 texMatOffset = Matrix4::IDENTITY);
	int getVindexOffet() const;
	void setVindexOffet(int vindexOffet);
	DSMappingBlock* getMappingBlock() ;
	void setMappingBlock(DSMappingBlock* mappingBlock = NULL);
	Matrix4 getStatTexMat() const;
	void setStatTexMat(Matrix4 statTexMat = Matrix4::IDENTITY);
	const Ogre::String& getName() const;
	void setName(const Ogre::String& name);
};

} /* namespace Ogre */

#endif /* SRC_DATABLOCKS_DATA_DSTEXTUREPARAM_H_ */
