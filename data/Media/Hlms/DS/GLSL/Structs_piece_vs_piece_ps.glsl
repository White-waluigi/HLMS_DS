@piece( InstanceDecl )
//Uniforms that change per Item/Entity
layout(binding = 2) uniform InstanceBuffer
{
    //.x =
	//The lower 9 bits contain the material's start index.
    //The higher 23 bits contain the world matrix start index.
    //
    //.y =
    //shadowConstantBias. Send the bias directly to avoid an
    //unnecessary indirection during the shadow mapping pass.
    //Must be loaded with uintBitsToFloat
    uvec4 worldMaterialIdx[4096];
} instance;
@end

@piece( VStoPS_block )
        flat uint drawId;
		vec4 pos;
		vec3 normal;
		vec3 vertex;
		vec4 position;
		vec3 tangent;
		vec4 worldPos;
		vec4 glPosition;
		
		mat4 worldMat;
		
		vec4 sF;
		vec4 eF;
				
		vec4 fc[4];
		
		float depth;
		@property( hlms_qtangent )
			flat float biNormalReflection;
		@end
		
			@foreach( hlms_uv_count, n )
		vec@value( hlms_uv_count@n ) uv@n;@end
		
		@property(hlms_num_shadow_maps)
			vec4 posL[@value(hlms_num_shadow_maps)];@end
		
			
			
		@insertpiece(custom_VStoPS)

@end


@piece( PassDecl )
struct ShadowData{
		vec4 size;
		vec4 lightID;
		vec4 depthrange;
		mat4 ShadowCamMat;
};  


layout(binding = 0) uniform PassBuffer
{
	vec4 debug;
	
	mat4 Proj;
	
	mat4 View;
	
	mat4 ViewProj;
	
	mat4 InvView;
	
	vec4 time;
	
		
	float nearClip;
	float farClip;
	float screenx;
	float screeny;
	
	vec4 farCorners;
	
	vec4 flip;
	
	float near;
	float far;
	float Invdepthrange;
	float depthrange;
	
	vec4 test;


} pass;

@end

@piece( MaterialDecl )

layout(binding = 1) uniform MaterialBuffer
{
	//usefull for finding out which materials have the same material block and a way to have materials without params, which glsl doesn't allow
	vec4 idColor;
	
	@insertpiece(MaterialVec4Params)


@foreach( NumTextureDefines, n )
	
	vec4 texloc_@n;
	

	
	mat4 texmat_@n;

	@property(NumSubTextures@n)
		vec4 texloc_sub_@n[@value(NumSubTextures@n)];
	@end
@end

} material;

@end

@piece(LightBuffer)



struct Light{	
    vec4 position;
    vec4 diffuse;
    vec4 specular;
    vec4 attenuation;
    vec4 spotdirection;
    vec4 spotparams;
    vec4 lightparams;
    vec4 lightsettings;
    vec4 shadowParams;
    vec4 shadowQualityParams;
    float data[54*4];
};  
layout(binding = 3) uniform LightBuffer
{

	Light lights[@value(MAX_FORWARD_LIGHTS)];

    
} lightb;
@end


@insertpiece(TextureHelper)
@end
