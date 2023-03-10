/*
 * DSDatablock.h
 *
 *  Created on: 24.11.2015
 *      Author: marvin
 */
#include "math.h"

#ifndef DATABLOCKS_DSDATABLOCK_H_
#define DATABLOCKS_DSDATABLOCK_H_
#include <OgreHlmsDatablock.h>
#include <OgreHlmsTextureManager.h>
#include <OgreConstBufferPool.h>
#include <OgreHeaderPrefix.h>


#include "Datablocks/Data/DSPropertyParam.h"
#include "Datablocks/Data/DSPieceParam.h"
#include "Datablocks/AutoParams/DSAutoParamParser.h"
#include "OgreHlmsDSPrerequisites.h"
#include "Datablocks/Data/DSShadowParam.h"
#include "Datablocks/Data/DSShadowTexture.h"

namespace Ogre {
class HlmsDS;
class DSTextureParamType;
class DSMaterialParamType;

class DSTextureParam;
class DSMaterialParam;
class DBParser;
class JsonParser;
class ParamVecParser;
class DSMappingBlock;
class  DSDatablock: public HlmsDatablock {
public:
	std::vector<DSTextureParamType> *textureParamTypes;
	std::vector<DSMaterialParamType> *vec4ParamTypes;
	std::vector<DSPropertyParamType> *propertyParamTypes;
	std::vector<DSPieceParamType> *pieceParamTypes;

	std::vector<DSTextureParam *> *textureParams;
	std::vector<DSPropertyParam *> *propertyParams;
	std::vector<DSMaterialParam *> *materialParams;
	std::vector<DSPieceParam *> *pieceParams;

	std::map<Ogre::String, DSMappingBlock *> MBs;
	std::map<Ogre::String, HlmsSamplerblock *> JsonSamplerBlocks;

	//std::vector<DSAutoParam*> *autoParams;

	std::vector<DSPropertyParam> check;

	Ogre::ColourValue* IDColour;

	DSAutoParamParser * APparser;
	ParamVecParser * Mparser;
	JsonParser * JMparser;

	static const size_t MaterialSizeInGpu;
	static const size_t MaterialSizeInGpuAligned;

	//default Values for now (14 Textures+4 vec4)
	static const size_t MaxVec4Params = 4;
	static const size_t MaxTextureParams = 4;

	enum DSTextureType {
		DS_TEXTURE_DIFFUSE,
		DS_TEXTURE_NORMAL,
		DS_TEXTURE_SPECULAR,
		DS_TEXTURE_GLOW,
		DS_TEXTURE_OPACITY,
		DS_TEXTURE_REFLECTION,

		DS_TEXTURE_CUSTOM,

		NUM_DS_TEXTURE_TYPES
	};
	enum DSDATABLOCKTYPE {
		DS_DATABLOCK_BASE,
		DS_DATABLOCK_MATERIAL,
		DS_DATABLOCK_LIGHT,
		DS_DATABLOCK_FORWARD,

		NUM_DS_DATABLOCK_TYPES
	} dbtype;
//
	struct DSBakedTexture {

		TexturePtr texture;
		HlmsSamplerblock const *samplerBlock;

		//kinda hacky but I hate hardcoding texturetypes

		DSBakedTexture() :
				samplerBlock(0) {
		}
		DSBakedTexture(const TexturePtr tex,
				const HlmsSamplerblock *_samplerBlock) :
				texture(tex), samplerBlock(_samplerBlock) {
		}

		bool operator ==(const DSBakedTexture &_r) const {
			return texture == _r.texture && samplerBlock == _r.samplerBlock;
		}
	};
	typedef FastArray<DSBakedTexture> DSBakedTexturesArray;

	int TextureSize;

	char static_control[16];
	char dynamic_control[16];

	uint16 mTexIndices[NUM_DS_TEXTURE_TYPES];

	HlmsDS *creator;

	// can't use Const Buffer Pool
	ConstBufferPacked* MaterialBuffer;
	ConstBufferPacked* ShadowMaterialBuffer;

	DSBakedTexturesArray mBakedTextures;
	/// The way to read this variable is i.e. get diffuse texture,
	/// mBakedTextures[mTexToBakedTextureIdx[PBSM_DIFFUSE]]
	/// Then read mTexIndices[PBSM_DIFFUSE] to know which slice of the texture array.
	uint8 mTexToBakedTextureIdx[NUM_DS_TEXTURE_TYPES];

	HlmsSamplerblock const *mSamplerblocks[NUM_DS_TEXTURE_TYPES];

	/// Sets the appropiate mTexIndices[textureType], and returns the texture pointer
	HlmsTextureManager::TextureLocation setTexture(const String &name,
			DSTextureType textureType, DSTextureParam*paramtype = NULL);

	HlmsTextureManager::TextureLocation setTexture(const String &name,
			Ogre::HlmsTextureManager::TextureMapType textureType, DSTextureParam*paramtype = NULL);

	DSDatablock(IdString name, HlmsDS *creator,
			const HlmsMacroblock *macroblock, const HlmsBlendblock *blendblock,
			const HlmsParamVec &params, bool derived = false);
	virtual ~DSDatablock();
	float randf();
	float randf2();

	enum Status {
		CLEAN, DIRTY, BROKEN
	};

	Status status;



	bool safeForRewrite;
	bool staticMaterial = true;

	Ogre::Vector3 randv();
	Ogre::Vector3 randv2();
	Ogre::Vector3 randv3();
	void markForUpdate();
	void syncWithGPU();
	void bakeTextures();
//    void bakeTextures();

	HlmsDS *mDSCreator;

	bool UpdateAllowed = false;
	bool JsonMaterial=false;


	int texunitstart=0;
	void frameEnded();

	void createConstBuffer(HlmsDS *creator);
	ConstBufferPacked *getMaterialBuffer();

	size_t getMaterialBufferSize();

	void calculateHash();

	virtual void initializeParamTypes();

//    void initParam(String paramVal,DSMaterialParam* param);
//    void initTextureUnit(String paramVal,DSTextureParamType * type,HlmsManager *hlmsManager);

	void generateControl(bool staticd = false);
	void setData(const HlmsParamVec &params);

	void setParam(Ogre::String, Ogre::String);
	void setParam(Ogre::String, Ogre::Vector4);
	void setParam(Ogre::String, float);

	const Ogre::Vector4* const getParam(Ogre::String);
	float*         getParam(Ogre::String,uint &size);


	float * getAutoParam(uint id);

	void initializeMaterial(const HlmsParamVec &params, HlmsDS *creator,
			DBParser * customparser = NULL);

	virtual void initializeProperties();
	virtual void setProperties();

	void syncShadowParams();
	void generateShadowParams();
	bool isDirty();

	void createTexIndexPropertys(bool caster);
	void createVec4IndexPropertys(bool caster);

	std::vector<DSPropertyParam *>* getPropertyParams() {
		return propertyParams;
	}
	std::vector<DSPieceParam *>* getPieceParams() {
		return pieceParams;
	}

	Status getStatus() const;
	void setStatus(Status status);

	Ogre::String getDName();

	void clear();
	ParamVecParser*& getMparser();
	void setMparser(ParamVecParser* mparser);

	void createMaterial();

	const HlmsSamplerblock* getSamplerBlock(HlmsSamplerblock sb);
	const HlmsSamplerblock* getSamplerBlock();
	HlmsDS* getDsCreator();
	bool isJsonMaterial() const;
	void setJsonMaterial(bool jsonMaterial);

	bool hasprop(Ogre::String str);
	bool hasparam(Ogre::String str);



};

} /* namespace Ogre */

#endif /* DATABLOCKS_DSDATABLOCK_H_ */
