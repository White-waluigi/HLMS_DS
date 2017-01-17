@piece(Shadow)
/***************************************************************Shadow**************************************************************************************************/
		if(depth<1.0){
	
		ShadowVal=vec4(0);
		
		
		
		float fL=screenPos.x*3.0;
		float ffL=(screenPos.y*3.0);
		uint IDoffset=0;
		float PPoffset=0.00001;
		


		@property(LIGHTTYPE_Direc)
		@end

		@property( !hlms_pssm_splits || !LIGHTTYPE_Direc)
		uint shadowID= floatBitsToUint(material.vec4_shadowParams.x);
		@end
		
		@property(LIGHTTYPE_Direc)
		
		@property( hlms_pssm_splits )

		uint inc=0u;
		
		float Pdepth	= (Sdepth  );


			//ShadowVal=vec4(0.5,0.5,0.5,0);

		
		if((Pdepth)<= material.vec4_shadowPssmSplits[1].x){
			//ShadowVal=vec4(0,1,1,0);
       	}
		@foreach( hlms_pssm_splits, n, 1 )
		else if( (Pdepth)<= material.vec4_shadowPssmSplits[@n+1].x ){
			inc=@nu;
			
			 
			//ShadowVal=vec4(@n==0,@n==1,@n==2,0);
			
		}

		@end 

		

		uint shadowID= floatBitsToUint(material.vec4_shadowParams.x)+inc;
		IDoffset=inc;
		@end 	
		
		@end
		


		uint i=uint(fL);
		uint ii=uint(ffL);
		
		uint curid=((i*3u)+ii)%9u;

		vec4 shadowRes=	material.vec4_shadowRes[IDoffset];
		mat4 shadowMat= material.vec4_shadowMat[IDoffset];
		vec4 shadowDr= material.vec4_shadowDr[IDoffset];
		
	vec4 shadowProjPos = 	shadowMat * vec4(worldPos,1);
	float shadowDistance=(((shadowProjPos).z-PPoffset)/shadowDr.y);
	//float shadowDistance=(shadowProjPos).z;
	
	//vec4 shadowProjPos =  pass.ShadowCamMat[curid] * (vec4(inPs.worldPos));
	//vec4 shadowProjPos =inPs.worldPos; 	
    shadowProjPos /= shadowProjPos.w;
    vec2 shadowSampleTexCoord = vec2(shadowProjPos.x,shadowProjPos.y);
		
		
	//shadowSampleTexCoord=inPs.posL[shadowID].xy/inPs.posL[shadowID].w;
	

	//vec2 texCoord=vec2(screenPos.x,1-screenPos.y);
	vec2 coords=vec2(mod(texCoord.x,0.33333),mod(texCoord.y,0.5))*vec2(3.0,2.0);
	//glow=texture2D(texShadowMap[curid], texCoord);
	
	//glow=texelFetch(worldMatBuf,(int(gl_FragCoord.x)));

	
	




		int samplerate=int(f2u(material.vec4_shadowQualityParams.x));
		float samplesize=((material.vec4_shadowQualityParams.y));


		vec2 samplingoffset=vec2(0,0);

		samplerate=1;
		samplesize=0.2;

		float varx=shadowRes.z;
		float vary=shadowRes.w;

		vec2 fra=fract(shadowSampleTexCoord*shadowRes.xy+0.5);

		fra=fract( shadowSampleTexCoord * shadowRes.xy + 0.50196 /*<- For some reason 0.00196 fixes inaccurate shadows*/);
		vec4 ls= vec4(0);


		for(int i=-samplerate;i<=samplerate;i++){
			for(int ii=-samplerate;ii<=samplerate;ii++){
				
				samplingoffset=vec2(i,ii);



			}
		}
		ShadowVal/=pow(samplerate*2+1,2);
		samplingoffset=vec2(0);
		@insertpiece(SampleShadow)

		/*
		@insertpiece(SampleShadowBC)
		*/
		int samplex=2;
		int sampley=2;
		

		

		if(floatBitsToUint(pass.debug.z)%4u==2u){

			float samp=texture(texShadowMap[shadowID], shadowSampleTexCoord)[0];
			float shadowDepth=samp;
        	ShadowVal=vec4(0);

			if( (shadowDistance)<(shadowDepth)){
	        	ShadowVal+=vec4(1);
			}
		}

		if(ShadowVal.x>1.1){
			final=vec4(0.4,0.3,0.7,0);
			return;
		}
		
		//ShadowVal=vec4( pow(ShadowVal.x,2.0) );
		

		
		}

@end



@piece(SampleShadow)


		vec2 Soffset=(shadowSampleTexCoord.xy+(shadowRes.zw*samplingoffset));

		vec2 centroidUV = (Soffset+(shadowRes.zw*0.5));
		vec4 sampl=textureGather(texShadowMap[shadowID], Soffset);

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
			//float samp=texture(texShadowMap[shadowID], centroidUV+offset).x;
			float samp=sampl[i];
			float shadowDepth=samp;
		
			if( (shadowDistance)<(shadowDepth)){
				ls[i]=1.0;
			}
			

		}
		float a = mix(ls[3], ls[0], fra.y);
        float b = mix(ls[2], ls[1], fra.y);
        float c = mix(a, b, fra.x);

        float ssr=(((samplerate*2.0)+1.0)*((samplerate*2.0)+1.0));


		float samp=texture(texShadowMap[shadowID], shadowSampleTexCoord+shadowRes.zw*samplingoffset)[0];
		float shadowDepth=samp;



        if(floatBitsToUint(pass.debug.z)%4u==0u){
        	ShadowVal+=vec4((c));
        }else if(floatBitsToUint(pass.debug.z)%4u==1u){
        	ShadowVal+=vec4(( (ls[0]+ls[1]+ls[2]+ls[3]) / 4.0));
        }else{
        	float samp=texture(texShadowMap[shadowID], centroidUV).x;
        	float shadowDepth=samp;
			if( (shadowDistance)<(shadowDepth)){
				ShadowVal+=vec4(1.0);
			}
        }

@end
@piece(ShadowDebug)
	uint numtex=@value(hlms_num_shadow_maps)u;

	float fL=screenPos.x*3.0;
	float ffL=(screenPos.y*float(3));
		
	uint i=uint(fL);
	uint ii=uint(ffL);
		
	uint curid=((i)+ii*3u);
	vec2 coords=vec2(mod(texCoord.x,0.33333),mod(texCoord.y,0.3333 ))*vec2(3.0,3.0);
	
	final=texture(texShadowMap[curid], coords)+(diffuse.rgbb/100.0);
	
	if(curid>=numtex){
		final.rgb=(light_diffuse.rgb*diffuse)+glow;

	}

@end
