@piece(Shadow)
/***************************************************************Shadow**************************************************************************************************/
		//Why is this static?
		vec4 s_UV_coords[@value(hlms_num_shadow_map_lights)];

		@foreach( hlms_num_shadow_map_lights, n )
				s_UV_coords[@n].x=@value( hlms_shadowmap@n_uv_min_x_int ).@value( hlms_shadowmap@n_uv_min_x_fract );
				s_UV_coords[@n].y=@value( hlms_shadowmap@n_uv_min_y_int ).@value( hlms_shadowmap@n_uv_min_y_fract );
				
				s_UV_coords[@n].z=@value( hlms_shadowmap@n_uv_max_x_int ).@value( hlms_shadowmap@n_uv_max_x_fract ) ;
				s_UV_coords[@n].w=@value( hlms_shadowmap@n_uv_max_y_int ).@value( hlms_shadowmap@n_uv_max_y_fract ) ;		
		@end
		
		
		if(depth<1.0){
	
		ShadowVal=vec4(0);
		
		
		
		float fL=screenPos.x*3.0;
		float ffL=(screenPos.y*3.0);
		uint IDoffset=0;
		float PPoffset=0.00001;
		


		
		
		//uint shadowTexID= floatBitsToUint(material.vec4_shadowParams.w);
		
		//can't have dynamic linking
		

		uint shadowTexID= @value(Shadow_Atlas_ID)-1;

		@property( !hlms_pssm_splits || !LIGHTTYPE_Direc)
		uint shadowID= floatBitsToUint(material.vec4_shadowParams.x);
		@end
		
		@property(LIGHTTYPE_Direc)
		
		@property( hlms_pssm_splits )

		uint inc=0u;
		
		float Pdepth	= (Sdepth  );



		
		if((Pdepth)<= material.vec4_shadowPssmSplits[1].x){
       	}
		@foreach( hlms_pssm_splits, n, 1 )
		else if( (Pdepth)<= material.vec4_shadowPssmSplits[@n+1].x ){
			inc=@nu;
			
			 

		}

		@end 

		

		uint shadowID= floatBitsToUint(material.vec4_shadowParams.x)+inc;
		IDoffset=inc;
		
		@end 	
		
		@end



	uint i=uint(fL);
	uint ii=uint(ffL);
		
	uint curid=((i*3u)+ii)%9u;
	vec4 shadowRes=material.vec4_shadowRes[IDoffset];

	
	mat4 shadowMat= material.vec4_shadowMat[IDoffset];
	vec4 shadowDr= material.vec4_shadowDr[IDoffset];
		
	vec4 shadowProjPos = 	shadowMat * vec4(worldPos,1);
	float shadowDistance=(((shadowProjPos).z-PPoffset)/shadowDr.y);

    shadowProjPos /= shadowProjPos.w;
    vec2 shadowSampleTexCoord = vec2(shadowProjPos.x,shadowProjPos.y);
		
		
	

	vec2 coords=vec2(mod(texCoord.x,0.33333),mod(texCoord.y,0.5))*vec2(3.0,2.0);





		int samplerate=int(f2u(material.vec4_shadowQualityParams.x));
		float samplesize=((material.vec4_shadowQualityParams.y));


		vec2 samplingoffset=vec2(0,0);

		samplerate=0;
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


		int samplex=2;
		int sampley=2;
		

		

		if(floatBitsToUint(pass.debug.z)%4u==2u){

			float samp=texture(texShadowMap[shadowTexID], shadowSampleTexCoord)[0];
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
		
		
		//Makes Shadows softer
		ShadowVal=vec4( pow(ShadowVal.x,5.0) );

		}

@end




