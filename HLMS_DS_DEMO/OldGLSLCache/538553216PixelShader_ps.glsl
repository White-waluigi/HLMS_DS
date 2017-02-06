//Datablock:	
#define PI 3.14159625


//Light Material 
	
//AMBIENT
		


	


#version 400 core
#extension GL_ARB_shading_language_420pack: require


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




float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}






layout(binding = 1) uniform MaterialBuffer
{
	//usefull for finding out which materials have the same material block and a way to have materials without params, which glsl doesn't allow
	vec4 idColor;
	
		 vec4 vec4_diffuse;
	 vec4 vec4_lightparams;





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
	

	
		
			vec4 pssmSplitPoints[3];
				ShadowData shadowD[5];
	
} pass;





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



		
			uniform sampler2D texShadowMap[5];
		
		uniform sampler2D GBuffer0;
		uniform sampler2D GBuffer1;
		uniform sampler2D GBuffer2;
		uniform sampler2D GBuffer3;
		uniform sampler2D GBuffer4;
	

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
				
					
		
			vec4 posL[5];		

} inPs;

uint f2u(float f){
	return floatBitsToUint(f);
}
uint f2u(vec4 f){
	return floatBitsToUint(f)[0];
}

in vec4 vcolor;
out vec4 final;



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


void main() {


	
	
	

	vec2 screenPos=vec2((gl_FragCoord.x/pass.screenx),(gl_FragCoord.y/pass.screeny));

	vec2 texCoord=vec2(screenPos.x,screenPos.y);
	

		
	vec3 diffuse=texture2D(GBuffer0 ,texCoord).rgb;
	float depth=texture2D(GBuffer1 ,texCoord).a;
	vec3 normal=texture2D(GBuffer1 ,texCoord).rgb;
	vec3 specular=texture2D(GBuffer3 ,texCoord).rgb;
	float rough=texture2D(GBuffer3 ,texCoord).w;
	
	float Sdepth=texture2D(GBuffer2 ,texCoord).x;
	
	vec3 glow=texture2D(GBuffer4 ,texCoord).rgb;


	
	
		

	
		
	uint light_type					=floatBitsToUint(material.vec4_lightparams.x);

	float light_power				=material.vec4_lightparams.y;	
	
	vec4 light_diffuse				=material.vec4_diffuse;

	


	
	if(floatBitsToUint(pass.debug.y)==1u){
		diffuse=vec3(1);
	}else if(floatBitsToUint(pass.debug.y)==2u){


	}else if(floatBitsToUint(pass.debug.y)==3u){
		glow=vec3(0);
	}else if(floatBitsToUint(pass.debug.y)==4u){
		diffuse=vec3(0);
	}else if(floatBitsToUint(pass.debug.y)==5u){
		specular=vec3(0);
	}else if(floatBitsToUint(pass.debug.y)==6u){
		vec3 swap=diffuse;
		diffuse=specular;
		specular=swap;
	}else if(floatBitsToUint(pass.debug.y)==8u){
		light_diffuse=vec4(0);
	}else if(floatBitsToUint(pass.debug.y)==9u){
		if(light_type!=0u&&light_type!=4u){
			final=vec4(0,0,0,0);
			return;
		}

	}



	
	
	
	//vec2 projPos=(vec2((gl_FragCoord.x/pass.screenx),(gl_FragCoord.y/pass.screeny) )-0.5)/0.5;
	
	vec2 projPos=(vec2(texCoord.x,texCoord.y) - 0.5) / 0.5;
	
	vec3 ray = vec3(projPos.x,projPos.y, 1)*pass.farCorners.xyz;
	vec3 viewPos = normalize(ray)*(depth*pass.farClip);
 
 	vec3 worldPos = (pass.InvView* vec4(viewPos, 1)).xyz;
 
 
   	float f=pass.farClip;
	float n = pass.nearClip;
   	
	vec3 objToLightVec ;
	vec3 total_light_contrib;






	
		

	//final.rgb=(light_diffuse.rgb*diffuse)+glow;
	

		
		
	
	
		
	
	

	
		
		
		

	
	
		

	if(floatBitsToUint(pass.debug.x)==0u){
		final.rgb=(light_diffuse.rgb*diffuse)+glow;		

		return;
	}	
	else if(floatBitsToUint(pass.debug.x)==1u){
			if(light_type>=1u){
	
				final=vec4(depth);
			}else {
				final=vec4(0);
			}	
		return;
	}else if(floatBitsToUint(pass.debug.x)==2u){
		final=vec4(0);
		if(light_type==4u){
			final=vec4(normal.rgbb);
		}
		return;
	}else if(floatBitsToUint(pass.debug.x)==3u){
		final=vec4(0);
		if(light_type==4u){
			final=vec4(diffuse.rgbb);
		}
		return;
	}else if(floatBitsToUint(pass.debug.x)==4u){
		final=vec4(0);
		if(light_type==4u){
			final=vec4(glow.rgbb);
		}
		return;
	}else if(floatBitsToUint(pass.debug.x)==5u){
		final=vec4(0);
		if(light_type==4u){
			final=vec4(specular.rgbb);
		}
		return;
	}else if(floatBitsToUint(pass.debug.x)==6u){
		
		final.rgb=(light_diffuse.rgb*diffuse);		

		return;
	}else if(floatBitsToUint(pass.debug.x)==7u){
		
	uint numtex=5u;

	float fL=screenPos.x*3.0;
	float ffL=(screenPos.y*float(3));
		
	uint i=uint(fL);
	uint ii=uint(ffL);
		
	uint curid=((i)+ii*3u);
	vec2 coords=vec2(mod(texCoord.x,0.33333),mod(texCoord.y,0.3333 ))*vec2(3.0,3.0);
	
	final=texture(texShadowMap[curid], coords);
	
	if(curid>=numtex){
		final.rgb=(light_diffuse.rgb*diffuse)+glow;

	}


		return;
		
	}else if(floatBitsToUint(pass.debug.x)==8u){
	 	if(light_type==4u){
			if(texCoord.x>0.666666){
				if(texCoord.y>0.5){
					final.rgb=normal.rgb;
				}else {
					final.rgb=diffuse.rgb;
				}
				
			}else if(texCoord.x>0.333333){
				if(texCoord.y>0.5){
					final.rgb=specular.rgb;
				}
				else {
					final.rgb=(light_diffuse.rgb*diffuse)+glow;		
				}
				
				
			}else if(texCoord.x>0.133333){
				if(texCoord.y>0.5){
					final=vec4(1,0,0,0);
				}
				else {
					final=vec4(0,0,1,0);
				}		
				}else {
		
					if(texCoord.y>0.5){
						final.rgb=glow.rgb;
					}
					else {
					final=vec4(depth);
					}

				}
			
			
			}else {
				final=vec4(0);
			}
	} else if (floatBitsToUint(pass.debug.x)==9u){
	
			final.rgb=glow.rgb;
					
 	}
	

		

	
 }
 

