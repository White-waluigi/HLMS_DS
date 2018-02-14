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
		const HlmsParamVec &params, bool derived) :
		HlmsDatablock(name, creator, macroblock, blendblock, params) {
	this->mCreator = creator;
	this->mClean = false;
	this->mSafeForRewrite = true;
	this->mIDColor = DSMath::random4();

}

DSDatablock::~DSDatablock() {
	// TODO Auto-generated destructor stub
}

void DSDatablock::createMaterialBuffer() {
	for (std::list<Ogre::DSResource*>::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		(*it)->init();
	}
	this->mMaterialBuffer = mCreator->mVaoManager->createConstBuffer(
			getBufferSize(), BT_DYNAMIC_DEFAULT, 0, false);
	for (std::list<Ogre::DSResource*>::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		(*it)->lock();
	}
	//No more changes possible after the MaterialBuffer has been created.
	mSafeForRewrite = false;
}

void Ogre::DSDatablock::upload() {

	bool clean = true;

	for (std::list<Ogre::DSResource*>::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		(*it)->update();

		//set clean to false if one Resource is still dirty after update
		clean &= (*it)->isClean();
	}

	GPUPointer *g = new GPUPointer();

	g->start(mMaterialBuffer, getBufferSize());

	g->map(mIDColor);

	//Map all resources
	for (std::list<DSResource*>::iterator it = mRes.begin(); it != mRes.end();
			++it) {
		(*it)->upload(g);
	}
	g->quit();

	//If at least one Resources remains dirty, the entire Datablock remains dirty
	this->mClean = clean;
}

void Ogre::DSDatablock::initalize(Parser* parser) {
	if (parser != NULL) {
		parser->parse(this);

	}
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
	int size = 0;
	//IDColor
	size += 4;
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
DSResource::propertyList DSDatablock::genProperties() {
	DSResource::propertyList *l = new DSResource::propertyList();
	for (std::list<Ogre::DSResource*>::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		DSResource::propertyList ll = (*it)->genProperties();
		l->insert(ll.begin(),ll.end());
	}


	(*l)[ShaderKeyWordBank::NUMTEXTURES]=GetTexArraySize();


	return *l;
}
DSResource::TextureRef::textureBuffList DSDatablock::genTextureBufs() {
	DSResource::TextureRef::textureBuffList l;

	for (std::list<Ogre::DSResource*>::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		DSResource::TextureRef::textureBuffList ll = (*it)->genTextureBuf();
		l.merge(ll);
	}
	return l;

}
std::map<IdString, String> DSDatablock::genPieces() {
	DSResource::ShaderPiece::shaderList *l =
			new DSResource::ShaderPiece::shaderList();


	l->push_back(ShaderGenerator::genTextureFunction());

	for (std::list<Ogre::DSResource*>::iterator it = mRes.begin();
			it != mRes.end(); ++it) {
		DSResource::ShaderPiece::shaderList ll = (*it)->genShaderPieces();

		l->splice(l->end(),ll);
	}


	return DSResource::ShaderPiece::GetPieces(l, mCreator->getShaderProfile());
}

int DSDatablock::newIncRSNum() {
	return mResourceNumber++;
}
int DSDatablock::GetTexArraySize() {
	int texUnit=0;
	DSResource::TextureRef::textureBuffList l=genTextureBufs();
	for (DSResource::TextureRef::textureBuffList::iterator it=l.begin(); it != l.end(); ++it) {

		DSResource::TextureRef * lol=(*it);
		if (lol->CustomName.compare("")==0) {

			texUnit++;

		} else {



		}
	}
	return texUnit;

}

} /* namespace Ogre */

