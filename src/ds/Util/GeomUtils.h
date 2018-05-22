/*
 * GeomUtils.h
 *
 *  Created on: Jan 22, 2018
 *      Author: marvin
 *
 *  Generate a variety of different shapes as vertex objects
 */

#ifndef _GEOMUTILS_H
#define _GEOMUTILS_H
//For generating the light meshes
#include <OgreString.h>
#include <OgreVertexIndexData.h>
 struct VerticeData
    {
        float px, py, pz;   //Position
        float nx, ny, nz;   //Normals

        VerticeData() {
        	px=0;
        	py=0;
        	pz=0;
        	nx=0;
        	ny=1;
        	nz=0;
        }
        VerticeData( float _px, float _py, float _pz,
                      float _nx, float _ny, float _nz ) :
            px( _px ), py( _py ), pz( _pz ),
            nx( _nx ), ny( _ny ), nz( _nz )
        {
        }
        VerticeData( float _px, float _py, float _pz) :
            px( _px ), py( _py ), pz( _pz )
        {
    		nx=0;
    		ny=1;
    		nz=0;
        }
    };
class GeomUtils
{
public:

	    // Create a sphere Mesh with a given name, radius, number of rings and number of segments
	    static void createSphere(const Ogre::String& strName
	        , float radius
	        , int nRings, int nSegments
	        , bool bNormals
	        , bool bTexCoords
	        );

    // Fill up a fresh copy of VertexData and IndexData with a sphere's coords given the number of rings and the number of segments
    static void createSphere(Ogre::v1::VertexData*& vertexData, Ogre::v1::IndexData*& indexData
        , float radius
        , int nRings, int nSegments
        , bool bNormals
        , bool bTexCoords);

    // Create a cone Mesh with a given name, radius and number of vertices in base
    // Created cone will have its head at 0,0,0, and will 'expand to' positive y
    static void createCone(const Ogre::String& strName
        , float radius
        , float height
        , int nVerticesInBase);

    // Fill up a fresh copy of VertexData and IndexData with a cone's coords given the radius and number of vertices in base
    static void createCone(Ogre::v1::VertexData*& vertexData, Ogre::v1::IndexData*& indexData
        , float radius
        , float height
        , int nVerticesInBase);


    // Fill up a fresh copy of VertexData with a normalized quad
    static void createQuad(Ogre::v1::VertexData*& vertexData);
    //create a round pyramid
    static  void createCone(std::vector<VerticeData>& vertex,
			std::vector<Ogre::uint16> &indices, float radius, float height,
			int nVerticesInBase);
  // Create a sphere Mesh with a given name, radius, number of rings and number of segments

  static  void createSphere(std::vector<VerticeData>& vertex,
			std::vector<Ogre::uint16>& indices,
			 float radius
	        , int nRings, int nSegments
	       );
  //Create a simple plane facing forward
  static  void createQuad(std::vector<VerticeData>& vertex,
			std::vector<Ogre::uint16> &indices);
  //Generate range for a given attenuation at which light is no longer visible (So lightmeshes aren't too large)
  static float getRadius(float a, float b, float c,float radius);
};


#endif
