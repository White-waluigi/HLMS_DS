@piece(DebugAmbient)
		
		
	//gl_FragDepth=depth;
	if(floatBitsToUint(pass.debug.x)==0u){
		final.rgb=((light_diffuse.rgb*diffuse)+glow);
		
		
		return;
	}	
	else if(floatBitsToUint(pass.debug.x)==1u){
			if(light_type>=1u){
				final.rgb=vec3(depth);
			}else {
				final.rgb=vec3(0);
			}	
		return;
	}else if(floatBitsToUint(pass.debug.x)==2u){
		if(light_type==4u){
			final.rgb=vec3(normal.rgb);
		}
		return;
	}else if(floatBitsToUint(pass.debug.x)==3u){
		if(light_type==4u){
			final.rgb=vec3(diffuse.rgb);
		}
		return;
	}else if(floatBitsToUint(pass.debug.x)==4u){
		if(light_type==4u){
			final.rgb=vec3(glow.rgb);
		}
		return;
	}else if(floatBitsToUint(pass.debug.x)==5u){
		if(light_type==4u){
			final.rgb=vec3(specular.rgb);
		}
		return;
	}else if(floatBitsToUint(pass.debug.x)==6u){
		
		final.rgb=(light_diffuse.rgb*diffuse);		

		return;
	}else if(floatBitsToUint(pass.debug.x)==7u){
		@insertpiece(ShadowDebug)
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
				
				
			}else if(texCoord.x>0.3){
				if(texCoord.y>0.5){
					final.rgb=vec3(1,0,0);
				}
				else {
					final.rgb=vec3(0,0,1);
				}		
				}else {
		
					if(texCoord.y>0.5){
						final.rgb=glow.rgb;
					}
					else {
					final.rgb=vec3(depth);
					}

				}
			
			
			}else {
				final.rgb=vec3(0);
			}
	} else if (floatBitsToUint(pass.debug.x)==9u){
	@property(LIGHTTYPE_Ambient)
			final.rgb=glow.rgb;
	@end
	@property(!LIGHTTYPE_Ambient)
			final.rgb=vec3(0,0,0,0);
	@end
			
 	}
	
@end
@piece(Fullbright)
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
			final.rgb=vec3(0,0,0);
			return;
		}

	}
@end
@piece(Debug)

	if(floatBitsToUint(pass.debug.x)==0u){
		if(floatBitsToUint(pass.debug.y)==7u){
			ShadowVal=vec4(1);
		}


		
		final=ominf(  vec4((total_light_contrib), 0.0))*ShadowVal;

		return;
	}else if(floatBitsToUint(pass.debug.x)==6u){
		final=vec4(0,0.0,0,0);
	
		if(light_type==1u){
			final=vec4(0.1,0.0,0,0);
			gl_FragDepth=0;
		}
		if(light_type==2u){
			final=vec4(0,0.1,0,0);
			gl_FragDepth=0;
		}
		return;
	}else {
		final=vec4(0);
		return;
	}
@end
