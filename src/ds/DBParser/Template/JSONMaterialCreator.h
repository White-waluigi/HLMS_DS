/*
 * JSONMaterialCreator.h
 *
 *  Created on: Dec 25, 2016
 *      Author: marvin
 */

#ifndef SRC_DBPARSER_TEMPLATE_JSONMATERIALCREATOR_H_
#define SRC_DBPARSER_TEMPLATE_JSONMATERIALCREATOR_H_
#include "DBParser/JsonParser.h"
#include "DBParser/Template/MTMultiData.h"
#include "Helper/DSShaderGenerator.h"
#include "Datablocks/Data/DSBlocks/MappingBlocks/DSMappingBlock.h"
#include "Datablocks/AutoParams/DSAutoParamParser.h"
namespace Ogre {
class DSDatablock;
class DSTextureParam;
class DSMaterialParam;

class JSONMaterialCreator {

	DSDatablock *db;
	DSShaderGenerator::ShaderCode helper;
	HlmsJson::NamedBlocks blocks;
	DSAutoParamParser *APparser=NULL;


public:
	JSONMaterialCreator();
	virtual ~JSONMaterialCreator();

	void initializeMaterial(DSDatablock *qi,MaterialTemplate *materialTemplate,const HlmsJson::NamedBlocks& blocks);

	void initializeMeta(MT_MultiData *mt);
	void initializeSettings(MT_MultiData *mt);
	void initializeParams(MT_MultiData *mt);

	void initializeAutoparam(MT_MultiData *mt);

	void initializeMappingBlock(MT_MultiData *mt);
	void initializePaired(MT_MultiData *mt);
	void initializeTexture(MT_MultiData *mt);
	void initializeVariable(MT_MultiData *mt);

	void initializePiece(MT_MultiData *mt);
	void initializeProperty(MT_MultiData *mt);

	void initVarData(DSMaterialParam* param, MT_MultiData* mt,Ogre::String datatype,Ogre::String key);
	void initTexData(DSTextureParam* param, MT_MultiData* mt,Ogre::String key,Ogre::String datatype);

	void quickSet(Ogre::String, int i);
	void quickSet(Ogre::String);


};

} /* namespace Ogre */

#endif /* SRC_DBPARSER_TEMPLATE_JSONMATERIALCREATOR_H_ */
