/*
 * DSDatablock.h
 *
 *  Created on: Jan 4, 2018
 *      Author: marvin
 */

#ifndef SRC_DS_DATABLOCK_DSDATABLOCK_H_
#define SRC_DS_DATABLOCK_DSDATABLOCK_H_
#include "Ogre.h"
#include "OgreHlmsDatablock.h"
#include <OgreHlmsTextureManager.h>
#include <OgreConstBufferPool.h>
#include <OgreHeaderPrefix.h>
#include "../Modules/ModuleReference.h"
#include "../Modules/Resources/DSResource.h"

namespace Ogre {
class HlmsDS;
class Parser;
class DSDatablock: public Ogre::HlmsDatablock, public ModuleReference{
	std::list<DSResource *> mRes;

	//To easily identfiy idndividual datablocks and to avoid empty MaterialBuffers
	Ogre::Vector4 mIDColor;
	bool mSafeForRewrite=true;
	bool mClean=false;
	bool mUpdateAllowed=true;
	int mResourceNumber=1;

	Parser * mLastParser=NULL;
	HlmsDS * mCreator=NULL;

	ConstBufferPacked* mMaterialBuffer=NULL;


	void createMaterialBuffer();
	int GetTexArraySize();
public:
	DSDatablock(IdString name, HlmsDS *creator,
			const HlmsMacroblock *macroblock, const HlmsBlendblock *blendblock,
			const HlmsParamVec &params, bool derived = false);


	void initalize(Parser * parser);

	void upload();

	//Mark as desynchronized
	void soil();
	//Mark as synchronized
	void clean();

	DSResource::propertyList genProperties();
	std::map<IdString,String> genPieces();
	DSResource::TextureRef::textureBuffList genTextureBufs();

	void addResource(DSResource *);
	int getBufferSize();
	ConstBufferPacked * getMaterialBuffer();
	virtual ~DSDatablock();
	const Ogre::Vector4& getIdColor() const;
	void setIdColor(const Ogre::Vector4& idColor);
	int newIncRSNum();
};

} /* namespace Ogre */

#endif /* SRC_DS_DATABLOCK_DSDATABLOCK_H_ */
