//Datablock:	
#define PI 3.14159625


//Shadow Material

//Gbuffer Material



#version 400 core
#extension GL_ARB_shading_language_420pack: require
#extension GL_EXT_texture_array : enable
layout(std140) uniform;

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
vec4 blend(vec4 s1,vec4 s2, float b){
	return mix(s1,s2,b);
	
}
vec4 blend(vec4 sb,vec4 s1, vec4 s2,vec4 s3, vec4 b){
	vec4	retval=mix(vec4(0),s1,b.r);
			retval+=mix(vec4(0),s2,b.g);
			retval+=mix(vec4(0),s3,b.b);
			retval=mix(sb,retval,b.a);
	return retval;
}
vec4 ominf(vec4 data){
	vec4 retval=data;
	//min doesn't work for some reason
	if(data.x>1)
		retval.x=1;
	if(data.y>1)
		retval.y=1;
	if(data.z>1)
		retval.z=1;
	if(data.w>1)
		retval.w=1;
	
	return retval;
	
	
}
vec4 inside(vec4 d,vec4 f,vec4 t){
	
	vec4 retval=vec4(0); 
	if(d.x<f.x){
		retval.x+=.5;
	}
	if(d.y<f.y){
		retval.z+=.5;
	}	

	if(d.x>t.x){
		retval.x+=.5;
	}
	if(d.y>t.y){
		retval.z+=.5;
	}	
	return retval;
}
bool insideTri(vec2 p, vec2 a, vec2 b, vec2 c ){
	vec2 v0 = vec2(c.x - a.x, c.y - a.y);
	vec2 v1 = vec2(b.x - a.x, b.y - a.y);
	vec2 v2 = vec2(p.x - a.x, p.y - a.y);

    float dot00 = (v0.x * v0.x) + (v0.y * v0.y);
    float dot01 = (v0.x * v1.x) + (v0.y * v1.y);
    float dot02 = (v0.x * v2.x) + (v0.y * v2.y);
    float dot11 = (v1.x * v1.x) + (v1.y * v1.y);
    float dot12 = (v1.x * v2.x) + (v1.y * v2.y);

    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);

    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return ((  (u >= 0) && (v >= 0) && (u + v < 1)  ));
	
}
vec2 cropUV(vec2 uv, vec2 start, vec2 end){
	
	
	return mix(start,end,uv);
	
}
vec4 cropUV(vec4 uv, vec2 start, vec2 end){
	
	vec4 retval=uv;
	uv.xy=mix(start,end,uv.xy);
	uv.zw=1/uv.xy;
	
	return retval;
	
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
	
	



	
	vec4 texloc_0;
	

	
	mat4 texmat_0;

	

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








uniform sampler2DArray textureMaps[1];layout(binding = 0) uniform samplerBuffer worldMatBuf;



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
		
		mat4 worldMat;
		
		vec4 sF;
		vec4 eF;
				
		vec4 fc[4];
		
		float depth;
				
			
		vec2 uv0;		
				
			
			
		


} inPs;


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


	
		
		
		diffuse=  texture( textureMaps[0], vec3( 
		(vec4(inPs.uv0.xy,0,1)*material.texmat_0).xy,
		f2u(material.texloc_0) ) );
//		diffuse=pow(inPs.uv0.x,inPs.uv0.y);
		
		
		

		

	

	


		
							
			
				









	




	
		
												
	
		
		
		float scb=0;
	
				depth=vec4(0);
		depth.x	=((inPs.glPosition.z)/ pass.farClip)+scb;
		//depth.yz=vec2(tan(tan(screenPos.x*100.0)),sin(sin(screenPos.y*100.0)));
		
		int px=int(screenPos.x*100);
		int py=int(screenPos.y*100);
		//depth=material.idColor;
		//depth.yz=vec2(float((px%2==0)^^ (py%2==0)),float(!((px%2==0)^^ (py%2==0))) );
		
		
			
	
	
	diffuse.a=0.9;
 }
 
 

