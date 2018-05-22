/*
 * DSTexture.h
 *
 *  Created on: Jan 25, 2018
 *      Author: marvin
 *
 *  DSTexture is used as a baseclass for Autotextures and is responsible for uploading Textures to the CPU and
 *  binding them to the shader.
 *  It also generates a number of help functions for accessing said shader and stores the transformation matrix,
 *  so it becomes possible to rotate, scale and translate a texture.
 *  This is done through UVModulators.
 *  Basically it takes an Object named TexInitData and creates a Texture from it.
 */

#ifndef SRC_DS_MODULES_RESOURCES_DSTEXTURE_H_
#define SRC_DS_MODULES_RESOURCES_DSTEXTURE_H_

#include "DSResource.h"
#include "OgreHlmsTextureManager.h"

namespace Ogre {

class DSTexture: public DSResource {
public:

	//Init Data necessary for texture creation,
	//provided by creator of Texture Module
	class TexInitData:public ModuleReference{
	public:
		//Name of Image File to be loaded
		String mTextureFile="";
		//Mapping Type for correct loading of color values
		HlmsTextureManager::TextureMapType mTmt;
		//Sampling, responsible for texture filtering
		const HlmsSamplerblock * mSb;

		//Constructor, pretty basic
		TexInitData(String mTextureFile,HlmsTextureManager::TextureMapType tmt,const HlmsSamplerblock *mSb){
			this->mTextureFile=mTextureFile;
			this->mTmt=tmt;
			this->mSb=mSb;

		}
	};
private:
	//Tex init data for construction, if NULL will produce Error Texture
	TexInitData * mTexInitData=NULL;
	//List of Textures loaded
	DSResource::TextureRef::TextureBuffList mTRef;
	//curent active Index for Multiple Textures
	int mActiveIndex=0;

	//Current Texture Matrix
	Ogre::Matrix4 MTexMat;
public:

	//Get TexInitData
	DSTexture(reflist l);

	//Get list of textures
	virtual TextureRef::TextureBuffList genTextureBuf();

	//Needs a TexInitData
	static refreq * getRequiredRefs();

	//Generate Access Data and Shader Mapping Function
	virtual DSResource::ShaderPiece::shaderList genShaderPieces();
	//Generate Keyword for Resource ID
	virtual DSResource::PropertyList genProperties();

	//Load texture through TextureManager, if not already loaded
	virtual void loadTexture();
	//Size of Access Data
	virtual int getBufferSize();

	//check texture, then load it
	virtual void init();
	//upload access data
	virtual void upload(GPUPointer * ptr);

	//Get currently active data
	virtual TextureRef *getActiveTexture();
	//Clean up
	virtual ~DSTexture();
};

} /* namespace Ogre */

#endif /* SRC_DS_MODULES_RESOURCES_DSTEXTURE_H_ */
