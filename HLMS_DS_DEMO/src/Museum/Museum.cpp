/*
 * Museum.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: marvin
 */

#include "Museum.h"
#include "OgreMesh2.h"
#include "OgreItem.h"
#include "OgreMesh.h"

#include "OgreSceneNode.h"
#include "../PbsMaterialsGameState.h"
#include "OgreMath.h"

namespace Demo {

Museum::Museum(Ogre::HlmsDS * hlmsDS, Ogre::SceneManager * sceneManager) {
	this->sceneManager = sceneManager;
	this->hlmsDS = hlmsDS;
	// TODO Auto-generated constructor stub

}

void Museum::initExhibits() {
	exhibit statue;

	statue.model = "Statue.mesh";
	statue.pedastol = 1;

	statue.materialParams.push_back("diffuse$0.7 0.7 0.2");
	statue.materialParams.push_back("opacity$0.6");
	statue.label = "transp";

	exhibits.push_back(statue);

	//************************************************************

	exhibit stone;

	stone.model = "Stone.mesh";
	stone.pedastol = 0;

	//stone.materialParams.push_back("normal_map$file=ghost_n.jpg");
	stone.materialParams.push_back("glow$1 1 1 1.0");
	stone.materialParams.push_back("glow_map$file=stone_g.png");

	stone.materialParams.push_back(
			"normal_map$file=ghost_n.jpg,scale-uv=3.0 3.0 3.0");

	stone.materialParams.push_back("diffuse$0.9 0.2 0.1 1.0");
	stone.materialParams.push_back("specular$0.1 0.1 0.1  32.0");
	stone.shadow = true;
	stone.label = "glow";
	exhibits.push_back(stone);

//************************************************************
	exhibit vase;

	vase.model = "Vase.mesh";
	vase.pedastol = 1;

	vase.materialParams.push_back("normal_map$'file=ghost_n.jpg,anim-uv=0.02'");

	vase.materialParams.push_back("diffuse$0.9 0.7 0.4 1.0");
	//vase.materialParams.push_back("glow$0.2 0 0.2");
	vase.label = "matrix";
	exhibits.push_back(vase);

	//************************************************************
	exhibit picture;
	//************************************************************

	exhibit Cagevase;

	Cagevase.model = "Vase.mesh";
	Cagevase.pedastol = 1;
	Cagevase.materialParams.push_back("opacity_map$file=cage.png,anim-uv=0.2");

	Cagevase.materialParams.push_back("opacity-sharp$1");
	Cagevase.materialParams.push_back("diffuse$0.9 0.7 0.0 1.0");
	Cagevase.materialParams.push_back("specular$1 1 1  32.0");

	//vase.materialParams.push_back("glow$0.2 0 0.2");
	Cagevase.label = "transp";
	exhibits.push_back(Cagevase);

	//************************************************************
	exhibit camera;

	camera.exID = 1;
	camera.model = "Camera.mesh";
	camera.pedastol = 0;
	//camera.materialParams.push_back("diffuse_map$file=ghost_n.jpg");

	camera.materialParams.push_back("glow$1 1 1 1.0");
	camera.materialParams.push_back("diffuse$0.07 0.08 0.09 1.0");
	camera.materialParams.push_back("glow_map$file=camerag.png");
	camera.materialParams.push_back("diffuse_map$file=camerad.png");

	//camera.materialParams.push_back("custom_post_material$'glow *= tan(pass.time.x);'");
	camera.materialParams.push_back(
			"custom_post_material$'glow *= tan(material.autoparam0.x)*2.0;'");
	camera.materialParams.push_back(
			"autoparam0$'type=time,time=2000,pulse=on'");

	camera.position = Ogre::Vector3(2.61811, 15.3257, -3.78816);
	camera.direction = Ogre::Vector3(1.5, -1, -1);

	camera.swing = 0.9;
	camera.shadow = false;

	exhibits.push_back(camera);

	//************************************************************
	exhibit sinbad;

	//************************************************************
	exhibit UFO;
	UFO.materialParams.push_back("glow$0 1 0 1.0");
	UFO.materialParams.push_back("opacity$1.0");

	//UFO.materialParams.push_back("custom_post_material$'opacity *= (pass.time.x*2.0)-0.5;'");
	UFO.materialParams.push_back(
			"custom_post_material$'opacity = ((material.autoparam0.x*2.0)-0.5);'");

	UFO.materialParams.push_back("autoparam0$'type=time,time=5000,pulse=on'");

	UFO.materialParams.push_back("diffuse$0 0 0 1.0");
	UFO.materialParams.push_back("specular$0.0 0.0 0.0  32.0");
	UFO.materialParams.push_back("glow$0.05 0.86 1 1.0");

	UFO.materialParams1.push_back("opacity$1.0");

	//UFO.materialParams1.push_back("custom_post_material$'opacity *= (pass.time.x*2.0)-0.5;'");
	UFO.materialParams1.push_back(
			"custom_post_material$'opacity = ((material.autoparam0.x*2.0)-0.5);'");

	UFO.materialParams1.push_back("autoparam0$'type=time,time=5000,pulse=on'");
	UFO.materialParams1.push_back("diffuse_map$file=hull.png");
	UFO.materialParams1.push_back("specular$1.0 1.0 1.0  32.0");

	UFO.label = "autoparam";

	UFO.model = "UFO.mesh";
	UFO.pedastol = 1;
	UFO.floating = true;

	exhibits.push_back(UFO);

	//************************************************************
	exhibit TV;

	//TV.materialParams.push_back("diffuse_map$'file=terminal@i.png,filter=none,anim-texture:17'");

	TV.materialParams.push_back("diffuse$0.002 0.002 0.002 1.0");

	TV.materialParams.push_back("specular$0.1 0.1 0.1  32.0");

	TV.materialParams1.push_back("diffuse$0.1 0.1 0.1 1.0");
	TV.materialParams1.push_back("glow$1 1 1 1.0");

	TV.materialParams1.push_back(
			"glow_map$'file=terminal@i.png,filter=none,anim-texture=17'");

	TV.materialParams1.push_back("specular$0.5 0.5 0.5  32.0");

	TV.label = "animated";
	TV.model = "TV.mesh";
	TV.pedastol = 1;

	exhibits.push_back(TV);
	//************************************************************
	exhibit hallImage;
	hallImage.materialParams.push_back("glow$ 0.0 0.0 0.0 1.0");

	hallImage.materialParams.push_back("diffuse_map$file=sinbad.png");
	hallImage.materialParams.push_back(
			"normal_map$file=painting_n.png,scale-uv=2.0 2.0 2.0");
	hallImage.materialParams.push_back("specular$0.03 0.03 0.03  32.0");

	hallImage.materialParams1.push_back("diffuse_map$file=frame.png");
	hallImage.materialParams1.push_back("specular$0.13 0.13 0.13  32.0");
	hallImage.model = "Image.mesh";
	hallImage.pedastol = 0;
	hallImage.position = Ogre::Vector3(-10.22549, 3.44192, -19.16379);

	exhibits.push_back(hallImage);

	//************************************************************
	exhibit BDImage;
	BDImage.materialParams.push_back("glow$ 1.5 1.5 1.5 1.0");
	BDImage.materialParams.push_back("diffuse_map$file=blooddragon.jpg");
	BDImage.materialParams.push_back("glow_map$file=blooddragon_g.jpg");
	BDImage.materialParams.push_back("specular$0.13 0.13 0.13  32.0");

	BDImage.materialParams1.push_back("diffuse_map$file=frame.png");
	BDImage.model = "Image.mesh";
	BDImage.pedastol = 0;
	BDImage.position = Ogre::Vector3(9.17253, 6.25655, -30.28128);
	BDImage.scale = Ogre::Vector3(3.0, 3.0, 1.0);

	exhibits.push_back(BDImage);
	//************************************************************
	exhibit Skydome;
	Skydome.compareFunc = "lequal";
	Skydome.materialParams.push_back("diffuse$0.0 0.0 0.0 0.0");
	Skydome.materialParams.push_back("glow$ 1.1 1.1 1.1 1.0");
	Skydome.materialParams.push_back(
			"glow_map$'file=skysphere.jpg,scale-uv=2 2 2'");
	Skydome.materialParams.push_back("specular$0.0 0.0 0.0  32.0");
	Skydome.materialParams.push_back("cdepth$2");
	Skydome.materialParams.push_back("noCamTransf$1");
	Skydome.infiniteAABB = true;

	Skydome.model = "Sphere1000.mesh";
	Skydome.pedastol = 0;
	Skydome.position = Ogre::Vector3(0, -5, 0);
	Skydome.scale = Ogre::Vector3(-40, 40, 40);
	Skydome.shadow = false;
	Skydome.skyanimate=true;
	exhibits.push_back(Skydome);
	//************************************************************

	exhibit goldstatue;

	goldstatue.model = "ogrehead.mesh";
	goldstatue.materialParams.push_back("reflection_map$file=spheremap.png");
	goldstatue.materialParams.push_back("reflection$0.3");

	goldstatue.materialParams.push_back("diffuse_map$file=RustySteel.jpg");

//	goldstatue.materialParams.push_back("specular$1.5 0.5 0.2 150.0");
	goldstatue.materialParams.push_back("specular$1 1 1 600.0");
	goldstatue.label = "reflection";
	goldstatue.positionoffset = Ogre::Vector3(0, 0.9, 0);
	goldstatue.scale = Ogre::Vector3(0.04, 0.04, 0.04);
	goldstatue.rqID = (0x00000001);
	goldstatue.pedastol = 1;
	exhibits.push_back(goldstatue);

	//************************************************************

	exhibit jollyroger;

	jollyroger.model = "terrain.mesh";
	jollyroger.materialParams.push_back("name$terrain");

//	jollyroger.materialParams.push_back("diffuse_map$file=terr_rock-dirt.jpg,scale-uv=15 15 15");
//	//jollyroger.materialParams.push_back("specular$0.005 0.005 0.005  32.0");
//	jollyroger.materialParams.push_back("specular-map$file=tile_diffuse.tga,scale-uv=0.3 0.3 0.3");
//
//	//jollyroger.materialParams.push_back("reflection$0.3");
////	goldstatue.materialParams.push_back("specular$1.5 0.5 0.2 150.0");
//	jollyroger.label="reflect";
//	jollyroger.scale=Ogre::Vector3(1,1,1);
////	jollyroger.rqID=(0x00000001);
//	jollyroger.pedastol=0;
	jollyroger.position = Ogre::Vector3(0, 0.1, 0);
	jollyroger.pedastol = 0;
	jollyroger.label = "";
	exhibits.push_back(jollyroger);

	//************************************************************
	exhibit roof;

	roof.materialParams.push_back(
			"diffuse_map$file=tiles.jpg,scale-uv=10 10 10");
	roof.materialParams1.push_back(
			"diffuse_map$file=KAMEN320x240-bump.jpg,scale-uv=10 10 10");
	roof.materialParams2.push_back("opacity$0.5");
	roof.materialParams2.push_back("diffuse$0.3 0.3 0.8 0.5");

	roof.model = "roof.mesh";

	roof.label = "reflection";
	roof.pedastol = 0;
	roof.positionAutoOffset = false;
	exhibits.push_back(roof);

	//************************************************************

	for (int i = 0; i < 500; i++) {
		exhibit tree;

		tree.model = "tree.mesh";
		tree.materialParams.push_back("diffuse_map$file=bark.jpg");
		tree.materialParams1.push_back("diffuse_map$file=leafes.png");
		tree.materialParams1.push_back("opacity-diffuse$1");
		tree.materialParams1.push_back("opacity-sharp$1");
		tree.materialParams1.push_back("opacity-cutoff$3");
		tree.materialParams1.push_back("shadow_const_bias$0.001");
		//jollyroger.materialParams.push_back("reflection$0.3");
		//	goldstatue.materialParams.push_back("specular$1.5 0.5 0.2 150.0");
		//jollyroger.materialParams.push_back("diffuse$1 1 1");
		//	jollyroger.rqID=(0x00000001);
		tree.position = Ogre::Vector3((randf() * 250) - 75-(randf()*2-1)*100, -1,
				(randf() * 100) -150-(randf()*2-1)*100);

		tree.pedastol = 0;
		exhibits.push_back(tree);
	}
	//************************************************************

	exhibit firewood;

	firewood.model = "firewood.mesh";
	firewood.pedastol = 0;

	firewood.materialParams.push_back("diffuse_map$file=bark.jpg");

	firewood.position = Ogre::Vector3(56.63, -1, -105.136);

	exhibits.push_back(firewood);
	//************************************************************

	exhibit jsonstatue;

	jsonstatue.model = "Statue.mesh";
	jsonstatue.pedastol = 1;

	jsonstatue.materialParams.push_back("name$glowTiles");

	jsonstatue.label = "json";

	exhibits.push_back(jsonstatue);

	//************************************************************
	exhibit mask;

	mask.model = "mask.mesh";
	mask.pedastol = 1;
	mask.scale=Ogre::Vector3(3,3,3);
	mask.positionoffset=Ogre::Vector3(-1.2,0,1.5);
	mask.materialParams2.push_back("name$lens");
	mask.materialParams1.push_back("specular$1.0 1.0 1.0 150.0");
	mask.materialParams.push_back("diffuse_map$file=frame.png");

	exhibits.push_back(mask);

	//***********************************************************
	exhibit water;

	water.model = "Cube_d.mesh";

	water.materialParams.push_back("name$water");
	water.position = Ogre::Vector3(0, -20, 420);
	water.scale = Ogre::Vector3(2000, 0.1, 300);

	exhibits.push_back(water);
	//***********************************************************
	exhibit lighthouse;

	lighthouse.model = "lighthouse.mesh";

	lighthouse.materialParams.push_back("diffuse_map$file=lighthouse.png");
	lighthouse.position = Ogre::Vector3(880, 110, 500);

	exhibits.push_back(lighthouse);
	//************************************************************
//
//	exhibit forward;
//
//	forward.model = "ogrehead.mesh";
//	forward.materialParams.push_back("reflection_map$file=spheremap.png");
//	forward.materialParams.push_back("reflection$0.3");
//
//	forward.materialParams.push_back("diffuse_map$file=RustySteel.jpg");
//
////	goldstatue.materialParams.push_back("specular$1.5 0.5 0.2 150.0");
//	forward.materialParams.push_back("specular$1 1 1 600.0");
//	forward.label="reflect";
//	forward.positionoffset=Ogre::Vector3(0,0.9,0);
//	forward.scale=Ogre::Vector3(0.04,0.04,0.04);
//	forward.rqID=(0x000000F0);
//	forward.pedastol=1;
//	exhibits.push_back(forward);
//	//************************************************************
//
//
//	exhibit blendtest1;
//
//	blendtest1.blendType="replace";
//	blendtest1.compareFunc="gequal";
//
//	blendtest1.model = "Vase.mesh";
//	blendtest1.materialParams.push_back("diffuse$1 0 0 1");
//	blendtest1.materialParams.push_back("cdepth$3");
//
//	blendtest1.label="reflect";
//	blendtest1.scale=Ogre::Vector3(1,1,1);
//	blendtest1.rqID=(0x000000F0);
//	blendtest1.pedastol=1;
//	blendtest1.position=Ogre::Vector3(1,2,30);
//	exhibits.push_back(blendtest1);

//	//************************************************************
//	exhibit blendtest2;
//
//	//blendtest2.blendType="replace";
//	//blendtest2.compareFunc="apass";
//
//	blendtest2.model = "Vase.mesh";
//	blendtest2.materialParams.push_back("diffuse$0 1 0 1");
//	blendtest2.label="reflect";
//	blendtest2.scale=Ogre::Vector3(1,1,1);
//	blendtest2.rqID=(0x000000F0);
//	blendtest2.pedastol=1;
//	blendtest2.position=Ogre::Vector3(2,2,20);
//	exhibits.push_back(blendtest2);
//
//	//************************************************************
//	exhibit blendtest3;
//
//	//blendtest3.blendType="replace";
//	//blendtest3.compareFunc="apass";
//
//	blendtest3.model = "Vase.mesh";
//	blendtest3.materialParams.push_back("diffuse$0 0 1 1");
//	blendtest3.label="reflect";
//	blendtest3.scale=Ogre::Vector3(1,1,1);
//	blendtest3.rqID=(0x000000F0);
//	blendtest3.pedastol=1;
//	blendtest3.position=Ogre::Vector3(1,2,10);
//	exhibits.push_back(blendtest3);

	//************************************************************
}

void Museum::setupMuseum() {
	Ogre::HlmsParamVec pv;
	Ogre::HlmsBlendblock * test = new Ogre::HlmsBlendblock();

	{
		pv = Ogre::HlmsParamVec();

		//pv.push_back(std::make_pair("diffuse_map","file=checker.png"));
		//pv.push_back(std::make_pair("opacity_map","'file=text.png,filter:none'"));

		//pv.push_back(std::make_pair("custom_post_material","'opacity *= (material.autoparam0*2.0)-0.5;'"));
		//pv.push_back(std::make_pair("autoparam0","'type:time,time:5000,pulse:on'"));

		//pv.push_back(std::make_pair("opacity","0.5"));
		Ogre::HlmsMacroblock * mb = new Ogre::HlmsMacroblock();

		mb->mCullMode = cm;

		pv.push_back(std::make_pair("specular", "1.0 1.0 1.0 50.0"));
		pv.push_back(std::make_pair("diffuse_map", "file=MRAMOR6X6.jpg"));
		pv.push_back(
				std::make_pair("specular_map",
						"file=tile_diffuse.tga,scale-uv=0.3 0.3 0.3"));
		std::sort(pv.begin(), pv.end());
		museumDatablock =
				static_cast<Ogre::DSDatablock*>(hlmsDS->createDatablock(
						"building", "building", *mb, *test, pv));

		pv = Ogre::HlmsParamVec();
		pv.push_back(std::make_pair("specular", "0.3 0.3 0.3 40"));
		pv.push_back(
				std::make_pair("diffuse_map",
						"file=strugotine.jpg,scale-uv=10 10 10"));
		pv.push_back(
				std::make_pair("specular_map",
						"file=strugotine.jpg,scale-uv=10 10 10"));
		pv.push_back(
				std::make_pair("normal_map",
						"file=ghost_n.jpg,scale-uv=30.0 30.0 30.0"));

		std::sort(pv.begin(), pv.end());

		museumDatablockf =
				static_cast<Ogre::DSDatablock*>(hlmsDS->createDatablock(
						"buildingf", "building", *mb, *test, pv));

		Ogre::Item *museumItem = sceneManager->createItem("Building.mesh",
				Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
				Ogre::SCENE_DYNAMIC);
		//item->setDatablock(pole_db);
		museumItem->setDatablock(museumDatablock);
		museumItem->getSubItem(1)->setDatablock(museumDatablockf);

		//item->getSubItem(1)->setDatablock(pole_db);
		museumItem->setCastShadows(true);

		//museumItem->setCastShadows(true);
		museumSn =
				sceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->createChildSceneNode(
						Ogre::SCENE_DYNAMIC);
		museumItem->setVisibilityFlags(0x00000001);

		museumSn->setPosition(0, 0, 0);
		museumSn->attachObject(museumItem);
	}
	{
		pv = Ogre::HlmsParamVec();

		pv.push_back(std::make_pair("diffuse_map", "file=wood.jpg"));

		Ogre::HlmsMacroblock * mb = new Ogre::HlmsMacroblock();
		mb->mCullMode = cm;

		std::sort(pv.begin(), pv.end());
		woodDatablock = static_cast<Ogre::DSDatablock*>(hlmsDS->createDatablock(
				"wood", "wood", *mb, *test, pv));

		Ogre::Item *woodItem = sceneManager->createItem("Wood.mesh",
				Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
				Ogre::SCENE_DYNAMIC);
		//item->setDatablock(pole_db);
		woodItem->setDatablock(woodDatablock);
		//item->getSubItem(1)->setDatablock(pole_db);

		woodItem->setCastShadows(true);
		woodSn =
				sceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->createChildSceneNode(
						Ogre::SCENE_DYNAMIC);
		woodItem->setVisibilityFlags(0x00000001);

		woodSn->setPosition(0, 0, 0);
		woodSn->attachObject(woodItem);
	}
}

void Museum::setupExhibits() {
	Ogre::HlmsParamVec pv;
	Ogre::HlmsBlendblock * test = new Ogre::HlmsBlendblock();

	Ogre::HlmsMacroblock * mb = new Ogre::HlmsMacroblock();
	mb->mCullMode = cm;
	pv.push_back(std::make_pair("diffuse_map", "file=wood.jpg"));

	pv.push_back(std::make_pair("specular", "0.1 0.1 0.1 15.0"));
	std::sort(pv.begin(), pv.end());

	pedastaldb = static_cast<Ogre::DSDatablock*>(hlmsDS->createDatablock(
			"pedastaldb", "pedastaldb", *mb, *test, pv));
	pv = Ogre::HlmsParamVec();

	pv.push_back(std::make_pair("reflection_map", "file=spheremap.png"));
	pv.push_back(std::make_pair("reflection", "0.9"));

	pv.push_back(std::make_pair("diffuse", "0.11 0.11 0.1 1"));
	pv.push_back(std::make_pair("specular", "1.0 1.0 1.0 60.0"));
	std::sort(pv.begin(), pv.end());
	labeldb = static_cast<Ogre::DSDatablock*>(hlmsDS->createDatablock("labeldb",
			"labeldb", *mb, *test, pv));
	int exhibitoffset = 0;

	for (int i = 0; i < exhibits.size(); i++) {

		if(exhibits[i].processed)
			continue;

		if (exhibits.at(i).positionAutoOffset
				&& exhibits.at(i).position == Ogre::Vector3::ZERO) {
			exhibits.at(i).position =  Ogre::Vector3((exhibitoffset + 1) * 5, 0,
					-25);
			exhibitoffset++;
		}
		if (exhibits.at(i).direction == Ogre::Vector3::ZERO) {
			exhibits.at(i).direction = Ogre::Vector3::UNIT_X;
		}

		exhibits.at(i).item = sceneManager->createItem(exhibits.at(i).model,
				Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
				Ogre::SCENE_DYNAMIC);
		if (exhibits.at(i).infiniteAABB) {
			exhibits.at(i).item->setLocalAabb(
					Ogre::Aabb(Ogre::Aabb::BOX_INFINITE));
		}
		if (exhibits.at(i).materialParams.size() != 0) {
			exhibits.at(i).datablock = getDatablock(
					exhibits.at(i).materialParams, exhibits.at(i).model,
					exhibits.at(i));
			exhibits.at(i).item->setDatablock(exhibits.at(i).datablock);
		}
		if (exhibits.at(i).materialParams1.size() != 0) {
			exhibits.at(i).datablock1 = getDatablock(
					exhibits.at(i).materialParams1, exhibits.at(i).model,
					exhibits.at(i));
			exhibits.at(i).item->getSubItem(1)->setDatablock(
					exhibits.at(i).datablock1);
		}
		if (exhibits.at(i).materialParams2.size() != 0) {
			exhibits.at(i).datablock2 = getDatablock(
					exhibits.at(i).materialParams2, exhibits.at(i).model,
					exhibits.at(i));
			exhibits.at(i).item->getSubItem(2)->setDatablock(
					exhibits.at(i).datablock2);
		}

		//exhibits.at(i).item->setDatablock(exhibits.at(i).datablock);

		exhibits.at(i).item->setCastShadows(exhibits.at(i).shadow);
		exhibits.at(i).sn =
				sceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->createChildSceneNode(
						Ogre::SCENE_DYNAMIC);
		exhibits.at(i).item->setVisibilityFlags(exhibits.at(i).rqID);

		Ogre::Vector3 pedaloffset = Ogre::Vector3(0, 0, 0);

		if (exhibits.at(i).pedastol) {
			pedaloffset = setupPedestal(exhibits.at(i).position);
		}

		if (exhibits.at(i).label.compare("") != 0) {
			setupLabels(exhibits.at(i).position, exhibits.at(i).label);
		}
		exhibits.at(i).position += pedaloffset;
		exhibits.at(i).sn->setPosition(
				exhibits.at(i).position + exhibits.at(i).positionoffset);
		exhibits.at(i).sn->setScale(exhibits.at(i).scale);
		exhibits.at(i).sn->setDirection(exhibits.at(i).direction);

		exhibits.at(i).sn->attachObject(exhibits.at(i).item);

		exhibits[i].processed=true;
	}
}

void Museum::setupLabels(Ogre::Vector3 pos, Ogre::String label) {
	Ogre::HlmsParamVec pv;
	Ogre::HlmsBlendblock * test = new Ogre::HlmsBlendblock();

	pv = Ogre::HlmsParamVec();

	Ogre::HlmsMacroblock * mb = new Ogre::HlmsMacroblock();
	mb->mCullMode = cm;

	pv.push_back(std::make_pair("diffuse_map", "file=" + label + ".jpg"));

	pv.push_back(std::make_pair("specular", "0.03 0.03 0.03 15.0"));
	std::sort(pv.begin(), pv.end());

	Ogre::DSDatablock* labeltextdb =
			static_cast<Ogre::DSDatablock*>(hlmsDS->createDatablock(
					"labeldb" + Ogre::StringConverter::toString(rand()) + label,
					"labeldb", *mb, *test, pv));

	Ogre::Item *pedastalItem = sceneManager->createItem("Label.mesh",
			Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
			Ogre::SCENE_DYNAMIC);
	//item->setDatablock(pole_db);
	pedastalItem->getSubItem(0)->setDatablock(labeldb);
	pedastalItem->getSubItem(1)->setDatablock(labeltextdb);

	//item->getSubItem(1)->setDatablock(pole_db);

	pedastalItem->setCastShadows(true);
	Ogre::SceneNode* pedastalSn = sceneManager->getRootSceneNode(
			Ogre::SCENE_DYNAMIC)->createChildSceneNode(Ogre::SCENE_DYNAMIC);
	pedastalItem->setVisibilityFlags(0x00000001);
	pedastalItem->setCastShadows(true);

	pedastalSn->setPosition(pos - Ogre::Vector3(0, 0, -3));
	pedastalSn->attachObject(pedastalItem);
}

Ogre::Vector3 Museum::setupPedestal(Ogre::Vector3 pos) {
	Ogre::HlmsParamVec pv;

	pv = Ogre::HlmsParamVec();

	Ogre::HlmsMacroblock * mb = new Ogre::HlmsMacroblock();
	mb->mCullMode = cm;

	std::sort(pv.begin(), pv.end());

	Ogre::Item *pedastalItem = sceneManager->createItem("Pedastal.mesh",
			Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
			Ogre::SCENE_DYNAMIC);
	//item->setDatablock(pole_db);
	pedastalItem->setDatablock(pedastaldb);
	//item->getSubItem(1)->setDatablock(pole_db);

	pedastalItem->setCastShadows(true);
	Ogre::SceneNode* pedastalSn = sceneManager->getRootSceneNode(
			Ogre::SCENE_DYNAMIC)->createChildSceneNode(Ogre::SCENE_DYNAMIC);
	pedastalItem->setVisibilityFlags(0x00000001);
	pedastalItem->setCastShadows(true);

	pedastalSn->setPosition(pos);
	pedastalSn->attachObject(pedastalItem);
	return Ogre::Vector3(0, 2, 0);
}

Museum::daystruct Museum::updateLights(float t) {
	timer += t * 0.1;

	daystruct retVal;
	for (int i = 0; i < lights.size(); i++) {
		if (lights.at(i).animate) {
			float sunphase = fmod((timer) / 2.0, M_PI * 2);
			float monthpase = fmod((timer) / 20.0, M_PI * 2 * 10);

			bool ismoon = false;
			float dayphase = sunphase / M_PI;

			if (sunphase > M_PI) {

				sunphase = (sunphase) - M_PI;
				ismoon = true;

			}

			PbsMaterialsGameState::wColor morning;
			morning.color = Ogre::Vector3(2.0, 1.3, 0.5);
			morning.color = Ogre::Vector3(2.0, 1.3, 0.5);
			morning.from = 0;
			morning.to = 0.3;

			PbsMaterialsGameState::wColor sun;
			sun.color = Ogre::Vector3(1.6, 1.6, 1.4);
			sun.color = Ogre::Vector3(1.6, 1.6, 1.4);
			sun.from = 0.1;
			sun.to = 0.9;

			PbsMaterialsGameState::wColor evening;
			evening.color = Ogre::Vector3(2.0, 1.0, 0.1);
			evening.color = Ogre::Vector3(2.0, 1.0, 0.1);
			evening.from = 0.8;
			evening.to = 1;

			PbsMaterialsGameState::wColor night;
			night.color = Ogre::Vector3(0.03, 0.03, 0.1);
			night.scolor = Ogre::Vector3(1.03, 1.03, 1.1);
			night.from = 0.9;
			night.to = 0.1;

//
//			PbsMaterialsGameState::wColor morning;
//			morning.color=Ogre::Vector3(1.0,0.1,0.2);
//			morning.from=0.0;
//			morning.to=0.3;
//
//			PbsMaterialsGameState::wColor sun;
//			sun.color=Ogre::Vector3(1.0,1.0,0.8);
//			sun.from=0.2;
//			sun.to=0.6;
//
//			PbsMaterialsGameState::wColor evening;
//			evening.color=Ogre::Vector3(1.0,0.1,0.1);
//			evening.from=0.5;
//			evening.to=0.9;
//
//
//			PbsMaterialsGameState::wColor nightstart;
//			nightstart.color=Ogre::Vector3(0.0,0.0,0.0);
//			nightstart.from=0.8;
//			nightstart.to=1.2;
//
//			PbsMaterialsGameState::wColor night;
//			night.color=Ogre::Vector3(0.03,0.03,0.30);
//			night.from=1.0;
//			night.to=2.0;

			//PbsMaterialsGameState::wColor array[4]={morning,sun,evening,night};
			std::vector<PbsMaterialsGameState::wColor *> array;
			array.push_back(&morning);
			array.push_back(&sun);
			array.push_back(&evening);
			array.push_back(&night);

			Ogre::Vector3 col = PbsMaterialsGameState::between(array, 4,
					dayphase);

			retVal.dayphase = dayphase;
			retVal.eveningweight = evening.weight;
			retVal.morningweight = morning.weight;
			retVal.nightweight = night.weight;
			retVal.dayweight = sun.weight;

			Ogre::ColourValue c = Ogre::ColourValue(col.x, col.y, col.z);

			Ogre::Vector3 x = Ogre::Vector3(cos(sunphase), sin(sunphase) * -1.0,
					-cos(monthpase + 0.5));
//			Ogre::ColourValue c=Ogre::ColourValue(abs(3.0-sin(timer/2.0)*3.0),1.0,1.0);
			for(int i=0;i<exhibits.size();i++){
				if(exhibits[i].skyanimate){
					exhibits[i].datablock->setParam("glow",Ogre::Vector4(c.r,c.g,c.b,1));
				}
			}
			lights.at(i).light->setDirection(x);
			lights.at(i).light->setDiffuseColour(c);
			if(!ismoon){
				lights.at(i).light->setSpecularColour(c);
			}else{
				lights.at(i).light->setSpecularColour(Ogre::ColourValue(1,1,1,1));
			}
		}
	}
	return retVal;

}

void Museum::updateExhibits(float t) {

	for (int i = 0; i < exhibits.size(); i++) {

		//exhibits.at(i).direction=(exhibits.at(i).direction+Ogre::Vector3(0,0.000001,0)).normalise();
		Ogre::Vector3 curpos = exhibits.at(i).position
				+ exhibits.at(i).positionoffset;
		Ogre::Vector3 curdir = exhibits.at(i).direction;
		float yaw = 0;

		if (exhibits.at(i).floating != 0.0) {
			exhibits.at(i).fphase += t * 0.5;
			exhibits.at(i).fphase = fmod(exhibits.at(i).fphase, 1.0);

			float phasef = (exhibits.at(i).fphase * 2.0) - 1.0;
			float phase = phasef;
			if (phasef < 0) {
				phase = 0 - phasef;
			}

			curpos = curpos + (Ogre::Vector3(0, cos(sin(phase)), 0));
		}
		float phasef = 0;
		if (exhibits.at(i).swing != 0.0) {
			exhibits.at(i).sphase += t * 0.5 * exhibits.at(i).soperator;
			//exhibits.at(i).sphase=fmod(exhibits.at(i).sphase,1.0);
			if ((exhibits.at(i).sphase < 0)) {
				exhibits.at(i).soperator = abs(exhibits.at(i).soperator);
			}
			if ((exhibits.at(i).sphase > 1)) {
				exhibits.at(i).soperator = -abs(exhibits.at(i).soperator);
			}
			phasef = ((exhibits.at(i).sphase * 2.0) - 1.0);
			float phase = phasef;

			//curdir=(curdir+Ogre::Vector3(0,phase,0)).normalisedCopy();
			yaw = phase * (slowDown ? 50 : 10);

		}
		Ogre::Quaternion quat;
		exhibits.at(i).sn->yaw(Ogre::Degree(yaw * t));
		exhibits.at(i).sn->setPosition(curpos);
		//exhibits.at(i).sn->setDirection(exhibits.at(i).direction);

	}
}
void Museum::setupLights() {
	sceneManager->setAmbientLight(Ogre::ColourValue(0.005f, 0.005f, 0.02f),
			Ogre::ColourValue(0.005f, 0.012f, 0.01f),
			Ogre::Vector3(1, 1, 1) + Ogre::Vector3::UNIT_Y * 0.2f);
	sceneManager->setAmbientLight(Ogre::ColourValue(0.02f, 0.015f, 0.05f),
			Ogre::ColourValue(0.01f, 0.22f, 0.2f),
			Ogre::Vector3(1, 1, 1) + Ogre::Vector3::UNIT_Y * 0.2f);
	for (int i = 0; i < lights.size(); i++) {
		if(lights[i].processed)
			continue;

		Ogre::SceneNode *rootNode = sceneManager->getRootSceneNode();

		if (lights.at(i).swing != NULL) {
			lights.at(i).sn = lights.at(i).swing->sn->createChildSceneNode();
		} else {
			lights.at(i).sn = rootNode->createChildSceneNode();

		}

		lights.at(i).sn->attachObject(lights.at(i).light);
		lights.at(i).sn->setPosition(lights.at(i).position);

		lights.at(i).light->setDirection(lights.at(i).direction);
		lights.at(i).light->setVisible(lights.at(i).visible);
		lights.at(i).processed=true;
	}
}
void Museum::initLights() {

	Mlight Sun;
	Sun.light = sceneManager->createLight();

	Sun.light->setDiffuseColour(1.5f, 1.3f, 1.2f); //Warm
	Sun.light->setSpecularColour(0.8f, 0.8f, 0.8f);
	Sun.light->setPowerScale(2.0);
	Sun.light->setType(Ogre::Light::LT_DIRECTIONAL);
	Sun.light->setShadowFarDistance(2000);

	Sun.position = Ogre::Vector3(5, 5.0f, -5);
	//exhibitL1.light->setDirection(Ogre::Vector3(1, -1, -1).normalisedCopy());
	Sun.light->setAttenuationBasedOnRadius(10.0f, 0.01f);
	Sun.light->setSpotlightInnerAngle(Ogre::Radian(0));

	Sun.light->setCastShadows(true);
	Sun.animate = true;
	Sun.direction = Ogre::Vector3(0.3, -0.2, -1).normalisedCopy();
	lights.push_back(Sun);
//************************************
	Mlight mobileLight;

	mobileLight.light = sceneManager->createLight();
	mobileLight.light->setDiffuseColour(10.7f, 7.8f, 6.8f); //Warm
	mobileLight.light->setSpecularColour(10.7f, 7.8f, 6.8f); //Warm
	mobileLight.light->setPowerScale(20);

	//firelight.light->setSpecularColour(20.9f, 7.8f, 2.6f); //Warm
	mobileLight.light->setType(Ogre::Light::LT_SPOTLIGHT);
	mobileLight.light->setAttenuationBasedOnRadius(60.0f, 0.01f);
	mobileLight.position = Ogre::Vector3(16.63, 5, -105.136);
	mobileLight.light->setCastShadows(true);
	mobileLight.direction = Ogre::Vector3(0.0, -0.8, -1).normalisedCopy();
	mobileLight.visible=true;

	this->mobileLight=lights.size();

	lights.push_back(mobileLight);

	//******************************************
	Mlight exhibitL1;

	exhibitL1.light = sceneManager->createLight();

	exhibitL1.light->setDiffuseColour(1.0f, 1.0f, 0.0f); //Warm
	exhibitL1.light->setSpecularColour(1.0f, 1.0f, 0.0f);
	exhibitL1.light->setPowerScale(5.0);
	exhibitL1.light->setType(Ogre::Light::LT_SPOTLIGHT);
	exhibitL1.position = Ogre::Vector3(50, 10.0f, -5);
	//exhibitL1.light->setDirection(Ogre::Vector3(1, -1, -1).normalisedCopy());
	exhibitL1.light->setAttenuationBasedOnRadius(10.0f, 0.01f);
	exhibitL1.light->setSpotlightInnerAngle(Ogre::Degree(0));

	exhibitL1.light->setCastShadows(true);
	exhibitL1.direction = Ogre::Vector3(-2.0, -0.2, -1).normalisedCopy();
	lights.push_back(exhibitL1);


	//******************************
	Mlight MainExhibitL1;

	//MainExhibitL1.light=sceneManager->createLight();
	//lights.push_back(MainExhibitL1);

	//******************************
	Mlight MainExhibitL2;

	//MainExhibitL2.light=sceneManager->createLight();
	//lights.push_back(MainExhibitL2);

	//******************************
	Mlight hallL;
	hallL.light = sceneManager->createLight();
	hallL.light->setDiffuseColour(0.9f, 0.8f, 0.6f); //Warm
	hallL.light->setSpecularColour(0.9f, 0.8f, 0.6f); //Warm
	hallL.light->setType(Ogre::Light::LT_POINT);
	hallL.light->setAttenuation(40, 1, 0.01, 0.1);
	hallL.position = Ogre::Vector3(-10, 5.0f, -15);
	hallL.light->setCastShadows(false);

	lights.push_back(hallL);
	//******************************
	Mlight firelight;
	firelight.light = sceneManager->createLight();
	firelight.light->setDiffuseColour(20.7f, 7.8f, 2.8f); //Warm

	//firelight.light->setSpecularColour(20.9f, 7.8f, 2.6f); //Warm
	firelight.light->setType(Ogre::Light::LT_POINT);
	firelight.light->setAttenuation(80, 0.1, 0.001, 0.05);
	firelight.position = Ogre::Vector3(56.63, 2, -105.136);
	firelight.light->setCastShadows(false);

	lights.push_back(firelight);
	//******************************
	Mlight exhibitL2;

	exhibitL2.light = sceneManager->createLight();

	exhibitL2.light->setDiffuseColour(1.0f, 1.0f, 1.0f); //Warm
	exhibitL2.light->setSpecularColour(1.0f, 1.0f, 1.0f);
	exhibitL2.light->setPowerScale(3.0);
	exhibitL2.light->setType(Ogre::Light::LT_SPOTLIGHT);
	exhibitL2.position = Ogre::Vector3(0, 10.0f, -5);
	//exhibitL1.light->setDirection(Ogre::Vector3(1, -1, -1).normalisedCopy());
	exhibitL2.light->setAttenuationBasedOnRadius(20.0f, 0.01f);
	exhibitL1.light->setSpotlightInnerAngle(Ogre::Degree(0));

	exhibitL2.light->setCastShadows(true);
	exhibitL2.direction = Ogre::Vector3(2.0, -0.2, -1).normalisedCopy();


	lights.push_back(exhibitL2);
	//******************************
	Mlight lhouselight;
	lhouselight.light = sceneManager->createLight();
	lhouselight.light->setDiffuseColour(1.07f, 0.68f, 0.28f); //Warm
	lhouselight.light->setSpecularColour(1.07f, 0.68f, 0.28f); //Warm

	lhouselight.light->setType(Ogre::Light::LT_POINT);
	lhouselight.light->setAttenuation(80, 0.1, 0.01, 0.05);
	lhouselight.position = Ogre::Vector3(900, 220, 500);
	lhouselight.light->setCastShadows(false);

	lights.push_back(lhouselight);
	//******************************

	Mlight cameraL;

	cameraL.light = sceneManager->createLight();

	cameraL.light->setDiffuseColour(1.0f, 0.01f, 0.01f); //Warm
	cameraL.light->setSpecularColour(1.0f, 0.01f, 0.02f);
	cameraL.light->setPowerScale(30.0);
	cameraL.light->setType(Ogre::Light::LT_SPOTLIGHT);
	cameraL.position = Ogre::Vector3(0, -5.0f, -1);
	//exhibitL1.light->setDirection(Ogre::Vector3(1, -1, -1).normalisedCopy());
	cameraL.light->setAttenuationBasedOnRadius(5.0f, 0.01f);

	cameraL.light->setSpotlightInnerAngle(Ogre::Radian(0));
	cameraL.light->setCastShadows(true);
	cameraL.direction = Ogre::Vector3(0, 0, -1).normalisedCopy();

	int exid = -1;
	for (int i = 0; i < exhibits.size(); i++) {
		if (exhibits.at(i).exID == 1) {
			exid = i;
			break;
		}
	}

	cameraL.swing = &exhibits.at(exid);

	lights.push_back(cameraL);

	//******************************




}

Ogre::DSDatablock* Museum::getDatablock(std::vector<Ogre::String> params,
		Ogre::String name, exhibit exhibit) {
	Ogre::HlmsParamVec pv = Ogre::HlmsParamVec();
	Ogre::DSDatablock * dbl = NULL;
	dbl = checkDouble(params);
	if (dbl) {
		return dbl;
	}

	for (int ii = 0; ii < params.size(); ii++) {
		Ogre::String str = params.at(ii);
		Ogre::StringVector strv = Ogre::StringUtil::split(str, "$");

		if (strv.at(0).compare("name") == 0) {

			Ogre::DSDatablock* wat =
					static_cast<Ogre::DSDatablock*>(hlmsDS->getDatablock(
							Ogre::IdString(strv.at(1))));

			return static_cast<Ogre::DSDatablock*>(hlmsDS->getDatablock(
					Ogre::IdString(strv.at(1))));
		}

		pv.push_back(std::make_pair(strv.at(0), strv.at(1)));

	}
	std::sort(pv.begin(), pv.end());
	Ogre::HlmsBlendblock bb;

	Ogre::HlmsMacroblock * mb = new Ogre::HlmsMacroblock();
	mb->mCullMode = cm;

	Ogre::String SBTs[] = { "add", "modulate", "replace" };

	for (int i = 0; i < 3; i++) {
		if (SBTs[i].compare(exhibit.blendType) == 0) {
			bb.setBlendType((Ogre::SceneBlendType) i);
		}
	}

	Ogre::String CMPFs[] = { "afail", "apass", "less", "lequal", "equal",
			"nequal", "gequal", "greater" };
	for (int i = 0; i < 8; i++) {
		if (CMPFs[i].compare(exhibit.compareFunc) == 0) {
			mb->mDepthFunc = ((Ogre::CompareFunction) i);
		}
	}

	Ogre::String material = Ogre::StringConverter::toString(rand())
			+ Ogre::StringConverter::toString(rand()) + "exhibit_" + name;
	Ogre::DSDatablock * retVal =
			static_cast<Ogre::DSDatablock*>(hlmsDS->createDatablock(material,
					material, *mb, bb, pv));
	registerDouble(retVal, params);

	return retVal;
}

float Museum::randf(int threshold) {
	return (-2.0 * (rand() / (double) (RAND_MAX + 1))) - 1.0;

}

Ogre::DSDatablock* Museum::checkDouble(std::vector<Ogre::String> vec) {

	for (int i = 0; i < this->doubleCheck.size(); i++) {
		if (doubleCheck[i].str.size()!=vec.size()) {
			continue;
		}
		bool ContnueouterMainLoop=false;
		for (int ii = 0; ii < this->doubleCheck[i].str.size(); ii++) {
			if(doubleCheck[i].str[ii].compare(vec[ii])!=0){
				ContnueouterMainLoop=true;
			}
		}
		if(ContnueouterMainLoop){
			continue;
		}
		return doubleCheck[i].db;
	}

	return NULL;
}

void Museum::registerDouble(Ogre::DSDatablock* dbl,
		std::vector<Ogre::String> str) {

	doubleCheck.push_back(Double(str,dbl));
}

Museum::~Museum() {
	// TODO Auto-generated destructor stub
}

} /* namespace Demo */

