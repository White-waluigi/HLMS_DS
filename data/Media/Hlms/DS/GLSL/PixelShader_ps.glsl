//Datablock:	@insertpiece(db_name)
@piece(PI)3.14159625@end
#define PI 3.14159625
@property(IS_JSON)
	//	Json Material
@end

@property(!isForward)


@property(hlms_shadowcaster)
	//Shadow Material
@end

@property(isGBuffer)
	//Gbuffer Material
	@insertpiece(GBuffer_PS)
@end

@property(isLight)
		//Light Material 
	@property(LIGHTTYPE_Ambient)
		//AMBIENT
	@end
	@property(LIGHTTYPE_Direc)
		//DIRECTIONAL
	@end

	@property(!hlms_shadowcaster)
		@insertpiece(Light_PS)
	@end
@end

@end

@property(isForward)
	//Forward Material
	@insertpiece(Forward_PS)
@end

