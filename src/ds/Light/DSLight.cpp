/*
 * DSLight.cpp
 *
 *  Created on: 24.11.2015
 *      Author: marvin
 */

#include "Light/DSLight.h"
#include "Light/DSLightManager.h"
#include "Datablocks/Data/DSMaterialParam.h"
namespace Ogre {

DSLight::DSLight(const Ogre::Light * parent, Ogre::IdType id,
		Ogre::ObjectMemoryManager *objectMemoryManager,
		Ogre::SceneManager *manager, Ogre::uint8 renderQueueId) :
		Ogre::MovableObject(id, objectMemoryManager, manager, renderQueueId), Renderable() {
	this->parent = parent;
	this->setAmbient(parent == NULL);
	//Set the bounds!!! Very important! If you don't set it, the object will not
	//appear on screen as it will always fail the frustum culling.
	//This example uses an infinite aabb; but you really want to use an Aabb as tight
	//as possible for maximum efficiency (so Ogre avoids rendering an object that
	//is off-screen)
	//Note the WorldAabb and the WorldRadius will be automatically updated by Ogre
	//every frame as rendering begins (it's calculated based on the local version
	//combined with the scene node's transform).
	if (parent != NULL && parent->getType() != Ogre::Light::LT_DIRECTIONAL) {
		Aabb aabbw = Aabb(parent->getParentNode()->_getDerivedPosition(),
				Vector3(parent->getAttenuationRange(),
						parent->getAttenuationRange(),
						parent->getAttenuationRange()));

		float radius = GeomUtils::getRadius(parent->getAttenuationConstant(),
				parent->getAttenuationLinear(), parent->getAttenuationQuadric(),
				parent->getPowerScale());

		Aabb aabb = Aabb(Vector3(Vector3(0, 0, 0)),
				Vector3(radius, radius, radius));

		aabbw = Aabb(Aabb::BOX_INFINITE);

		mObjectData.mLocalAabb->setFromAabb(aabb, mObjectData.mIndex);
		mObjectData.mWorldAabb->setFromAabb(aabb, mObjectData.mIndex);
		mObjectData.mLocalRadius[mObjectData.mIndex] =
				std::numeric_limits<Real>::max();
		mObjectData.mWorldRadius[mObjectData.mIndex] =
				std::numeric_limits<Real>::max();
//        Aabb aabb( Aabb::BOX_INFINITE );
//        mObjectData.mLocalAabb->setFromAabb( aabb, mObjectData.mIndex );
//        mObjectData.mWorldAabb->setFromAabb( aabb, mObjectData.mIndex );
//        mObjectData.mLocalRadius[mObjectData.mIndex] = std::numeric_limits<Real>::max();
//        mObjectData.mWorldRadius[mObjectData.mIndex] = std::numeric_limits<Real>::max();

	} else {
		Aabb aabb(Aabb::BOX_INFINITE);
		mObjectData.mLocalAabb->setFromAabb(aabb, mObjectData.mIndex);

		mObjectData.mWorldAabb->setFromAabb(aabb, mObjectData.mIndex);
		mObjectData.mLocalRadius[mObjectData.mIndex] =
				std::numeric_limits<Real>::max();
		mObjectData.mWorldRadius[mObjectData.mIndex] =
				std::numeric_limits<Real>::max();

	}

	Ogre::String name="Ambient";
	if(parent!=NULL){
		name=parent->getName()+"_"+Ogre::StringConverter::toString(parent->getId());
	}
	this->setName("Light_"+name);
	createBuffers();
	//This is very important!!! A MovableObject must tell what Renderables to render
	//through this array. Since we derive from both MovableObject & Renderable, add
	//ourselves to the array. Otherwise, nothing will be rendered.
	//Tip: You can use this array as a rough way to show or hide Renderables
	//that belong to this MovableObject.
	mRenderables.push_back(this);

	//If we don't set a datablock, we'll crash Ogre.
//        this->setDatablock( Root::getSingleton().getHlmsManager()->OgreDSLightGeom
//                            getHlms( HLMS_PBS )->getDefaultDatablock() );

}
//-----------------------------------------------------------------------------------
DSLight::~DSLight() {
	VaoManager *vaoManager =
			mManager->getDestinationRenderSystem()->getVaoManager();

	VertexArrayObjectArray::const_iterator itor = mVaoPerLod[0].begin();
	VertexArrayObjectArray::const_iterator end = mVaoPerLod[0].end();
	while (itor != end) {
		VertexArrayObject *vao = *itor;

		const VertexBufferPackedVec &vertexBuffers = vao->getVertexBuffers();
		VertexBufferPackedVec::const_iterator itBuffers = vertexBuffers.begin();
		VertexBufferPackedVec::const_iterator enBuffers = vertexBuffers.end();

		while (itBuffers != enBuffers) {
			vaoManager->destroyVertexBuffer(*itBuffers);
			++itBuffers;
		}

		if (vao->getIndexBuffer())
			vaoManager->destroyIndexBuffer(vao->getIndexBuffer());
		vaoManager->destroyVertexArrayObject(vao);

		++itor;
	}
}
//-----------------------------------------------------------------------------------
void DSLight::createBuffers(void) {
	//Create the indices.
//    const Ogre::uint16 c_indexData[3 * 2 * 6] =
//    {
//        0, 1, 2, 2, 3, 0, //Front face
//        6, 5, 4, 4, 7, 6, //Back face
//
//        3, 2, 6, 6, 7, 3, //Top face
//        5, 1, 0, 0, 4, 5, //Bottom face
//
//        4, 0, 3, 3, 7, 4, //Left face
//        6, 2, 1, 1, 5, 6, //Right face
//    };
	std::vector<Ogre::uint16> indices;
	std::vector<VerticeData> vertices;

	//get Geometry Size

	if (this->ambient) {
		GeomUtils::createQuad(vertices, indices);
	} else if (this->parent->getType() == 0) {
		GeomUtils::createQuad(vertices, indices);
	} else if (this->parent->getType() == 1) {

		float radius = GeomUtils::getRadius(parent->getAttenuationConstant(),
				parent->getAttenuationLinear(), parent->getAttenuationQuadric(),
				parent->getPowerScale());

		GeomUtils::createSphere(vertices, indices, radius, 20, 20);
	} else if (this->parent->getType() == 2) {
		float height = GeomUtils::getRadius(parent->getAttenuationConstant(),
				parent->getAttenuationLinear(), parent->getAttenuationQuadric(),
				parent->getPowerScale());

		Radian coneRadiusAngle = parent->getSpotlightOuterAngle() / 2;
		Real rad = Math::Tan(coneRadiusAngle) * height;
		GeomUtils::createCone(vertices, indices, rad, height, 40);
	}
//    for(int i=0;i<(3 * 2 * 6);i++){
//    	indices.push_back(c_indexData[i]);
//    }

//**************************************************************************************************************
	//todo this is probably not good code
	Ogre::uint16 * indicesArray = new Ogre::uint16[indices.size()];
	for (uint i = 0; i < indices.size(); i++) {
		indicesArray[i] = indices.at(i);

	}
	//seams more modern to hold the passbufferdata in a buffer instead of directly sendin

	VerticeData * verticesArray = new VerticeData[vertices.size()];
	for (uint i = 0; i < vertices.size(); i++) {
		verticesArray[i] = vertices.at(i);

	}

	Ogre::uint16 *indiceData = reinterpret_cast<Ogre::uint16*>(OGRE_MALLOC_SIMD(
			sizeof(Ogre::uint16) * indices.size(), Ogre::MEMCATEGORY_GEOMETRY));
	memcpy(indiceData, indicesArray, sizeof(Ogre::uint16) * indices.size());

	VaoManager *vaoManager =
			mManager->getDestinationRenderSystem()->getVaoManager();
	Ogre::IndexBufferPacked *indexBuffer = 0;

	try {
		indexBuffer = vaoManager->createIndexBuffer(
				Ogre::IndexBufferPacked::IT_16BIT, indices.size(),
				Ogre::BT_IMMUTABLE, indiceData, true);
	} catch (Ogre::Exception &e) {
		// When keepAsShadow = true, the memory will be freed when the index buffer is destroyed.
		// However if for some weird reason there is an exception raised, the memory will
		// not be freed, so it is up to us to do so.
		// The reasons for exceptions are very rare. But we're doing this for correctness.
		OGRE_FREE_SIMD(indexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
		indexBuffer = 0;
		throw e;
	}

	//Create the vertex buffer

	//Vertex declaration
	VertexElement2Vec vertexElements;
	vertexElements.push_back(VertexElement2(VET_FLOAT3, VES_POSITION));
	vertexElements.push_back(VertexElement2(VET_FLOAT3, VES_NORMAL));

	//For immutable buffers, it is mandatory that cubeVertices is not a null pointer.
	VerticeData *verticeData = reinterpret_cast<VerticeData*>(OGRE_MALLOC_SIMD(
			sizeof(VerticeData) * vertices.size(), Ogre::MEMCATEGORY_GEOMETRY));
	//Fill the data.
	memcpy(verticeData, verticesArray, sizeof(VerticeData) * vertices.size());

	Ogre::VertexBufferPacked *vertexBuffer = 0;
	try {
		//Create the actual vertex buffer.
		vertexBuffer = vaoManager->createVertexBuffer(vertexElements,
				vertices.size(), BT_IMMUTABLE, verticeData, true);
	} catch (Ogre::Exception &e) {
		OGRE_FREE_SIMD(vertexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
		vertexBuffer = 0;
		//seams more modern to hold the passbufferdata in a buffer instead of directly sendin
		throw e;
	}

	//Now the Vao. We'll just use one vertex buffer source (multi-source not working yet)
	VertexBufferPackedVec vertexBuffers;
	vertexBuffers.push_back(vertexBuffer);

	Ogre::VertexArrayObject *vao = vaoManager->createVertexArrayObject(
			vertexBuffers, indexBuffer, OT_TRIANGLE_LIST);

	mVaoPerLod[0].push_back(vao);
	//Use the same geometry for shadow casting. You can optimize performance by creating
	//a different Vao that only uses VES_POSITION, VES_BLEND_INDICES & VES_BLEND_WEIGHTS
	//(packed together to fit the caches) and avoids duplicated vertices (usually
	//needed by normals, UVs, etc)
	mVaoPerLod[1].push_back(vao);
}
//-----------------------------------------------------------------------------------
const String& DSLight::getMovableType(void) const {
	String *retval = new String("DLight");

	return *retval;
}
//-----------------------------------------------------------------------------------
const LightList& DSLight::getLights(void) const {
	return this->queryLights(); //Return the data from our MovableObject base class.
}
//-----------------------------------------------------------------------------------
void DSLight::getRenderOperation(v1::RenderOperation& op, bool casterPass) {
	OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
			"MyCustomRenderable do not implement getRenderOperation."
					" You've put a v2 object in "
					"the wrong RenderQueue ID (which is set to be compatible with "
					"v1::Entity). Do not mix v2 and v1 objects",
			"MyCustomRenderable::getRenderOperation");
}
//-----------------------------------------------------------------------------------
void DSLight::getWorldTransforms(Matrix4* xform) const {
	OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
			"MyCustomRenderable do not implement getWorldTransforms."
					" You've put a v2 object in "
					"the wrong RenderQueue ID (which is set to be compatible with "
					"v1::Entity). Do not mix v2 and v1 objects",
			"MyCustomRenderable::getRenderOperation");
}
//-----------------------------------------------------------------------------------
bool DSLight::getCastsShadows(void) const {
	OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
			"MyCustomRenderable do not implement getCastsShadows."
					" You've put a v2 object in "
					"the wrong RenderQueue ID (which is set to be compatible with "
					"v1::Entity). Do not mix v2 and v1 objects",
			"MyCustomRenderable::getRenderOperation");
	return true;
}

void DSLight::setAmbient(bool bool1) {
	this->ambient = bool1;
}

bool DSLight::isDead() {
	if (ambient) {
		return false;
	}
	if (parent == NULL) {
		return true;
	}

	return false;
}
int DSLight::mapFullData(GPUPointer* gp) {
	int presc=40;
	int useup=presc;



	DSDatablock* db= dynamic_cast<DSDatablock*>(this->getDatablock());
	Ogre::String params[]={
	"position",
	"diffuse",
	"specular",
	"attenuation",
	"spotdirection",
	"spotparams",
	"lightparams",
	"lightsettings",
	"shadowParams",
	"shadowQualityParams",
	"shadowRes",//array
	"shadowMat",//array
	"shadowDr",//array
	"shadowPssmSplits",//array
	};

	union un{
		float f;
		uint i;
	};
	un p;

	int rest=64;
	float * start=gp->getPassBufferPtr();
	uint paramiter=-1;
	for(int i=0;i<64;i++){
		paramiter++;

		Vector4 retval=Vector4(0,0,0,0);

		if(paramiter<14&&db->hasparam(params[paramiter])){
			retval.x=db->getParam(params[paramiter])->x;
			retval.y=db->getParam(params[paramiter])->y;
			retval.z=db->getParam(params[paramiter])->z;
			retval.w=db->getParam(params[paramiter])->w;
		}else{gp->map(Ogre::Vector4());continue; }
		//if(params[i].compare("lightparams")==0&&parent!=NULL&&parent->getType()==Ogre::Light::LT_SPOTLIGHT&&Ogre::StringUtil::startsWith(parent->getName(),"DynamicLight_") ){
		if(parent!=NULL&&i==0){
			p.f=retval.x;
		}
		if(i>=10){
			uint size=0;
			float * retvalf=db->getParam(params[paramiter],size);
			assert(size>0);
			gp->map((int)(size/4));
			gp->map(1234.0f);
			gp->map(1.0f);
			gp->map(1.0f);

			for(uint ii=0;ii<size;ii++){
				gp->map(retvalf[ii]);
				if(ii%4==0)
					i++;
				assert(i<64);
			}
			assert(size%4==0);

		}
		else{

			gp->map(retval);
		}

	}
//	for(int i=0;i<54;i++){
//		Vector4 retval=Vector4(0,0,0,0);
//		gp->map(retval);
//		//gp->map(DSMath::RainbowPhase(i/(54.0f)));
//	}
	//assert(gp->getSize()%64==1);

	return presc;
}

} /* namespace Ogre */

