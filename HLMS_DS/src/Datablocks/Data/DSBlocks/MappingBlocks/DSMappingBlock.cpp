/*
 * DSMappingBlock.cpp
 *
 *  Created on: Jan 4, 2017
 *      Author: marvin
 */

#include "DSMappingBlock.h"
#include "DSDefaultMBFactory.h"
#include "DSMBModulator.h"
#include "../../DSTextureParam.h"
namespace Ogre {

DSMappingBlock::DSMappingBlock(MT_MultiData * mt) {
	// TODO Auto-generated constructor stub
	this->modulatorFactorys.push_back(new DSDefaultMBFactory(this));


	this->UVindexOffset=0;
	this->matOffset=Matrix4::IDENTITY;
	this->BasematOffset=Matrix4::IDENTITY;

	timer.reset();
	initialize(mt);
}

DSMappingBlock::~DSMappingBlock() {

}
void Ogre::DSMappingBlock::modulate(DSTextureParam * parent) {
	matOffset=Matrix4::IDENTITY;

	for(uint i=0;i<this->modulators.size();i++){
		modulators[i]->modulate();
	}
 	parent->setTexMatOffset(BasematOffset*matOffset);


	parent->setVindexOffet(this->UVindexOffset);
}

void Ogre::DSMappingBlock::initialize(MT_MultiData* mt) {
	this->name=mt->key;

	float UVscalex=mt->getDataD("UVscalex",1.0).f;
	float UVscaley=mt->getDataD("UVscaley",1.0).f;
	float UVoffsetx=mt->getDataD("UVoffsetx",0.0).f;
	float UVoffsety=mt->getDataD("UVoffsety",0.0).f;


	if(mt->has("animation")&&mt->getObject("animation").o->has("VScroll")){
		UVoffsety=0;
	}

	BasematOffset.setTrans(Vector3(UVoffsetx,UVoffsety,0));
	BasematOffset.setScale(Vector3(UVscalex,UVscaley,1.0));



	if(!mt->has("animation")){
		return;
	}

	this->animated=true;
	for(uint i=0;i<modulatorFactorys.size();i++){
		for(int ii=0;ii<mt->getObject("animation").o->size();ii++){

			DSMBModulator* mod=  modulatorFactorys[i]->parseModulator(mt->getData("animation",ii).o);
			if(mod!=NULL){
				this->modulators.push_back(mod);
			}
		}

	}
}


} /* namespace Ogre */

 Ogre::Timer Ogre::DSMappingBlock::getTimer()  {
	return timer;
}

void Ogre::DSMappingBlock::setTimer( Ogre::Timer timer) {
	this->timer = timer;
}
