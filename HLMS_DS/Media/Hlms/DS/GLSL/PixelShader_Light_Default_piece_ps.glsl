//Default Lighting
@piece(GBufferparams)

	vec2 screenPos=vec2((gl_FragCoord.x/pass.screenx),(gl_FragCoord.y/pass.screeny));

	vec2 texCoord=vec2(screenPos.x,screenPos.y);
	

		
	vec3 diffuse=texture2D(GBuffer0 ,texCoord).rgb;
	float depth=texture2D(GBuffer1 ,texCoord).a;
	vec3 normal=texture2D(GBuffer1 ,texCoord).rgb;
	vec3 specular=texture2D(GBuffer2 ,texCoord).rgb;
	float rough=texture2D(GBuffer2 ,texCoord).w;
	
	
	
	vec3 glow=texture2D(GBuffer3 ,texCoord).rgb;
	
	float Sdepth=texture2D(GBuffer4 ,texCoord).x;
	float SSR=texture2D(GBuffer4 ,texCoord).y;
@end


@piece(LightParams)

	uint light_type					=floatBitsToUint(material.vec4_lightparams.x);

	uint light_id					=floatBitsToUint(material.vec4_lightparams.z);

	float light_power				=material.vec4_lightparams.y;
	
	float light_visible				=material.vec4_lightsettings.x;
	
	float light_static				=material.vec4_lightsettings.y;
	
	float light_shadows				=material.vec4_lightsettings.z;
	
	vec4 light_position				=material.vec4_position;
	
	vec4 light_diffuse				=material.vec4_diffuse;
	
	vec4 light_specular				=material.vec4_specular;

	vec4 light_attenuation			=material.vec4_attenuation;
	
	vec4 light_spotDirection		=pass.View*material.vec4_spotdirection;
	
	vec4 light_spotParams			=material.vec4_spotparams;
	
	vec4 ShadowVal=vec4(1);

@end


@piece(LightParamsAmbient)
	uint light_type					=floatBitsToUint(material.vec4_lightparams.x);

	float light_power				=material.vec4_lightparams.y;	
	
	vec4 light_diffuse				=material.vec4_diffuse;
@end


@piece(LightCalcAmbient)

	//final.rgb=(light_diffuse.rgb*diffuse)+glow;
	
@end


@piece(LightCalc)
	//vec2 projPos=(vec2((gl_FragCoord.x/pass.screenx),(gl_FragCoord.y/pass.screeny) )-0.5)/0.5;
	
	vec2 projPos=(vec2(texCoord.x,texCoord.y) - 0.5) / 0.5;
	
	vec3 ray = vec3(projPos.x,projPos.y, 1)*pass.farCorners.xyz;
	vec3 viewPos = normalize(ray)*(depth*pass.farClip);
 
 	vec3 worldPos = (pass.InvView* vec4(viewPos, 1)).xyz;
 
 
   	float f=pass.farClip;
	float n = pass.nearClip;
   	
	vec3 objToLightVec =vec3(-1);
	vec3 total_light_contrib=vec3(-1);


@end


@piece(LightCalcDirectional)

	// Calculate diffuse colour
	//Light Position is Direction for Directional Lights
	total_light_contrib = max(0.0,dot(-light_position.xyz, normal)) * light_diffuse.rgb*diffuse;

	vec3 viewDir = -normalize(viewPos);
	vec3 h = normalize(viewDir + -light_position.xyz);
	vec3 final_specular = pow(dot(normal, h),rough) * light_specular.rgb;
	total_light_contrib += specular * final_specular;

@end


@piece(LightCalcPoint)
	objToLightVec =(vec4(light_position.xyz,1)).xyz-viewPos;
	float len_sq = dot(objToLightVec, objToLightVec);
	float len = sqrt(len_sq);
	vec3 objToLightDir = objToLightVec/len;
		
	// Calculate diffuse colour
	total_light_contrib = max(0.0,dot(objToLightDir, normal)) * light_diffuse.rgb*diffuse;


	vec4 rim=vec4(0);
	

	
	// Calculate specular component
	vec3 viewDir = -normalize(viewPos);
	vec3 h = normalize(viewDir + objToLightDir);
	vec3 final_specular = pow(dot(normal, h),rough) * light_specular.rgb;
	total_light_contrib += specular * final_specular;

    //LT_DIRECTIONAL = 0,
    //LT_POINT = 1,
    // LT_SPOTLIGHT = 2,

	float attenuation = dot(light_attenuation.yzw, vec3(1.0, len, len_sq));
	total_light_contrib /= attenuation;
	
	vec2 test =(vec4(light_position.xyz,1)).xy-viewPos.xy;
	float len2_sq = dot(objToLightVec, objToLightVec);
	float len2 = sqrt(len_sq);
	vec2 objToLightDir2 = test/len2;
	//final=vec4(0) ;
	//if(len<15.0){
	//	final=vec4( pow( (15.0-len)/15.0,2 ))*pass.flip;	
	//}
	


@end


@piece(LightCalcSpot)

	@insertpiece(LightCalcPoint)
	float spotlightAngle = clamp(dot(         ( vec4(light_spotDirection.xyz,0) ).xyz        , -objToLightDir), 0.0, 1.0);
	float spotFalloff = clamp((spotlightAngle - light_spotParams.x) / (light_spotParams.y - light_spotParams.x), 0.0, 1.0);
	total_light_contrib *= (1-spotFalloff);
			
@end
