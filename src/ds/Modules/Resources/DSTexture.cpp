/*
 * DSTexture.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: marvin
 */

#include "DSTexture.h"
#include "../../OgreHlmsDS.h"
#include "../../Util/ShaderGenerator.h"
#include "../../Util/ShaderKeyWordBank.h"
namespace Ogre {

DSTexture::~DSTexture() {
	// TODO Auto-generated destructor stub
}

DSTexture::DSTexture(reflist l) :
		DSResource(l) {
	//Need some texture Init data, to generate Texture from
	this->mTexInitData  = dynamic_cast<TexInitData *>(getReference("texinit"));

}

DSResource::TextureRef::TextureBuffList DSTexture::genTextureBuf() {
	return this->mTRef;
}

int DSTexture::getBufferSize() {
	//main texloc value
	int size = 4;

	//texture matrix
	size += 16;

	for (DSResource::TextureRef::TextureBuffList::iterator iter =
			this->mTRef.begin(); iter != mTRef.end(); iter++) {
		//All texlocs for switching
		size += 4;
	}
	return size;
}



void DSTexture::init() {

	//Tex Init Data is necessary
	if (this->mTexInitData == NULL) {
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,
				"No Texture specified", "void DSTexture::init() {");
	}
	//Load file or check if available
	loadTexture();

}
refreq * DSTexture::getRequiredRefs() {
	//get Parent Refs
	refreq * r = DSResource::getRequiredRefs();
	//Need a texinit
	r->push_back("texinit");
}

void DSTexture::loadTexture() {

	//Get TextureManager to load Texture
	HlmsManager *hlmsManager = mParentHLMS->getHlmsManager();
	HlmsTextureManager *hlmsTextureManager = hlmsManager->getTextureManager();

	//Check if available otherwise just load it
	HlmsTextureManager::TextureLocation texLocation =
			hlmsTextureManager->createOrRetrieveTexture(
					mTexInitData->mTextureFile, mTexInitData->mTmt);


	//Save Location of Texture
	this->mTRef.push_back(
			new DSResource::TextureRef(texLocation, (mTexInitData->mSb)  )  );

}

DSResource::ShaderPiece::shaderList DSTexture::genShaderPieces() {
	ShaderPiece::shaderList l;

	//Generate GLSL Function for easier acess to texture data
	l.push_back(
			ShaderGenerator::AddTextureFunction(this->getName(),this->getRSNum(),getActiveTexture()->texloc.yIdx));

	//Txture Location (x,y)
	l.push_back(
			ShaderGenerator::getDataTypeShader(UVEC4, this->getName(),
					"texloc_", "MaterialDataParams"));
	//Txture Matrix
	l.push_back(
			ShaderGenerator::getDataTypeShader(MAT4, this->getName(), "texmat_",
					"MaterialDataParams"));

	//All texlocs for switching
	l.push_back(
			ShaderGenerator::getDataTypeShader(UVEC4, this->getName(),
					"texlocA_", "MaterialDataParams", mTRef.size()));


	return l;
}
DSResource::PropertyList DSTexture::genProperties() {
	PropertyList l;
	//Save Resource as String for convience and readability (Resource ID is used to acess shader with the universal texture function)
	l[ShaderKeyWordBank::TEXTUREID + this->getName()] = this->getRSNum();
	return l;
}
void DSTexture::upload(GPUPointer* ptr) {

	//Upload the coordiantes of the currently active Texture is Array
	ptr->map(getActiveTexture()->texloc.xIdx);
	ptr->map(getActiveTexture()->texloc.xIdx);
	ptr->map(getActiveTexture()->texloc.divisor);
	//Padding
	ptr->map(0.0f);

	//Upload texture matrix for texture orientation
	ptr->map(MTexMat);

	//upload all the textures Adresses
	for (DSResource::TextureRef::TextureBuffList::iterator iter =
			this->mTRef.begin(); iter != mTRef.end(); iter++) {

		ptr->map((*iter)->texloc.xIdx);
		ptr->map((*iter)->texloc.xIdx);
		ptr->map((*iter)->texloc.divisor);
		//Padding
		ptr->map(0.0f);
	}
}

DSTexture::TextureRef* DSTexture::getActiveTexture() {

	//If active Index is correct, return it.
	if (mTRef.size() > mActiveIndex) {
		//Map is an indexless storage datatype, so we have to cheat a little bit
		DSResource::TextureRef::TextureBuffList::iterator it = mTRef.begin();
		std::advance(it, mActiveIndex);
		return (*it);
	}
	return NULL;
}

} /* namespace Ogre */
