/*
 * Museum.h
 *
 *  Created on: Jun 20, 2016
 *      Author: marvin
 */

#ifndef SRC_MUSEUM_MUSEUM_H_
#define SRC_MUSEUM_MUSEUM_H_

#include "OgrePrerequisites.h"
#include "OgreHlmsDS.h"
#include "OgreSceneManager.h"
#include "OgreVector3.h"
#include "OgrePlatform.h"

namespace Demo {

class Museum {
	struct exhibit{
		int exID=0;

		Ogre::String model;
		int pedastol;

		bool floating=false;
		float fphase=0.0;
		Ogre::Vector3 scale=Ogre::Vector3::UNIT_SCALE;

		float swing=0.0;
		float soperator=1.0;
		float sphase=0.0;
		Ogre::uint32 rqID=0x00000001;
		int side=0;
		bool shadow=true;
		Ogre::String label="";
		bool infiniteAABB=false;
		std::vector<Ogre::String> materialParams;
		std::vector<Ogre::String> materialParams1;
		std::vector<Ogre::String> materialParams2;

		Ogre::DSDatablock * datablock;
		Ogre::DSDatablock * datablock1;
		Ogre::DSDatablock * datablock2;
		Ogre::Item * item;
		Ogre::SceneNode* sn;

		Ogre::Vector3 position=Ogre::Vector3::ZERO;
		bool positionAutoOffset=true;
		Ogre::Vector3 positionoffset=Ogre::Vector3::ZERO;
		Ogre::Vector3 direction=Ogre::Vector3(0,0,-1);

		Ogre::String blendType;
		Ogre::String compareFunc;
	};
	struct label{
		Ogre::String description;
		Ogre::Vector3 position;
		Ogre::Vector3 direction;

		Ogre::DSDatablock * datablock;
		Ogre::Item * item;
		Ogre::SceneNode *sn;
	};
	struct Mlight{
		Ogre::Light * light;
		Ogre::Vector3 position;
		Ogre::Vector3 direction;
		exhibit* swing=NULL;
		float sphase;
		Ogre::DSDatablock * datablock;
		Ogre::Item * item;
		Ogre::SceneNode *sn;
		bool animate=false;
	};

	Ogre::DSDatablock * museumDatablock;
	Ogre::DSDatablock * museumDatablockf;
	Ogre::Item * museumItem;
	Ogre::SceneNode *museumSn;
	Ogre::DSDatablock * woodDatablock;
	Ogre::Item * woodItem;
	Ogre::SceneNode* woodSn;

	std::vector<exhibit> exhibits;
	std::vector<label> 	labels;
	std::vector<Mlight> 	lights;


	Ogre::HlmsDS * hlmsDS;
	Ogre::SceneManager * sceneManager;

	Ogre::DSDatablock* pedastaldb;
	Ogre::DSDatablock* labeldb;

	Ogre::CullingMode cm=Ogre::CULL_CLOCKWISE;

	float timer=0;

public:

	struct daystruct{
		float morningweight;
		float dayweight;
		float eveningweight;
		float nightweight;

		float dayphase;
	};

	Ogre::SceneNode * museum=0;
    bool slowDown=true;

	Museum(Ogre::HlmsDS * hlmsDS,Ogre::SceneManager * sceneManager);

	void initExhibits();
	void initLights();
	void setupMuseum();
	void setupExhibits();
	void setupLabels(Ogre::Vector3 pos,Ogre::String label);
	void setupLights();

	daystruct updateLights(float t);
	void updateExhibits(float t);
	Ogre::DSDatablock * getDatablock(std::vector<Ogre::String> params,Ogre::String name,exhibit exhibit);
	Ogre::Vector3 setupPedestal(Ogre::Vector3 pos);

	float randf(int treshhold =0);



	virtual ~Museum();
};

} /* namespace Demo */

#endif /* SRC_MUSEUM_MUSEUM_H_ */
