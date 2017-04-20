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
	

	@property(hlms_num_shadow_maps)
		@property( hlms_pssm_splits )
	//		vec4 pssmSplitPoints[@value(hlms_pssm_splits)];
		@end
	//	ShadowData shadowD[@value(hlms_num_shadow_maps)];
	@end

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


/*@insertpiece(AutoParams)*/



} material;

@end

@piece(RAND)
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
@end
@piece(RAINBOW)

vec4 rainbow(float phase)
{
	float p=mod(phase,1.0);
	float c=p*3.0;
	float level = c;

	float x=mod(c,1.0);
	float y=1-mod(c,1.0);

	float r,g,b;
	if(level<1){
		r=y;
		g=x;
		b=0;
	}else if(level<2){
		r=0;
		g=y;
		b=x;
	}else if(level<3){
		r=x;
		g=0;
		b=y;
	}

	return vec4(r,g,b,0);
}
@end

@insertpiece(TextureHelper)
@end
