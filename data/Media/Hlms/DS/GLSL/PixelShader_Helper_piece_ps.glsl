@piece(Helper)
@insertpiece( Common_Matrix_DeclUnpackMatrix4x4 )
@insertpiece( Common_Matrix_DeclUnpackMatrix4x3 )
vec4 rainbow(float phase,bool cap=false)
{
	if(cap&&(phase<0||phase>1)){
		return vec4(0);
	}
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
uint f2u(float f){
	return floatBitsToUint(f);
}
uint f2u(vec4 f){
	return floatBitsToUint(f)[0];
}
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

float p2c(uvec2 pos,int num){
	@insertpiece(NumAr)
			
	return numar[num*5*4 + pos.y*4 + pos.x ];
	
}
float log10(float num){
	return log(num)/log(10);
}
vec4 print(vec2 pos_t,vec2 offset, float num){
	
	vec2 pos=(pos_t )-(offset); 

	

	ivec2 spos=ivec2(int(pos.x/10),int(pos.y/10));
			
	int ipx=int(spos.x/4.0);
	int ipy=int(spos.y/5.0);
	
	ivec2 cpos=ivec2(mod(spos.x,4),mod(spos.y,5));
	
	int rpl=int(log10(abs(num)));
 
	
	int rp[9];
	
	
	rp[1]=int(mod(int(  num/pow(10,rpl  ) ),10 ) );
		
	rp[3]=int(mod(int(  num/pow(10,rpl-1) ),10 ) );
	rp[4]=int(mod(int(  num/pow(10,rpl-2) ),10 ) );
	rp[5]=int(mod(int(  num/pow(10,rpl-3) ),10 ) );
	


	
	
	rp[8]=abs(rpl);
	
	if(pos.x<0||pos.y<0){
		return vec4(0);
	}
	
	if(ipx>8){
		return vec4(0);
	}
	if(ipx<0){
		return vec4(0);
	}

	if(ipy!=0){
		return vec4(0);
	}

	//return vec4(cpos/5.0,0,0);
	if(ipx==0){
		if(sign(num)==-1){
			return vec4(p2c(cpos,10));
		}else{
			return vec4(0);
		}
	}
	if(ipx==2){
		return vec4(p2c(cpos,11));
	}
	if(ipx==6){
		return vec4(p2c(cpos,12));
	}
	if(ipx==7){
		if(sign(rpl)==-1){
			return vec4(p2c(cpos,10));
		}else{
			return vec4(0);
		}
	}
	return vec4(p2c(cpos,rp[ipx]));

}
//Quick Integer Sample
ivec4 qis(samplerBuffer s,int i){
	return floatBitsToInt(texelFetch(s,i));
}
@end