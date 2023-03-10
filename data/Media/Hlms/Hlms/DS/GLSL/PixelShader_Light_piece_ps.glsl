
@piece(Light_PS)


#version 400 core
#extension GL_ARB_shading_language_420pack: require


@insertpiece(RAND)

@insertpiece(STRUCT)

@insertpiece(MaterialDecl)

@insertpiece( PassDecl )


@insertpiece(InstanceDecl)
//layout(binding = 4) uniform indexBuffer
//{
//	uvec4 colour; //kD.w is alpha_test_threshold
//	uvec4 viewProj0;
//	uvec4 viewProj1;
//	uvec4 viewProj2;
//	uvec4 viewProj3;
	
//} test;
//uniform samplerBuffer worldMatBuf;

//layout(binding = 2) uniform InstanceBuffer
//{
    //.x =
	//The lower 9 bits contain the material's start index.
    //The higher 23 bits contain the world matrix start index.
    //
    //.y =
    //shadowConstantBias. Send the bias directly to avoid an
    //unnecessary indirection during the shadow mapping pass.
    //Must be loaded with uintBitsToFloat
    //uvec4 worldMaterialIdx[4096];
//} instance;

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


	@insertpiece(GBufferparams)

	@property(!LIGHTTYPE_Ambient)
		@insertpiece(LightParams)
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
			@insertpiece(Shadow)

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