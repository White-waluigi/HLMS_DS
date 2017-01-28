@piece(DebugAmbient)

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
	@property(LIGHTTYPE_Ambient)
			final.rgb=glow.rgb;
	@end
	@property(!LIGHTTYPE_Ambient)
			final=vec4(0,0,0,0);
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
			final=vec4(0,0,0,0);
			return;
		}

	}
@end
@piece(Debug)

	if(floatBitsToUint(pass.debug.x)==0u){
		if(floatBitsToUint(pass.debug.y)==7u){
			ShadowVal=vec4(1);
		}

		final=vec4((total_light_contrib*light_power), 0.0)*ShadowVal;
		

		//final=vec4(ShadowVal)/10.0;

		//if(light_id!=floatBitsToUint(pass.debug.z)+8u){
		//if(light_type==0u){
			//final=vec4(0);
		//}
		return;
	}else if(floatBitsToUint(pass.debug.x)==6u){
		final=vec4(0,0.0,0,0);
	
		if(light_type==1u){
			final=vec4(0.1,0.0,0,0);
		}
		if(light_type==2u){
			final=vec4(0,0.1,0,0);
		}
		return;
	}else {
		final=vec4(0);
		return;
	}
@end
