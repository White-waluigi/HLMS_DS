@piece(PBSSample)
	float fDepth = psPosLN.z;
	vec2 uv = psPosLN.xy / psPosLN.w;

	float retVal = 0;


	vec2 offsets[@value(pcf_iterations)] =
	{
	
		vec2( 0, 0 ),	//0, 0
		vec2( 1, 0 ),	//1, 0
		vec2( 0, 1 ),	//1, 1
		vec2( 0, 0 ) 	//1, 1
		};

	float row[2];
	row[0] = 0;
	row[1] = 0;


	vec2 fW;
	vec4 c;

	@foreach( pcf_iterations, n )

		@property( !hlms_shadow_uses_depth_texture )
		// 2x2 PCF
		//The 0.00196 is a magic number that prevents floating point
		//precision problems ("1000" becomes "999.999" causing fW to
		//be 0.999 instead of 0, hence ugly pixel-sized dot artifacts
		//appear at the edge of the shadow).
		fW = fract( uv * invShadowMapSize.zw + 0.00196 );

		@property( !hlms_tex_gather )
			c.w = texture(shadowMap, uv ).r;
			c.z = texture(shadowMap, uv + vec2( invShadowMapSize.x, 0.0 ) ).r;
			c.x = texture(shadowMap, uv + vec2( 0.0, invShadowMapSize.y ) ).r;
			c.y = texture(shadowMap, uv + vec2( invShadowMapSize.x, invShadowMapSize.y ) ).r;
		@end @property( hlms_tex_gather )
			c = textureGather( shadowMap, uv + invShadowMapSize.xy * 0.5 );
		@end

		c = step( fDepth, c );

		@property( !pcf_3x3 && !pcf_4x4 )
			//2x2 PCF: It's slightly faster to calculate this directly.
			retVal += mix(
						mix( c.w, c.z, fW.x ),
						mix( c.x, c.y, fW.x ),
						fW.y );
		@end @property( pcf_3x3 || pcf_4x4 )
			row[0] += mix( c.w, c.z, fW.x );
			row[1] += mix( c.x, c.y, fW.x );
		@end
		@end @property( hlms_shadow_uses_depth_texture )
			retVal += texture( shadowMap, vec3( uv, fDepth ) ).r;
		@end
	@end

	@property( (pcf_3x3 || pcf_4x4) && !hlms_shadow_uses_depth_texture )
		//NxN PCF: It's much faster to leave the final mix out of the loop (when N > 2).
		retVal = mix( row[0], row[1], fW.y );
	@end

	@property( pcf_3x3 )
		retVal *= 0.25;
	@end @property( pcf_4x4 )
		retVal *= 0.11111111111111;
	@end

	return retVal;
@end