#version 400 core
#extension GL_ARB_shading_language_420pack: require



//Uniforms that change per pass
layout (binding = 0) uniform PassBuffer
{
	@insertpiece( PassInternalDecl )
} passBuf;
out vec4 final;
@property(NUM_TEXTURES)
uniform sampler2DArray textureMapsArray[@value(NUM_TEXTURES)];
@end

in block
{
@insertpiece( VStoPS_block )
} inPs;
@insertpiece( MaterialDecl )
@insertpiece( InstanceDecl )
@insertpiece( testpiece )
@insertpiece(TEXTUREFUNCTION)

float rand(uint s){

	vec2 co=vec2(float(s),float(s*2));
    return fract(sin(dot(co,vec2(12.9898,78.233))) * 43758.5453);
}

vec4 randv(uint s){

	return vec4(rand(s),rand(s*9),rand(s*3),rand(s*s));
}
void main() {


	vec2 screenPos=vec2((gl_FragCoord.x/1920.0),(gl_FragCoord.y/1080.0));

	final=vec4(.4,.4,.7,0);
	
@property(TEXTURE_testtex)
	//final=	texture(textureMapsArray[0],vec3(inPs.uv0.xy,0 ))*inPs.normal.xyzz;
	vec4 op=getDSTexture(@value(TEXTURE_testtex),inPs.uv0.xy*vec2(0.5,1.0)+vec2(0.5,0.0));
	if(op.x>0.03){
		discard;
	}

	final=getDSTexture(@value(TEXTURE_testtex),inPs.uv0.xy*vec2(0.5,1.0));
	final*=getDSTexture(@value(TEXTURE_testtex2),inPs.uv0.xy*vec2(0.5,1.0));
@end
	
}
