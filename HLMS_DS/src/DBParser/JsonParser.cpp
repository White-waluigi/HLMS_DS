/*
 * JsonParser.cpp
 *
 *  Created on: Nov 17, 2016
 *      Author: marvin
 */

#include "JsonParser.h"
#include "../Datablocks/DSDatablock.h"
#include "Template/MaterialTemplate.h"
#include "Template/JSONMaterialCreator.h"
#include <iostream>
namespace Ogre {




void JsonParser::saveMaterial(const HlmsDatablock* datablock,
		String& outString) {
}

void JsonParser::collectSamplerblocks(const HlmsDatablock* datablock,
		set<const HlmsSamplerblock*>::type& outSamplerblocks) {
    assert( dynamic_cast<const DSDatablock*>(datablock) );


}

void JsonParser::loadMaterial(const rapidjson::Value& json,
		const HlmsJson::NamedBlocks& blocks, HlmsDatablock* datablock) {

    assert( dynamic_cast<DSDatablock*>(datablock) );
    db = static_cast<DSDatablock*>(datablock);
    Ogre::String name="";
#ifdef OGRE_DEBUG_MODE
    name=datablock->getName().mDebugString;
#endif
    std::cout <<"#####################################JSON: "<<name<<"#####################################\n";
   // printValue(json,1);

	String samplername="DoomSB";
	rapidjson::Value val;
	val.SetString("DoomSB");
	int inte=0;

	QueueItem *test=new QueueItem();
	test->templ=new MaterialTemplate(json);
	test->blocks=blocks;
	test->datablock= db;

	QueuedTemplates[datablock->getName()]=test;
	bool change=true;
	int loopcounter=0;
	while(change){

		loopcounter++;
		change=false;
		for(std::map<Ogre::IdString,QueueItem *>::iterator iterator = QueuedTemplates.begin(); iterator != QueuedTemplates.end(); iterator++) {
			try{
				bool requiresTemplate=false;

				Ogre::String s;
				if(iterator->second->templ->data->getObject("meta").o->has("template")){
					MT_MultiData* mto= iterator->second->templ->data->getObject("meta").o;

					s=mto->getData("template").s;
					requiresTemplate=true;
				}
				if(!requiresTemplate||MergedTemplates.find(s)!=MergedTemplates.end()){
					change=true;

					MaterialTemplate* templ=new MaterialTemplate( iterator->second->templ);
					if(requiresTemplate){


						templ=new MaterialTemplate( MergedTemplates.find(s)->second->templ);
						templ->RemoveMeta();
						templ->Merge(iterator->second->templ);

					}

					MT_MultiData* mto= iterator->second->templ->data->getObject("meta").o;

					s=mto->getData("type").s;
					if(s.compare("material")==0){
						createMaterial(templ,iterator->second, blocks);
					}
					change=true;
					MergedTemplates[iterator->first]=iterator->second;
					QueuedTemplates.erase(iterator->first);
					templ->data->print(0);

				}

			}catch (Ogre::Exception *e){
			    Ogre::String name=iterator->first.getReleaseText();
			#ifdef OGRE_DEBUG_MODE
			    name=datablock->getName().mDebugString;
			#endif
				std::cout<<"Parsing Material "<<name<<" failed because "<<e->getDescription()<<"\n";
				assert(false);
				QueuedTemplates.erase(iterator->first);
			}
		}
	}
}

void JsonParser::parse(HlmsManager *hlmsManager){

}

JsonParser::JsonParser(HlmsManager* mHlmsManager,HlmsDS * ds) {
	this->mHlmsManager=mHlmsManager;
	this->parent=ds;
	db=NULL;
}

void JsonParser::printValue(const rapidjson::Value &json, int indent) {

}



void JsonParser::createMaterial(MaterialTemplate* materialTemplate,
		QueueItem* item,const HlmsJson::NamedBlocks& blocks) {
	//item->datablock->clear();
	JSONMaterialCreator jmc;
	jmc.initializeMaterial(item->datablock,materialTemplate,blocks);

}

JsonParser::~JsonParser() {
	// TODO Auto-generated destructor stub
}

} /* namespace Ogre */
