
@piece(Forward_PS)


#version 400 core
#extension GL_ARB_shading_language_420pack: require
#extension GL_EXT_texture_array : enable
layout(std140) uniform;
@insertpiece(Helper)


@property(hlms_shadowcaster)

@end

@insertpiece(STRUCT)
@insertpiece(RAINBOW)



/*@insertpiece(LightBuffer)*/
layout(binding = 3) uniform samplerBuffer fLightList;
uniform sampler2D Post1;
uniform sampler2D Post2;

@insertpiece(MaterialDecl)

@insertpiece( PassDecl )

@property( num_textures )uniform sampler2DArray textureMaps[@value( num_textures )];@end

layout(binding = 0) uniform samplerBuffer worldMatBuf;


uniform sampler2D GBuffer0;
uniform sampler2D GBuffer1;
uniform sampler2D GBuffer2;
uniform sampler2D GBuffer3;
uniform sampler2D GBuffer4;


@property(hlms_num_shadow_map_textures)
	uniform sampler2D texShadowMap[@value(hlms_num_shadow_map_textures)];
@end		

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

@property( !hlms_shadowcaster)
out vec4 color;

@end
@property(hlms_shadowcaster)
out float depth;
@end
@insertpiece(emptyVals)
@insertpiece(Spref)
void main()
{
	vec3 viewPos=inPs.pos.xyz;
	vec4 normal=vec4(normalize(inPs.normal),0);
	vec4 diffuse=vec4(0.2,0.3,0.9,0);
	float rough=20;
	vec4 specular=vec4(1);
	vec4 glow=vec4(0);
	float opacity=1.0;
	float orough=128.0;
	vec2 screenPos=vec2((gl_FragCoord.x/pass.screenx),(gl_FragCoord.y/pass.screeny));
	
	@insertpiece(Forward_Material)
	
	
	
	@property( !hlms_shadowcaster)
		vec3 normalo=vec3(0);
		@property( tex_normal_map )
			normalo=normalMP;
		@end

		vec4 alpha=texture(Post1,screenPos+normalo.xy*0.05 @property(!flatRefract)+inPs.normal.xy*0.2 @end  );

		@insertpiece(Forward_Lighting)

		color.xyz=mix(alpha.xyz+opacity_specular,(color.xyzz).xyz,opacity);
		color.a=1.0;
		
	@end	
	@property(hlms_shadowcaster)
		
		
		float scb=0;
		@property(vec4_shadow_const_bias)
			scb=(material.vec4_shadow_const_bias.x*10000)/ pow(pass.farClip,.3);
			
		@end
		depth=(0);
		depth	=((inPs.glPosition.z)/ pass.farClip)+scb;
	@end
}
@end
