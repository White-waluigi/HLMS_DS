/*
 * DSTextureParam.cpp
 *
 *  Created on: 03.03.2016
 *      Author: marvin
 */

#include "DSTextureParam.h"
#include "DSTextureParamType.h"
#include "DSBlocks/MappingBlocks/DSMappingBlock.h"
namespace Ogre {

DSTextureParam::DSTextureParam(DSTextureParamType* type, DSDatablock * db) {
	// TODO Auto-generated constructor stub
	this->PostFix = type->postFix;
	this->textureType = type->textureType;
	this->samplerBlock = type->samplerBlock;

	this->anim_timer = new Timer();

	this->parent = db;

	this->UVindex = 0;
	this->type=NULL;

}
DSTextureParam::DSTextureParam(HlmsSamplerblock sb, DSDatablock * db) {

	this->anim_timer = new Timer();
	this->samplerBlock = sb;

	this->parent = db;

	this->UVindex = 0;
	this->type=NULL;
}
DSTextureParam::~DSTextureParam() {
	// TODO Auto-generated destructor stub
}

const Ogre::String& Ogre::DSTextureParam::getPostFix() const {
	return PostFix;
}

DSDatablock::DSTextureType Ogre::DSTextureParam::getTextureType() const {
	return textureType;
}

void Ogre::DSTextureParam::setTextureType(
		DSDatablock::DSTextureType textureType) {
	this->textureType = textureType;
}

void Ogre::DSTextureParam::setPostFix(const Ogre::String& postFix) {
	PostFix = postFix;
}

int Ogre::DSTextureParam::getUVindex() const {
	return UVindex;
}

bool DSTextureParam::isDynamic() const {
	return dynamic;
}

size_t DSTextureParam::getNumTextures() {
	return texture.size();
}

DSDatablock::DSBakedTexture* DSTextureParam::getActiveTexture() {
	if (!isMultiTex()) {
		return getTexture(0);
	}
	return NULL;
}

uvec2* DSTextureParam::getActiveLocation() {
	if (!isMultiTex()) {
		return getLocation(0);
	}
	return getLocation(UVindexOffet%texLocation.size());

}

bool DSTextureParam::isMultiTex() {
	return getNumTextures() > 1;
}

int DSTextureParam::getAnimInterval() const {
	return anim_interval;
}

void DSTextureParam::setAnimInterval(int animInterval) {
	anim_interval = animInterval;
}

bool DSTextureParam::isAnimatedTexture() const {
	return animatedTexture || animatedUV;
}

void DSTextureParam::setAnimatedTexture(bool animatedTexture) {
	this->animatedTexture = animatedTexture;
	this->setDynamic(true);

}

int DSTextureParam::getMaxAnim() const {
	return max_anim;
}

Matrix4 DSTextureParam::getTextMat() const {
	Matrix4 retval = texMatOffset * stat_texMat;
	return retval;
}

void DSTextureParam::setTextMat(Matrix4 textMat) {
	this->stat_texMat = textMat;
}

void DSTextureParam::setMaxAnim(int maxAnim) {
	max_anim = maxAnim;
}

void DSTextureParam::setDynamic(bool dynamic) {
	this->dynamic = dynamic;
}

DSDatablock::DSBakedTexture* DSTextureParam::getTexture(int i) {
	assert(i < texture.size());
	return texture.at(i);
}

uvec2 *DSTextureParam::getLocation(int i) {
	return texLocation.at(i);
}

void DSTextureParam::addTexture(HlmsTextureManager::TextureLocation* texloc) {
	uvec2 *vec = new uvec2(texloc->yIdx, texloc->xIdx);

	this->texLocation.push_back(vec);

	DSDatablock::DSBakedTexture* bakedtex = new DSDatablock::DSBakedTexture();
	bakedtex->texture = texloc->texture;
	if (this->PostFix.compare("perlin") == 0) {
		getSamplerBlock();
	}
	bakedtex->samplerBlock = parent->getSamplerBlock(getSamplerBlock());

	this->texture.push_back(bakedtex);
}

void Ogre::DSTextureParam::setUVindex(int vindex) {
	UVindex = vindex;
}
bool Ogre::DSTextureParam::update() {
	long cur = anim_timer->getMicroseconds() / 1000;

	if (animatedTexture) {
		if ((cur - anim_elapsed) > anim_interval) {
			anim_elapsed = cur;
			active_anim++;
			active_anim %= max_anim;
		}
	}

	if (animatedUV) {

		float v = fmod(cur / 1000.0, (anim.transt));

		this->anim_texMat = Matrix4::IDENTITY;
		//this->textMat.setTrans(Vector3(v,0 ,0));
		this->anim_texMat.setTrans(
				Vector3(v * anim.trans.x, v * anim.trans.y, 1));

	}
	if (MappingBlock!=0) {

		MappingBlock->modulate(this);
	}
	return true;
}
bool DSTextureParam::isAnimatedUv() const {
	return animatedUV;
}

void DSTextureParam::setAnimatedUv(bool animatedUv) {
	animatedUV = animatedUv;
	this->setDynamic(true);

}
Matrix4 DSTextureParam::getTexMatOffset() const {
	return texMatOffset;
}

void DSTextureParam::setTexMatOffset(Matrix4 texMatOffset) {
	this->texMatOffset = texMatOffset;
}

int DSTextureParam::getVindexOffet() const {
	return UVindexOffet;
}

void DSTextureParam::setVindexOffet(int vindexOffet) {
	UVindexOffet = vindexOffet;
}
DSMappingBlock* DSTextureParam::getMappingBlock() {
	return MappingBlock;
}

void DSTextureParam::setMappingBlock(DSMappingBlock* mappingBlock) {
	MappingBlock = mappingBlock;
	if (mappingBlock->animated) {
		setDynamic(true);
	}
}
Matrix4 DSTextureParam::getStatTexMat() const {
	return stat_texMat;
}

void DSTextureParam::setStatTexMat(Matrix4 statTexMat) {
	stat_texMat = statTexMat;
}
const Ogre::String& DSTextureParam::getName() const {
	return name;
}

void DSTextureParam::setName(const Ogre::String& name) {
	this->name = name;
}

} /* namespace Ogre */

