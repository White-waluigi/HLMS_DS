/*
 * DSDatablock.cpp
 *
 *  Created on: Jan 4, 2018
 *      Author: marvin
 */

#include "DSDatablock.h"

#include "../OgreHlmsDS.h"
#include "Parsers/Parser.h"
#include "../Util/DSMath.h"
#include "../Util/GPUPointer.h"
#include "../Util/ShaderGenerator.h"
#include "../Util/ShaderKeyWordBank.h"
#include "../Modules/Resources/Value.h"

namespace Ogre {

DSDatablock::DSDatablock(IdString name, HlmsDS *creator,
		const HlmsMacroblock *macroblock, const HlmsBlendblock *blendblock,
		const HlmsParamVec &params) :
		HlmsDatablock(name, creator, macroblock, blendblock, params) {
	assert(creator);
	assert(macroblock);
	assert(blendblock);


	//Set parent HLMS
	this->mCreator = creator;
	//Not synced before the first upload
	this->mClean = false;
	//This Datablock can still be changed
	this->mSafeForRewrite = true;
	this->mIDColor = DSMath::random4();

}

DSDatablock::~DSDatablock() {
	// TODO Cleanup
}

void DSDatablock::createMaterialBuffer() {

	//Initialize every Resource
	for (DSResList::iterator it = mRes.begin();
			it != mRes.end(); ++it) {

		assert(*it);

		(*it)->init();
	}

	//Reserve Space on the GPU for the Buffer
	this->mMaterialBuffer = mCreator->mVaoManager->createConstBuffer(
			getBufferSize(), BT_DYNAMIC_DEFAULT, 0, false);


	//Lock Resource so they cannot be modified (except values and textures)
	for (DSResList::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		(*it)->lock();
	}
	//No more changes to datablock possible after the MaterialBuffer has been created.
	mSafeForRewrite = false;
}

void Ogre::DSDatablock::upload() {

	assert(mMaterialBuffer);
	assert(getBufferSize());

	//assumed clean until proven otherwise
	bool clean = true;

	//update every Recource before first upload (rarely necessary, maybe removed later)
	for (DSResList::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		(*it)->update();

		//set clean to false if one Resource is still dirty after update (AutoVals),
		//otherwise mark as clean
		//If Datablock contains at least one perpetually dirty resource, it will never
		//be clean itself
		clean &= (*it)->isClean();
	}
	//Use GPUPointer to upload data to MaterialBuffer
	GPUPointer *g = new GPUPointer();

	//start with static size
	g->start(mMaterialBuffer, getBufferSize());

	//Upload IDColor to identify material and ensure that some data is uploaded
	g->map(this->mIDColor);

	//Upload data for all Resources
	for (DSResList::iterator it = mRes.begin(); it != mRes.end();
			++it) {
		(*it)->upload(g);
	}
	//Done uploading
	g->quit();

	//If at least one Resources remains dirty, the entire Datablock remains dirty
	this->mClean = clean;

}

void Ogre::DSDatablock::initalize(Parser* parser) {
	//If a parser is present, use it initialize the entire datablock
	if (parser != NULL) {
		parser->parse(this);
	}
	//Initialize MaterialBuffer and therefore lock datablock
	createMaterialBuffer();
}

ConstBufferPacked * DSDatablock::getMaterialBuffer() {
	return mMaterialBuffer;
}

void DSDatablock::soil() {
	this->mClean = false;
}

void DSDatablock::clean() {
	this->mClean = true;
}

int DSDatablock::getBufferSize() {
	//return value
	int size = 0;
	//IDColor
	size += 4;
	//Add each Resource's static bufferSize
	for (std::list<Ogre::DSResource*>::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		size += (*it)->getBufferSize();
	}
	return size;
}
const Ogre::Vector4& Ogre::DSDatablock::getIdColor() const {
	return mIDColor;
}

void DSDatablock::addResource(DSResource *r) {
	assert(r);
	//can only add more Resources if the datablock isn't locked
	if (!mSafeForRewrite) {
		throw Ogre::Exception(Exception::ERR_INVALID_STATE,
				"No Resources can be added after initialization",
				"void DSDatablock::addResource(DSResource *r) {");
	}
	mRes.push_back(r);
}

void Ogre::DSDatablock::setIdColor(const Ogre::Vector4& idColor) {
	mIDColor = idColor;
}
DSResource::PropertyList DSDatablock::genProperties() {
	//Return value, empty PropertyList
	DSResource::PropertyList *l = new DSResource::PropertyList();

	//Let each Resource generate its own PropertyList, then add to return value
	for (DSResList::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		DSResource::PropertyList ll = (*it)->genProperties();
		//l+= doesn't work for some reason
		l->insert(ll.begin(),ll.end());
	}

	//Set the number of textures used by this datablock, usefull for shaders
	(*l)[ShaderKeyWordBank::NUMTEXTURES]=GetTexArraySize();


	return *l;
}
DSResource::TextureRef::TextureBuffList DSDatablock::genTextureBufs() {
	//begin with empty list
	DSResource::TextureRef::TextureBuffList l;

	//Let each Resource generate its own TextureBuffList, then add to return value
	for (DSResList::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		DSResource::TextureRef::TextureBuffList ll = (*it)->genTextureBuf();
		l.merge(ll);
	}
	return l;

}
std::map<IdString, String> DSDatablock::genPieces() {

	//begin with empty list
	DSResource::ShaderPiece::shaderList *l =
			new DSResource::ShaderPiece::shaderList();

	//generate main textreu function first
	l->push_back(ShaderGenerator::genTextureFunction());

	//Let each Resource generate its own Pieces, then add to return value
	for (DSResList::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		DSResource::ShaderPiece::shaderList ll = (*it)->genShaderPieces();

		l->splice(l->end(),ll);
	}

	//Only return pieces for currently selected Shader Profile
	return DSResource::ShaderPiece::GetPieces(l, mCreator->getShaderProfile());
}

int DSDatablock::newIncRSNum() {
	//Increase number overtime a Resource asks for one
	return mResourceNumber++;
}
int DSDatablock::GetTexArraySize() {
	//return value
	int texUnit=0;

	//Get textures for each Resource
	DSResource::TextureRef::TextureBuffList l=genTextureBufs();

	//Count each exture
	for (DSResource::TextureRef::TextureBuffList::iterator it=l.begin(); it != l.end(); ++it) {

		//Dereference the texture
		DSResource::TextureRef * texture=(*it);

		//If texture contains a mCustomName, it is not in the main texture array and doesn't need to be counted
		if (texture->mCustomName.compare("")==0) {
			texUnit++;
		}
	}
	return texUnit;

}
bool DSDatablock::isClean() const {
	return mClean;
}

} /* namespace Ogre */

