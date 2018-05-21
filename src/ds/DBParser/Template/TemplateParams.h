#ifndef SRC_DBPARSER_TEMPLATE_MATERIALPARSER_H_
#define SRC_DBPARSER_TEMPLATE_MATERIALPARSER_H_

#include <Ogre.h>
#include <OgreIdString.h>

class MT_WrongDTException : std::exception{

	virtual const char* what() const throw()
	{
		return "wrong datatype";
	}
};
   
class MT_Meta {
	enum type {
		Template, Material, Library
	} type = Template;

	Ogre::String TemplateMaterial="";
	bool inheritDefault = true;

};
class MT_Settings {
	Ogre::String category;
	enum renderer {
		Deferred, Forward
	} renderer = Deferred;

	float shadowConstBias = 0.01;
	bool optimizable = true;

};

class MT_Param {
	Ogre::String name;
};


//
//class MT_Piece: public MT_Param {
//public:
//	Ogre::String id;
//
//	bool optional = false;
//	MT_Shader  shader;
//	enum Method {
//		ADD, REPLACE, FIRST, NOREPLACE
//	} method = ADD;
//};

class MT_Mappingblock: public MT_Param {
public:
	std::map<Ogre::String, std::vector<float> > animations;
};

class MT_AutoParam: public MT_Param {
	Ogre::String type;
	std::vector<float> params;

};

class MT_TexData: public MT_Param {
	Ogre::String file;
	Ogre::String sampler;
	Ogre::String MT_Mappingblock;
};

class MT_VarData: public MT_Param {
	float * val;
};




class MT_Paired: public MT_Param {
	Ogre::String datatype = "vec4";
	std::vector<float> value;
	MT_TexData texData;
};

class MT_Texture: public MT_Param {
	Ogre::String datatype = "vec4";
	MT_TexData texData;
};

class MT_Simple: public MT_Param {
	Ogre::String datatype = "vec4";
	std::vector<float> value;
};
class MT_Property: public MT_Param {
public:
	Ogre::String id;
	Ogre::int32 value = 0;
};

class MT_Shader: public MT_Param {
public:
	std::map<Ogre::String, Ogre::String> code;
};


#endif
