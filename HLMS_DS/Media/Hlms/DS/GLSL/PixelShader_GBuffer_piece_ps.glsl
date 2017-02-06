
@piece(GBuffer_PS)


#version 400 core
#extension GL_ARB_shading_language_420pack: require
#extension GL_EXT_texture_array : enable

@insertpiece(Helper)


@property(hlms_shadowcaster)

@end

@insertpiece(STRUCT)
@insertpiece(RAINBOW)

@insertpiece(MaterialDecl)

@insertpiece( PassDecl )






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
@insertpiece(param_helper_custom)

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
	
	

	
		
	normal.w=vec4((length(inPs.pos.xyz) / pass.farClip)).a;
	//Ogre Shadows want different depth than DS lighting
	//Linear depth
	pos.x= (inPs.glPosition.z ) ;


	@insertpiece(custom_GBuffer_No_Shadow)

 	
@end
@insertpiece(custom_post_material)



	@insertpiece(custom_GBuffer)




	@property(!manual-opacity)
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
	@end
	@property( manual-opacity)
		@insertpiece(opacity_calc)
	@end
	
	@property(hlms_shadowcaster)
		
		
		float scb=0;
	
		@property(vec4_shadow_const_bias)
			scb=material.vec4_shadow_const_bias.x;
		@end
		depth.x	=((inPs.glPosition.z+scb)/ pass.farClip);


		
		@insertpiece(custom_post_shadow)
	@end
	@property(!hlms_shadowcaster)
		@insertpiece(custom_post_no_shadow)	
	@end
	
	@insertpiece(custom_post)
	
	diffuse.a=0.9;
@property( !hlms_shadowcaster)
	
	if(floatBitsToUint(pass.debug.x)==9u){
 		glow=material.idColor;	
 	}
@end
@property(rainbow)
		glow=rainbow(inPs.uv0.x*screenPos.x);
@end
@property(cdepth)
	gl_FragDepth =@value(cdepth)-1;
@end
 }
 
 @end
