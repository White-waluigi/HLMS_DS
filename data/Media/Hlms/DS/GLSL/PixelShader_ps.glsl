#version 400 core
#extension GL_ARB_shading_language_420pack: require

//Uniforms that change per pass
layout (binding = 0) uniform PassBuffer
{
	@insertpiece( PassInternalDecl )
} passBuf;
out vec4 final;
in block
{
@insertpiece( VStoPS_block )
} inPs;


void main() {
	final=vec4(0.004, 1.0, 0.671,0);
	final.xyz=	inPs.normal;


}
