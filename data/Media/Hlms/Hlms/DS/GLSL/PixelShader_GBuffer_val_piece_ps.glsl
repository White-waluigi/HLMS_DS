@piece(diffuse_value)
		
	@property(!hlms_shadowcaster)
	
	@property( !tex_diffuse_map)
		@property( vec4_diffuse  )	
			diffuse.rgb=material.vec4_diffuse.rgb;	
		@end
		@property( !vec4_diffuse  )	
			diffuse.rgb=vec3(1);	
		@end		
	@end
	
	@end
	@property( tex_diffuse_map)
		diffuse=  texture( textureMaps[@value(texv_diffuse_map)], vec3( 
		(vec4(inPs.uv@value(uvi_diffuse_map).xy,0,1)*material.texmat_@value(texd_diffuse_map)).xy,
		f2u(material.texloc_@value(texd_diffuse_map)) ) );
		diffuse=pow(inPs.uv@value(uvi_diffuse_map).x,inPs.uv@value(uvi_diffuse_map).y);
		
		
		vec2 v=mod(inPs.uv@value(uvi_diffuse_map),1.0);
		float fl= (   (int(v.x*1000.0))^(int(v.y*1000))  )/1000.0;



	@end
	

	
@end



@piece(glow_value)
@property( vec4_glow  )	
		glow.rgb=material.vec4_glow.rgb;	
	@end
	@property( !vec4_glow  )	
		glow.rgb=vec3(0);	
	@end		
	@property( tex_glow_map)
		glow*=  texture( textureMaps[@value(texv_glow_map)], vec3(
		 (vec4(inPs.uv@value(uvi_glow_map).xy,0,1)*material.texmat_@value(texd_glow_map)).xy,
		 f2u( material.texloc_@value(texd_glow_map) ) ) );
	@end

@end



@piece(specular_value)
		@property( vec4_specular  )	
			specular=material.vec4_specular;	
		@end
		@property( !vec4_specular  )	
			specular=vec4(vec3(0),32.0);	
		@end		
	@property( tex_specular_map)
		specular.rgb*=  texture( textureMaps[@value(texv_specular_map)], vec3( 
		(vec4(inPs.uv@value(uvi_specular_map).xy,0,1)*material.texmat_@value(texd_specular_map)).xy,
		f2u(material.texloc_@value(texd_specular_map) ) ) ).rgb;
	@end

@end



@piece(normal_value)


	normal.xyz=normalize(inPs.normal);
	normal.w=1.0;

	@property( tex_normal_map)
	
		vec3 geomNormal = normalize( inPs.normal );
		vec3 vTangent = normalize( inPs.tangent );

		//Get the TBN matrix
    	vec3 vBinormal   = normalize( cross( geomNormal, vTangent )@insertpiece( tbnApplyReflection ) );
		mat3 TBN		= mat3( vTangent, vBinormal, geomNormal );
	
		normal.xyz= getTSNormal( vec3( 
		(vec4(inPs.uv@value(uvi_normal_map).xy,0,1)*material.texmat_@value(texd_normal_map)).xy,  
		f2u(material.texloc_@value(texd_normal_map) ) ) );
		normal.xyz = normalize( (TBN * normal.xyz) );
		
		
	@end
		
@end

@piece(opacity_value)


		@property(!tex_opacity_map)
			@property( vec4_opacity  )	
				opacity=material.vec4_opacity.r;							
			@end
		@end
		
			
		@property( tex_opacity_map)
		


			opacity=  texture( textureMaps[@value(texv_opacity_map)], vec3( 
			(vec4(inPs.uv@value(uvi_opacity_map),0,1)*material.texmat_@value(texd_opacity_map)).xy, 
			f2u( material.texloc_@value(texd_opacity_map) ) ) ).g;
		@end
		@property(opacity-diffuse)
			opacity=diffuse.a;
		@end

@end

@piece(reflection_value)
@property( vec4_reflection  )	
		float reflection=material.vec4_reflection.r;
		
	@end
	@property( !vec4_reflection  )	
		float reflection=0.5;	
	@end		
	@property( tex_reflection_map)
		//Not sure about this
		//vec3 rNormal=(vec4(normal.xyz,0)*pass.View).xyz;
		vec3 rNormal=reflect(normalize(-inPs.pos.xyz),normal.xyz);
		
		vec2 ruv=vec2(asin(rNormal.x)/@insertpiece(PI) + 0.5 ,asin(rNormal.y)/@insertpiece(PI) + 0.5);
		diffuse=mix(diffuse,  
		texture( textureMaps[@value(texv_reflection_map)], vec3(ruv,
		 f2u( material.texloc_@value(texd_reflection_map) ) ) ),  
		 reflection);
	@end

@end



@piece(opacity_calc)
		@property(!opacity-sharp)
		if(opacity<0.999&&opacity>0.001){
			bool big=opacity>=0.5;
			if(!big){	
				float dval=opacity;
				uint uval=uint(1/dval);
				uint inc=uint(gl_FragCoord.y)%2u; 
				uint offsetx=uint(gl_FragCoord.x)+uint(gl_FragCoord.y*gl_FragCoord.y)+inc;
			
				
				if((offsetx)%uval!=0u){
					discard;
				}
			}
			else {	
				float dval=abs(1-opacity);
				uint uval=uint(1/dval);
				
				uint inc=uint(gl_FragCoord.y)%2u; 
				uint offsetx=uint(gl_FragCoord.x)+uint(gl_FragCoord.y*gl_FragCoord.y)+inc;
				
				if((offsetx)%uval==0u){
					discard;
				}
			}
		}else if(opacity<0.001){
			discard;
		}
		@end

		@property(opacity-sharp)
			float cutoff=0.5;
			@property(opacity-cutoff)
				cutoff=1.0-(1.0/(@value(opacity-cutoff)*1.0));
			@end

			if(opacity < cutoff) discard;
		@end
@end
