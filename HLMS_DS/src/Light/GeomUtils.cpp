/*
 -----------------------------------------------------------------------------
 This source file is part of OGRE
 (Object-oriented Graphics Rendering Engine)
 For the latest info, see http://www.ogre3d.org/

 Copyright (c) 2000-2014 Torus Knot Software Ltd
 Also see acknowledgements in Readme.html

 You may use this sample code for anything you like, it is not covered by the
 same license as the rest of the engine.
 -----------------------------------------------------------------------------
 */

#include "GeomUtils.h"

#include "OgreMeshManager.h"
#include "OgreSubMesh.h"
#include "OgreHardwareBufferManager.h"
#include "OgreMesh.h"

using namespace Ogre;

void GeomUtils::createSphere(const String& strName, float radius, int nRings,
		int nSegments, bool bNormals, bool bTexCoords) {
	v1::MeshPtr pSphere = v1::MeshManager::getSingleton().createManual(strName,
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	v1::SubMesh *pSphereVertex = pSphere->createSubMesh();
	pSphere->sharedVertexData[0] = new v1::VertexData();

	createSphere(pSphere->sharedVertexData[0], pSphereVertex->indexData[0], radius,
			nRings, nSegments, bNormals // need normals
			, bTexCoords // need texture co-ordinates
			);

	// Generate face list
	pSphereVertex->useSharedVertices = true;

	// the original code was missing this line:
	pSphere->_setBounds(
			AxisAlignedBox(Vector3(-radius, -radius, -radius),
					Vector3(radius, radius, radius)), false);
	pSphere->_setBoundingSphereRadius(radius);
	// this line makes clear the mesh is loaded (avoids memory leaks)
	pSphere->load();
}

void GeomUtils::createSphere(v1::VertexData*& vertexData,
		v1::IndexData*& indexData, float radius, int nRings, int nSegments,
		bool bNormals, bool bTexCoords) {
	assert(vertexData && indexData);

	// define the vertex format
	v1::VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	size_t currOffset = 0;
	// positions
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
	currOffset += v1::VertexElement::getTypeSize(VET_FLOAT3);

	if (bNormals) {
		// normals
		vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_NORMAL);
		currOffset += v1::VertexElement::getTypeSize(VET_FLOAT3);

	}
	// two dimensional texture coordinates
	if (bTexCoords) {
		vertexDecl->addElement(0, currOffset, VET_FLOAT2,
				VES_TEXTURE_COORDINATES, 0);
	}

	// allocate the vertex buffer
	vertexData->vertexCount = (nRings + 1) * (nSegments + 1);
	v1::HardwareVertexBufferSharedPtr vBuf =
			v1::HardwareBufferManager::getSingleton().createVertexBuffer(
					vertexDecl->getVertexSize(0), vertexData->vertexCount,
					v1::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	v1::VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	float* pVertex = static_cast<float*>(vBuf->lock(
			v1::HardwareBuffer::HBL_DISCARD));

	// allocate index buffer
	indexData->indexCount = 6 * nRings * (nSegments + 1);
	indexData->indexBuffer =
			v1::HardwareBufferManager::getSingleton().createIndexBuffer(
					v1::HardwareIndexBuffer::IT_16BIT, indexData->indexCount,
					v1::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	v1::HardwareIndexBufferSharedPtr iBuf = indexData->indexBuffer;
	unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(
			v1::HardwareBuffer::HBL_DISCARD));

	float fDeltaRingAngle = (Math::PI / nRings);
	float fDeltaSegAngle = (2 * Math::PI / nSegments);
	unsigned short wVerticeIndex = 0;

	// Generate the group of rings for the sphere
	for (int ring = 0; ring <= nRings; ring++) {
		float r0 = radius * sinf(ring * fDeltaRingAngle);
		float y0 = radius * cosf(ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for (int seg = 0; seg <= nSegments; seg++) {
			float x0 = r0 * sinf(seg * fDeltaSegAngle);
			float z0 = r0 * cosf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			*pVertex++ = x0;
			*pVertex++ = y0;
			*pVertex++ = z0;

			if (bNormals) {
				Vector3 vNormal = Vector3(x0, y0, z0).normalisedCopy();
				*pVertex++ = vNormal.x;
				*pVertex++ = vNormal.y;
				*pVertex++ = vNormal.z;
			}
			if (bTexCoords) {
				*pVertex++ = (float) seg / (float) nSegments;
				*pVertex++ = (float) ring / (float) nRings;
			}

			if (ring != nRings) {
				// each vertex (except the last) has six indices pointing to it
				*pIndices++ = wVerticeIndex + nSegments + 1;
				*pIndices++ = wVerticeIndex;
				*pIndices++ = wVerticeIndex + nSegments;
				*pIndices++ = wVerticeIndex + nSegments + 1;
				*pIndices++ = wVerticeIndex + 1;
				*pIndices++ = wVerticeIndex;
				wVerticeIndex++;
			}
		}; // end for seg
	} // end for ring

	// Unlock
	vBuf->unlock();
	iBuf->unlock();
}

void GeomUtils::createQuad(v1::VertexData*& vertexData) {
	assert(vertexData);

	vertexData->vertexCount = 4;
	vertexData->vertexStart = 0;

	v1::VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	v1::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

	vertexDecl->addElement(0, 0, VET_FLOAT3, VES_POSITION);

	v1::HardwareVertexBufferSharedPtr vbuf =
			v1::HardwareBufferManager::getSingleton().createVertexBuffer(
					vertexDecl->getVertexSize(0), vertexData->vertexCount,
					v1::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	// Bind buffer
	bind->setBinding(0, vbuf);
	// Upload data
	float data[] = { -1, 1, -1,  // corner 1
			-1, -1, -1, // corner 2
			1, 1, -1,   // corner 3
			1, -1, -1 }; // corner 4
	vbuf->writeData(0, sizeof(data), data, true);
}

void GeomUtils::createCone(const Ogre::String& strName, float radius,
		float height, int nVerticesInBase) {
	v1::MeshPtr pCone = v1::MeshManager::getSingleton().createManual(strName,
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	v1::SubMesh *pConeVertex = pCone->createSubMesh();
	pCone->sharedVertexData[0] = new v1::VertexData();

	createCone(pCone->sharedVertexData[0], pConeVertex->indexData[0], radius, height,
			nVerticesInBase);

	// Generate face list
	pConeVertex->useSharedVertices = true;

	// the original code was missing this line:
	pCone->_setBounds(
			AxisAlignedBox(Vector3(-radius, 0, -radius),
					Vector3(radius, height, radius)), false);

	pCone->_setBoundingSphereRadius(
			Math::Sqrt(height * height + radius * radius));
	// this line makes clear the mesh is loaded (avoids memory leaks)
	pCone->load();
}

void GeomUtils::createCone(Ogre::v1::VertexData*& vertexData,
		Ogre::v1::IndexData*& indexData, float radius, float height,
		int nVerticesInBase) {
	assert(vertexData && indexData);

	// define the vertex format
	v1::VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	// positions
	vertexDecl->addElement(0, 0, VET_FLOAT3, VES_POSITION);

	// allocate the vertex buffer
	vertexData->vertexCount = nVerticesInBase + 1;
	v1::HardwareVertexBufferSharedPtr vBuf =
			v1::HardwareBufferManager::getSingleton().createVertexBuffer(
					vertexDecl->getVertexSize(0), vertexData->vertexCount,
					v1::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	v1::VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	float* pVertex = static_cast<float*>(vBuf->lock(
			v1::HardwareBuffer::HBL_DISCARD));

	// allocate index buffer - cone and base
	indexData->indexCount = (3 * nVerticesInBase) + (3 * (nVerticesInBase - 2));
	indexData->indexBuffer =
			v1::HardwareBufferManager::getSingleton().createIndexBuffer(
					v1::HardwareIndexBuffer::IT_16BIT, indexData->indexCount,
					v1::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	v1::HardwareIndexBufferSharedPtr iBuf = indexData->indexBuffer;
	unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(
			v1::HardwareBuffer::HBL_DISCARD));

	//Positions : cone head and base
	for (int i = 0; i < 3; i++)
		*pVertex++ = 0.0f;

	//Base :
	float fDeltaBaseAngle = (2 * Math::PI) / nVerticesInBase;
	for (int i = 0; i < nVerticesInBase; i++) {
		float angle = i * fDeltaBaseAngle;
		*pVertex++ = radius * cosf(angle);
		*pVertex++ = height;
		*pVertex++ = radius * sinf(angle);
	}

	//Indices :
	//Cone head to vertices
	for (int i = 0; i < nVerticesInBase; i++) {
		*pIndices++ = 0;
		*pIndices++ = (i % nVerticesInBase) + 1;
		*pIndices++ = ((i + 1) % nVerticesInBase) + 1;
	}
	//Cone base
	for (int i = 0; i < nVerticesInBase - 2; i++) {
		*pIndices++ = 1;
		*pIndices++ = i + 3;
		*pIndices++ = i + 2;
	}

	// Unlock
	vBuf->unlock();
	iBuf->unlock();
}
void GeomUtils::createCone(std::vector<VerticeData>& vertex,
		std::vector<Ogre::uint16> &indices, Ogre::Real radius, Ogre::Real height,
		int nVerticesInBase) {

	vertex.clear();
	indices.clear();
//	for (int i = 0; i < 3; i++)
//		vertex.push_back(0.0f);

	vertex.push_back(VerticeData(0,0,0,0,1,0));

	//Base :
	float fDeltaBaseAngle = (2 * Math::PI) / nVerticesInBase;
	for (int i = 0; i < nVerticesInBase; i++) {
		float angle = i * fDeltaBaseAngle;
//		vertex.push_back(radius * cosf(angle));
//		vertex.push_back(height);
//		vertex.push_back(radius * sinf(angle));
		vertex.push_back(VerticeData(radius * cosf(angle),height,radius * sinf(angle),0,1,0));
	}

	//Indices :
	//Cone head to vertices
	for (int i = 0; i < nVerticesInBase; i++) {
		indices.push_back(0);
		indices.push_back((i % nVerticesInBase) + 1);
		indices.push_back(((i + 1) % nVerticesInBase) + 1);
	}
	//Cone base
	for (int i = 0; i < nVerticesInBase - 2; i++) {
		indices.push_back(1);
		indices.push_back(i + 3);
		indices.push_back(i + 2);
	}

}

 void GeomUtils::createSphere(std::vector<VerticeData>& vertex,
		std::vector<Ogre::uint16>& indices,
		 float radius
        , int nRings, int nSegments) {

	float fDeltaRingAngle = (Math::PI / nRings);
	float fDeltaSegAngle = (2 * Math::PI / nSegments);
	unsigned short wVerticeIndex = 0;

	// Generate the group of rings for the sphere
	for (int ring = 0; ring <= nRings; ring++) {
		float r0 = radius * sinf(ring * fDeltaRingAngle);
		float y0 = radius * cosf(ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for (int seg = 0; seg <= nSegments; seg++) {
			float x0 = r0 * sinf(seg * fDeltaSegAngle);
			float z0 = r0 * cosf(seg * fDeltaSegAngle);

			vertex.push_back(VerticeData(x0,y0, z0));
			// Add one vertex to the strip which makes up the sphere







			if (ring != nRings) {
				// each vertex (except the last) has six indices pointing to it
				indices.push_back( wVerticeIndex + nSegments + 1);
				indices.push_back(  wVerticeIndex);
				indices.push_back(  wVerticeIndex + nSegments);

				indices.push_back(  wVerticeIndex + nSegments + 1);
				indices.push_back( wVerticeIndex + 1);
				indices.push_back( wVerticeIndex);


				wVerticeIndex++;
			}

		}; // end for seg
	} // end for ring
//	std::cout <<indices.size()<<"\n";
//	std::cout <<vertex.size()<<"\n";
//	abort();

	// Unlock


}

void GeomUtils::createQuad(std::vector<VerticeData>& vertex,
		std::vector<Ogre::uint16>& indices) {

	// Upload data
	float data[] = { -1, 1, -1,  // corner 1
			-1, -1, -1, // corner 2
			1, 1, -1,   // corner 3
			1, -1, -1 }; // corner 4

		vertex.push_back(VerticeData(-1, 1, -1,0, 0, 0));
		vertex.push_back(VerticeData(-1, -1, -1,0, 0, 0));
		vertex.push_back(VerticeData(1, 1, -1,0, 0, 0));
		vertex.push_back(VerticeData(1, -1, -1,0, 0, 0));

		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(1);

		indices.push_back(1);
		indices.push_back(3);
		indices.push_back(2);



}

float GeomUtils::getRadius(float a, float b, float c,float range) {

	    // Set Attenuation parameter to shader
	    //setCustomParameter(3, Vector4(c, b, a, 0));
	    float outerRadius;
	    /// There is attenuation? Set material accordingly


	            //// Calculate radius from Attenuation

	    		// difference of 10-15 levels deemed unnoticeable
	    		// lol, bullshit, its waaaaaaaaaaaaaay too low
	            float threshold_level = 0.1;
	            float threshold = 1.0f/(threshold_level/256.0f);

	            //// Use quadratic formula to determine outer radius
	            c = c-threshold;
	            float d=sqrt(b*b-4*a*c);
	            outerRadius = (-2*c)/(b+d);
	            outerRadius *= 1.2*range;



	    return (outerRadius);

}
