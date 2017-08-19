/*
 * JsonParser.cpp
 *
 *  Created on: Nov 17, 2016
 *      Author: marvin
 */

#include <iostream>


#include "DBParser/JsonParser.h"
#include "Datablocks/DSDatablock.h"
#include "DBParser/Template/MaterialTemplate.h"
#include "DBParser/Template/JSONMaterialCreator.h"
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
#ifndef NDEBUG
    name=datablock->getName().mDebugString;

#endif
#endif
//    std::cout <<"#####################################JSON: "<<name<<"#####################################\n";
   // printValue(json,1);

    //Convert JSON into MT_Multidata Object, which can be Merged with templates

	QueueItem *test=new QueueItem();
	test->templ=new MaterialTemplate(json);
	test->blocks=blocks;
	test->datablock= db;

	//Input into Queued Templates temporarily
	QueuedTemplates[datablock->getName()]=test;
	bool change=true;
	int loopcounter=0;

	//Loop until the lower loop didn't lead to a Merge
	while(change){

		//Insert  the Material in QueuedTemplates if not all templates required are found. Once they are, insert into MergedTemplates
		loopcounter++;
		change=false;
		for(std::map<Ogre::IdString,QueueItem *>::iterator iterator = QueuedTemplates.begin(); iterator != QueuedTemplates.end(); iterator++) {
			try{
				bool requiresTemplate=false;

				Ogre::String s;

				//Does it even need a template?
				if(iterator->second->templ->data->getObject("meta").o->has("template")){
					MT_MultiData* mto= iterator->second->templ->data->getObject("meta").o;

					s=mto->getData("template").s;
					requiresTemplate=true;
				}
				//Does it not require a Template or is the rquired Template in the list?
				//If neither wait until it is found
				if(!requiresTemplate||MergedTemplates.find(s)!=MergedTemplates.end()){
					change=true;

					MaterialTemplate* templ=new MaterialTemplate( iterator->second->templ);
					if(requiresTemplate){

						//Merge with Template
						templ=new MaterialTemplate( MergedTemplates.find(s)->second->templ);
						templ->RemoveMeta();
						templ->Merge(iterator->second->templ);

					}


					MT_MultiData* mto= iterator->second->templ->data->getObject("meta").o;

					s=mto->getData("type").s;

					//If its an actual Material and not just a template, create the Material
					if(s.compare("material")==0){
						createMaterial(templ,iterator->second, blocks);
					}
					change=true;
					MergedTemplates[iterator->first]=iterator->second;
					QueuedTemplates.erase(iterator->first);
					//templ->data->print(0);

				}

			}catch (Ogre::Exception *e){
			    Ogre::String name=iterator->first.getReleaseText();
			#ifdef OGRE_DEBUG_MODE
			#ifndef NDEBUG
			    name=datablock->getName().mDebugString;
			#endif
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
