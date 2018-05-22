
@piece( PassInternalDecl )
	mat4 viewProjf;
	mat4 view;
	mat4 proj;
	
	vec4 depth;
	vec4 resolution;
	
@end
@piece( PassDecl )
//Uniforms that change per pass
layout_constbuffer(binding = 0) uniform PassBuffer
{
	@insertpiece( PassInternalDecl )
} passBuf;
@end

@piece( MaterialDecl )
layout(binding = 1) uniform MaterialBuffer
{
	//usefull for finding out which materials have the same material block and a way to have materials without params, which glsl doesn't allow
	vec4 idColor;
@insertpiece(MaterialDataParams)



} material;
@end


@piece( InstanceDecl )
//Uniforms that change per Item/Entity
layout(binding = 2) uniform InstanceBuffer
{
	//.x =
	//Contains the material's start index.
	//
	//.y =
	//shadowConstantBias. Send the bias directly to avoid an
	//unnecessary indirection during the shadow mapping pass.
	//Must be loaded with uintBitsToFloat
	//
	//.z =
	//Contains 0 or 1 to index into passBuf.viewProj[]. Only used
	//if hlms_identity_viewproj_dynamic is set.
	vec4 worldMaterialIdx[4096];
} instance;
@end

@piece( VStoPS_block )
	@property( !hlms_shadowcaster )
		
		@property( hlms_colour )vec4 colour;@end
		@foreach( out_uv_half_count, n )
			vec@value( out_uv_half_count@n ) uv@n;@end
	@end
	@property( hlms_shadowcaster )
		@property( (!hlms_shadow_uses_depth_texture || exponential_shadow_maps) && !hlms_shadowcaster_point )
			float depth;
		@end
		@property( hlms_shadowcaster_point )
			vec3 toCameraWS;
			@property( !exponential_shadow_maps )
				flat float constBias;
			@end
		@end
	@end
	vec3 normal;
	vec3 uv0;
	flat uint drawId;
	@insertpiece( custom_VStoPS )
@end
