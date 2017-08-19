@piece(SampleShadow)



		vec2 Soffset=(shadowSampleTexCoord.xy+(shadowRes.zw*samplingoffset));

		vec2 centroidUV = (Soffset+(shadowRes.zw*0.5));
		vec4 sampl=textureGather(texShadowMap[shadowTexID], Soffset);
		
		
		for(int i=0;i<4;i++){

			
			
			
			vec2 offset=vec2(0); 
			if(i==1){
				offset.y=vary;
			}
			if(i==2){
				offset.x=varx;
			}
			if(i==3){
				offset.x=varx;
				offset.y=vary;
			}

			float samp=sampl[i];
						
			float shadowDepth=samp;
			
			
			//sampl[i]>0.9999 to generate shadows beyond shadowcam range
			if( (shadowDistance)<(shadowDepth)||sampl[i]>0.9999){
				ls[i]=1.0;
			}
			

		}
		
		float a = mix(ls[3], ls[0], fra.y);
        float b = mix(ls[2], ls[1], fra.y);
        float c = mix(a, b, fra.x);

        float ssr=(((samplerate*2.0)+1.0)*((samplerate*2.0)+1.0));






        if(floatBitsToUint(pass.debug.z)%4u==0u){
        	ShadowVal+=vec4((c));
        }else if(floatBitsToUint(pass.debug.z)%4u==1u){
        	ShadowVal+=vec4(( (ls[0]+ls[1]+ls[2]+ls[3]) / 4.0));
        }else{
        	

        	float samp=texture(texShadowMap[shadowTexID], centroidUV).x;
        	float shadowDepth=samp;
			if( (shadowDistance)<(shadowDepth)){
				ShadowVal+=vec4(1.0);
			}
        }
        
        
        
@end
@piece(ShadowDebug)
	uint numtex=@value(hlms_num_shadow_map_textures)u;

	float fL=screenPos.x*3.0;
	float ffL=(screenPos.y*float(3));
		
	uint i=uint(fL);
	uint ii=uint(ffL);
		
	uint curid=((i)+ii*3u);
	vec2 coords=vec2(mod(texCoord.x,0.33333),mod(texCoord.y,0.3333 ))*vec2(3.0,3.0);
	
	final=texture(texShadowMap[curid], coords);
	if((curid>=numtex)){
		final.rgb=(light_diffuse.rgb*diffuse)+glow;
		//final.rgb=vec3(0.4,0.3,0.1);
	}

@end