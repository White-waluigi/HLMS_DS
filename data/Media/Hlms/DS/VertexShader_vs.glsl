#version 400 core
#extension GL_ARB_shading_language_420pack: require

@insertpiece( Common_Matrix_DeclUnpackMatrix4x4 )

//Uniforms that change per pass
layout_constbuffer(binding = 0) uniform PassBuffer
{
	@insertpiece( PassInternalDecl )
} passBuf;
in vec4 vertex;
in vec3 normal;
in vec3 uv0;
in uint drawId;


out block
{
@insertpiece( VStoPS_block )
} outVs;
uniform samplerBuffer worldMatBuf;



@insertpiece( MaterialDecl )

void main()
{

    
	mat4 worldViewProj = UNPACK_MAT4( worldMatBuf, drawId );
	
	vec4 ff=worldViewProj*vertex ;
    gl_Position =ff; 
	outVs.normal=normal;
	outVs.uv0=uv0;
	outVs.drawId=drawId;
}
    
