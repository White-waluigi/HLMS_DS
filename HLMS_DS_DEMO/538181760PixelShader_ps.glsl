//Datablock:	


//	Json Material


//Shadow Material

//Gbuffer Material



#version 400 core
#extension GL_ARB_shading_language_420pack: require
#extension GL_EXT_texture_array : enable


vec4 cubic(float v){
    vec4 n = vec4(1.0, 2.0, 3.0, 4.0) - v;
    vec4 s = n * n * n;
    float x = s.x;
    float y = s.y - 4.0 * s.x;
    float z = s.z - 4.0 * s.y + 6.0 * s.x;
    float w = 6.0 - x - y - z;
    return vec4(x, y, z, w) * (1.0/6.0);
}
vec4 textureBicubic(sampler2D sampler, vec2 texCoords,vec2 texSize){


   vec2 invTexSize = 1.0 / texSize;
   
   texCoords = texCoords * texSize - 0.5;

   
    vec2 fxy = fract(texCoords);
    texCoords -= fxy;

    vec4 xcubic = cubic(fxy.x);
    vec4 ycubic = cubic(fxy.y);

    vec4 c = texCoords.xxyy + vec2(-0.5, +1.5).xyxy;
    
    vec4 s = vec4(xcubic.xz + xcubic.yw, ycubic.xz + ycubic.yw);
    vec4 offset = c + vec4(xcubic.yw, ycubic.yw) / s;
    
    offset *= invTexSize.xxyy;
    
    vec4 sample0 = texture(sampler, offset.xz);
    vec4 sample1 = texture(sampler, offset.yz);
    vec4 sample2 = texture(sampler, offset.xw);
    vec4 sample3 = texture(sampler, offset.yw);

    float sx = s.x / (s.x + s.y);
    float sy = s.z / (s.z + s.w);

    return mix(
       mix(sample3, sample2, sx), mix(sample1, sample0, sx)
    , sy);
}









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




layout(binding = 1) uniform MaterialBuffer
{
	//usefull for finding out which materials have the same material block and a way to have materials without params, which glsl doesn't allow
	vec4 idColor;
	
		 vec4 vec4_diffuse;
	 vec4 vec4_glow;
	 vec4 vec4_leaf;
	 vec4 vec4_specular;
	 vec4 vec4_testvar55;
	 vec4 vec4_wave;




	
	vec4 texloc_0;
	

	
	mat4 texmat_0;

	
	
	vec4 texloc_1;
	

	
	mat4 texmat_1;

	
	
	vec4 texloc_2;
	

	
	mat4 texmat_2;

	

/**/



} material;




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
	

	
} pass;








uniform sampler2DArray textureMaps[3];layout(binding = 0) uniform samplerBuffer worldMatBuf;



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


in block
{

        flat uint drawId;
		vec4 pos;
		vec3 normal;
		vec3 vertex;
		vec4 position;
		vec3 tangent;
		vec4 worldPos;
		vec4 glPosition;
		float depth;
				
			
		vec2 uv0;		
				

} inPs;
in vec4 vcolor;


out vec4 depth;

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




	vec4 diffuse=vec4(1,0.5,0,0);
	vec4 normal=vec4(1,0.5,0,0);
	vec4 specular=vec4(1,0,1,0);


	
		
			

	

	


		
							
			
				





vec4 leaf=material.vec4_leaf;

 leaf =  texture( textureMaps[0], vec3( 
(vec4(inPs.uv0.xy,0,1)*material.texmat_0).xy, 
f2u( material.texloc_0 ) ) ); 


vec4 perlin =  texture( textureMaps[1], vec3( 
(vec4(inPs.uv0.xy,0,1)*material.texmat_1).xy, 
f2u( material.texloc_1 ) ) ); 


vec4 testmap =  texture( textureMaps[2], vec3( 
(vec4(inPs.uv0.xy,0,1)*material.texmat_2).xy, 
f2u( material.texloc_2 ) ) ); 


vec4 testvar55=material.vec4_testvar55;

vec4 wave=material.vec4_wave;

// glow *= tan(material.wave.x)*2.0;




	diffuse=rainbow((inPs.uv0.x+inPs.uv0.y+pass.time.x)+perlin.r);
//opacity*=sqrt(1-pow(leaf.r,2.0));





	
		
												
	
		
		
		//depth=texelFetch(worldMatBuf,int(inPs.drawId)+(int(gl_FragCoord.x)%16) );

		//depth=vec4((length(inPs.pos.xyz) / pass.farClip));
		
		//depth=vcolor;
		//depth=normalize(vcolor);
		//depth=inPs.pos;	
		depth.x	=inPs.glPosition.z/ pass.farClip;


		//depth=vec4(inPs.pos.xyzz);
		
		
			
	
	
	diffuse.a=0.9;
 }
 
 

