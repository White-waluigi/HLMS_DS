/*
 * DSShaderGenerator.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: marvin
 */

#include "DSShaderGenerator.h"

namespace Ogre {

DSShaderGenerator::DSShaderGenerator() {
	// TODO Auto-generated constructor stub

}

DSShaderGenerator::ShaderCode DSShaderGenerator::generateTextureHelperCode(Ogre::String varname,Ogre::String texname,Ogre::String datatype,Ogre::String modulator) {

	Ogre::String code=		datatype+" "+varname+" "+modulator+"=  texture( textureMaps[@value(texv_$%texname%)], vec3( \n"
							"(vec4(inPs.uv@value(uvi_$%texname%).xy,0,1)*material.texmat_@value(texd_$%texname%)).xy, \n"
							"f2u( material.texloc_@value(texd_$%texname%) ) ) ); \n";

	code=Ogre::StringUtil::replaceAll(code,"$%texname%",texname);

	return DSShaderGenerator::ShaderCode::getDefault(code);
}

DSShaderGenerator::ShaderCode DSShaderGenerator::generateVarHelperCode(Ogre::String varname, Ogre::String datatype) {
	assert(varname.compare("")!=0);
	assert(datatype.compare("")!=0);

	return DSShaderGenerator::ShaderCode::getDefault(datatype+" "+varname+"="+ "material.vec4_"+varname+";");
}

DSShaderGenerator::ShaderCode DSShaderGenerator::generatePairedHelperCode(Ogre::String varname,Ogre::String texname,Ogre::String datatype,Ogre::String modulator="*" ) {
	DSShaderGenerator::ShaderCode sc =generateVarHelperCode(varname,datatype);
	return ShaderCode::merge( sc,generateTextureHelperCode(varname,texname,"", modulator));

}

DSShaderGenerator::ShaderCode DSShaderGenerator::ShaderCode::getDefault(Ogre::String allocator) {
	DSShaderGenerator::ShaderCode sc;
	sc.code["glsl"]=allocator;
	sc.code["hlsl"]=allocator;
	return sc;
}

DSShaderGenerator::~DSShaderGenerator() {
	// TODO Auto-generated destructor stub
}

DSShaderGenerator::ShaderCode DSShaderGenerator::ShaderCode::merge(DSShaderGenerator::ShaderCode s1,DSShaderGenerator::ShaderCode s2) {

		ShaderCode retVal;
	  for (std::map<Ogre::String,Ogre::String>::iterator it=s1.code.begin(); it!=s1.code.end(); ++it){
		  const Ogre::String vx=it->first;
		  if(s2.code.find(vx)!=s2.code.end()){
			  retVal.code[it->first]=it->second+"\n\n"+s2.code[it->first];
		  }
	  }

	  return retVal;
}

} /* namespace Ogre */
