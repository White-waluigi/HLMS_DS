
@piece(GBuffer_PS)


#version 400 core
#extension GL_ARB_shading_language_420pack: require
#extension GL_EXT_texture_array : enable
@property(hlms_shadowcaster)

@end

@insertpiece(STRUCT)
@insertpiece(RAINBOW)

@insertpiece(MaterialDecl)

@insertpiece( PassDecl )




@property( diffuse_map )	uint diffuseIdx;@end
@property( normal_map )	uint normalIdx;@end
@property( specular_map )	uint specularIdx;@end
@property( roughness_map )	uint roughnessIdx;@end
@property( detail_weight_map )	uint weightMapIdx;@end


@property( num_textures )uniform sampler2DArray textureMaps[@value( num_textures )];@end
layout(binding = 0) uniform samplerBuffer worldMatBuf;

@property( tex_normal_map )
vec3 getTSNormal( vec3 uv )
{
	vec3 tsNormal;
	//Normal texture must be in U8V8 or BC5 format!
	tsNormal.xy = texture( textureMaps[@value(texv_normal_map)], uv ).xy;

	tsNormal.z	= sqrt( 1.0 - tsNormal.x * tsNormal.x - tsNormal.y * tsNormal.y );

	return tsNormal;
}
@end

@insertpiece(InstanceDecl)
//layout(binding = 4) uniform indexBuffer
//{
//	uvec4 colour; //kD.w is alpha_test_threshold
//	uvec4 viewProj0;
//	uvec4 viewProj1;
//	uvec4 viewProj2;
//	uvec4 viewProj3;
	
//} test;

//layout(binding = 2) uniform InstanceBuffer
//{
    //.x =
	//The lower 9 bits contain the material's start index.
    //The higher 23 bits contain the world matrix start index.
    //
    //.y =
    //shadowConstantBias. Send the bias directly to avoid an
    //unnecessary indirection during the shadow mapping pass.
    //Must be loaded with uintBitsToFloat
    //uvec4 worldMaterialIdx[4096];
//} instance;

in block
{
@insertpiece( VStoPS_block )
} inPs;
in vec4 vcolor;

@property( !hlms_shadowcaster)
out vec4 diffuse;
out vec4 normal;
out vec4 pos;
out vec4 specular;
out vec4 glow;
@end
@property(hlms_shadowcaster)
out vec4 depth;
@end

uint f2u(float f){
	return floatBitsToUint(f);
}
uint f2u(vec4 f){
	return floatBitsToUint(f)[0];
}
void main() {
	
	
			
@property(NumTextureDefines)
	mat4 material_texmat_0=mat4(1,0,0,0 ,0,1,0,0, 0,0,1,0 ,0,0,0,1);
	mat4 material_texmat_1=mat4(1,0,0,0 ,0,1,0,0, 0,0,1,0 ,0,0,0,1);
	mat4 material_texmat_2=mat4(1,0,0,0 ,0,1,0,0, 0,0,1,0 ,0,0,0,1);
	mat4 material_texmat_3=mat4(1,0,0,0 ,0,1,0,0, 0,0,1,0 ,0,0,0,1);
	
	
@end
	float opacity=1.0;

	vec2 screenPos=vec2((gl_FragCoord.x/pass.screenx),(gl_FragCoord.y/pass.screeny));

	vec2 texCoord=vec2(screenPos.x,screenPos.y);


@property(hlms_shadowcaster)

	vec4 diffuse=vec4(1,0.5,0,0);
	vec4 normal=vec4(1,0.5,0,0);
	vec4 specular=vec4(1,0,1,0);


	@insertpiece(diffuse_value)
	@insertpiece(opacity_value)


@end

@property( !hlms_shadowcaster)
	diffuse=vec4(0);
	normal=vec4(0);
	specular=vec4(0);
	glow=vec4(0.1);
	
	@insertpiece(custom_pre_material)
	@insertpiece(diffuse_value)
	@insertpiece(normal_value)
	@insertpiece(specular_value)
	@insertpiece(glow_value)
	@insertpiece(opacity_value)
	@insertpiece(reflection_value)
	
	
	@property(rainbow)
		diffuse=rainbow(inPs.uv0.x);
	@end
	
		
	normal.w=vec4((length(inPs.pos.xyz) / pass.farClip)).a;
	//Ogre Shadows want different depth than DS lighting
	//Linear depth
	pos.x= (inPs.glPosition.z ) ;


	
	if(floatBitsToUint(pass.debug.x)==9u){
 		glow=material.idColor;	
 	}
 	
@end
@insertpiece(custom_post_material)

@property(hlms_shadowcaster)
	/*@insertpiece(opacity_value)
	*/
	

@end

	@property(!tex_opacity_map)
		@property( vec4_opacity  )	
			@insertpiece(opacity_calc)
		@end
	@end
	@property( opacity-diffuse)
		@insertpiece(opacity_calc)
	@end
	@property( tex_opacity_map)
		@insertpiece(opacity_calc)
		
		
	@end

	@property(hlms_shadowcaster)
		
		
		//depth=texelFetch(worldMatBuf,int(inPs.drawId)+(int(gl_FragCoord.x)%16) );

		//depth=vec4((length(inPs.pos.xyz) / pass.farClip));
		
		//depth=vcolor;
		//depth=normalize(vcolor);
		//depth=inPs.pos;	
		depth.x	=inPs.glPosition.z/ pass.farClip;


		//depth=vec4(inPs.pos.xyzz);
	@end


 }
 
 @end
