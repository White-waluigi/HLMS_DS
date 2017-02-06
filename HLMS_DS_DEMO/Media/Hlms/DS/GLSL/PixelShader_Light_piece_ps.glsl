
@piece(Light_PS)


#version 400 core
#extension GL_ARB_shading_language_420pack: require

@insertpiece(Helper)


@insertpiece(RAND)

@insertpiece(STRUCT)

@insertpiece(MaterialDecl)

@insertpiece( PassDecl )


@insertpiece(InstanceDecl)


		@property(hlms_num_shadow_maps)
			uniform sampler2D texShadowMap[@value(hlms_num_shadow_maps)];
		@end	
		uniform sampler2D GBuffer0;
		uniform sampler2D GBuffer1;
		uniform sampler2D GBuffer2;
		uniform sampler2D GBuffer3;
		uniform sampler2D GBuffer4;
	

in block
{
@insertpiece( VStoPS_block )
} inPs;

uint f2u(float f){
	return floatBitsToUint(f);
}
uint f2u(vec4 f){
	return floatBitsToUint(f)[0];
}

in vec4 vcolor;
out vec4 final;

@insertpiece(RAINBOW)

void main() {


	
	@property(!LIGHTTYPE_Ambient)
	@end

	@insertpiece(GBufferparams)

	
	
	@property(!LIGHTTYPE_Ambient)
		@insertpiece(LightParams)
		if(light_visible<=0){
			final=vec4(0);
			return;
		}
	@end
	

	@property(LIGHTTYPE_Ambient)
		@insertpiece(LightParamsAmbient)
	@end	


	@insertpiece(Fullbright)


	
	
	@insertpiece(LightCalc)



	@property(LIGHTTYPE_Ambient)
		@insertpiece(LightCalcAmbient)
	@end
	
	@property(LIGHTTYPE_Direc)
		@insertpiece(LightCalcDirectional)
	@end
	
	
	
	@property(LIGHTTYPE_Point)
		@insertpiece(LightCalcPoint)
	@end
	
	
	@property(LIGHTTYPE_Spot)
		@insertpiece(LightCalcSpot)
	@end


	@property(LIGHTTYPE_Spot || LIGHTTYPE_Direc)
	
		@property(hlms_num_shadow_maps)	
		if(light_shadows>0){
			@insertpiece(Shadow)
		}

		@end

	@end

		
		
	@property(!LIGHTTYPE_Ambient)
		@insertpiece(Debug)
	@end
	

	
	@property(LIGHTTYPE_Ambient)
		@insertpiece(DebugAmbient)
	@end
	

	
 }
 @end
