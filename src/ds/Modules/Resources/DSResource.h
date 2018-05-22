/*
 * DSResource.h
 *
 *  Created on: Jan 21, 2018
 *      Author: marvin
 *
 * The Resource class is probably the third most important class in
 * the entire plugin (after Datablock and HLMS_DS)
 *
 * It is an abstact Baseclass for any type of Object that wants to upload data
 * to the GPU (such as Values, AutoVals and Textures),
 * and/or modify the shader (Pieces, Properties)
 *
 * It is used by Datablocks (including Light and Forwarded Datablocks) and even the PassBufferManager
 * to manage their respective data.
 *
 * For the Datablock this works like basicly like this:
 *
 * Before a Datablock can be rendered it is asked if it has any data it wants to be uploaded
 * or if it wants to modify source code, so the block then asks its Resources if they have anything
 * they want uploaded or changed.
 * So everytime the Datablock is asked for that Information, it asks every single Resource for it,
 * even those who are not programmed to provide any.
 *
 * That way, Textures can upload Shader Code, change properties and upload floating point value
 *
 * The advantage of this System is, that all the Texture related code is contained in the DSTexture Class
 * and there needs to be no special treatment inside any other class.
 * It also provides the user a lot of flexibility when it comes to creating his own custom Resources.
 */

#ifndef SRC_DS_MODULES_RESOURCES_DSRESOURCE_H_
#define SRC_DS_MODULES_RESOURCES_DSRESOURCE_H_
#include "Ogre.h"
#include "../DSModule.h"
#include "../../Util/GPUPointer.h"
#include "OgreHlmsTextureManager.h"
#include "../../Util/ShaderKeyWordBank.h"

namespace Ogre {
class HlmsDS;
class DSDatablock;


class DSResource: public DSModule {
public:
	//All maps are typedefed to indentify their purpose easier
	typedef std::map<IdString,int32> PropertyList;



	//Class for keeping track of a Texture
	class TextureRef{
	public:
		typedef std::list<String > textureConstList;
		typedef std::list<TextureRef*> TextureBuffList;

		HlmsTextureManager::TextureLocation texloc;
	 	const HlmsSamplerblock *  samplerBlock;
		String mCustomName="";

		TextureRef() {
		}
		TextureRef(const HlmsTextureManager::TextureLocation tex,
			 	const HlmsSamplerblock *  _samplerBlock) :
				texloc(tex), samplerBlock(_samplerBlock) {
		}


	};




	//Class for keeping track of a Piece of ShaderCode (do not confuse with Piece class)
	class ShaderPiece{
	public:
		//Shader Code is stored as name of the shading language (glsl, hlsl) and the Code
		typedef std::map<IdString,String> shaderCode;
		typedef std::list<ShaderPiece*> shaderList;

		//the code
		shaderCode mCode;
		//acess name of the Piece
		Ogre::IdString mShaderKeyWord;

		//What happens if two ShaderPieces have the same name?
		enum conflictSolution{
			APPEND,
			REPLACE,
			PREPEND,
			IGNORE,
			INSERT
		} mConflict;

		//Resolve Shader conflice
		String Merge(Ogre::String,Ogre::String);

		//Merge all Pieces and the return them back
		static std::map<IdString,String> GetPieces(shaderList *,String );

		//set as synced
		static String clean(String);
	};

	//Predefined dataTypes, just for convenience
	enum dataType{
			SINGLEFLOAT,
			VEC4,
			MAT4,
			UINT,
			UVEC4,
			CUSTOM
	};

	//In order to initialize the DSResource Module, it needs to be proved some Mandatory Init Data by its creator
	class MandatoryInit:public ModuleReference{
	public:
		Ogre::String mName;

		MandatoryInit(Ogre::String name);
	};

protected:


	//Name of the DSResource, for convenience, doesn't even need to be locally unique
	Ogre::String mName;

	//Maybe NULL for some Resources (PassBuffer)
	HlmsDS * mParentHLMS;

	//Is the Resource synced
	bool mClean=false;
	//Can the Resource be changed?
	bool mLocked=false;
	//Current local Resource number
	int mResourceNum=-1;

public:
	//Every Module needs a reflist to extract its constructor data
	DSResource(reflist l);
	//Get Resources name
	Ogre::String getName();
	//Clean up
	virtual ~DSResource();

	//Required References are listed, to ensure the reflist contains them before construction
	static refreq * getRequiredRefs();
	//Virtual init used by Datablock
	virtual void init();
	//Update before any Resources are uploaded, rarely necessary
	virtual void update(){};
	//Upload data to an Adress specified by Datablocks Material Buffer
	virtual void upload(GPUPointer * ptr)=0;
	//Get Size added to Material Buffer
	virtual int getBufferSize()=0;


	//Generate a list of shader pieces to be added to Datablock
	virtual ShaderPiece::shaderList genShaderPieces();
	//Generate a list of properties to be added to Datablock
	virtual PropertyList genProperties();
	//Generate a list of textures to be added to Datablock
	virtual TextureRef::TextureBuffList genTextureBuf();

	//Check if Datablock is locked
	void checkLock();
	//Is Resource locked?
	bool locked();
	//Lock the Resource
	void lock();

	//is Resource synced?
	bool isClean();
	//Resource marked as not synced
	void soil();
	//Resource marked as synced
	void clean();

	//Get local Resource number
	int getRSNum();
};

//All list are typedefed to indicate function
typedef std::list<DSResource *> DSResList;

} /* namespace Ogre */

#endif /* SRC_DS_MODULES_RESOURCES_DSRESOURCE_H_ */
