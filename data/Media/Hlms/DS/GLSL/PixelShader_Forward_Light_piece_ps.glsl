@piece(emptyVals)
int shadowRes_offs=0;
int shadowRes_size=0;
	
int shadowMat_offs=0;
int shadowMat_size=0;
			
int shadowDr_offs=0;
int shadowDr_size=0;

int shadowPssmSplits_offs=0;
int shadowPssmSplits_size=0;
@end
@piece(Forward_Lighting)
	color=vec4(0);
		uint size=f2u(texelFetch( fLightList,0));
		int poffs=1;
		vec3 worldPos=inPs.worldPos.xyz;
		vec3 opacity_specular=vec3(0);
		for(int i=0;i<size;i++)
		{
			vec3 objToLightVec =vec3(0);
			vec3 total_light_contrib=vec3(0);
		
			
			
			@insertpiece(ForwardLightParams);
			
			if(f2u(texelFetch(fLightList,poffs+6))==0u){
				@insertpiece(LightCalcDirectional)
			}
			if(f2u(texelFetch(fLightList,poffs+6))==1u){
		
				@insertpiece(LightCalcPoint)				
			}
			if(f2u(texelFetch(fLightList,poffs+6))==2u){
				@insertpiece(LightCalcSpot)
			}
			if(f2u(texelFetch(fLightList,poffs+6))==4u){
				@insertpiece(LightCalcAmbient)
				
			}

	
			color.xyz+=(ominf(  vec4((total_light_contrib), 0.0))*ShadowVal).xyz;
			poffs+=64;
			
		}
@end
@piece(ForwardLightParams)
    
    
    
    
    
    //0 position;
    //1 diffuse;
    //2 specular;
    //3 attenuation;
    //4 spotdirection;
    //5 spotparams;
    //6 lightparams;
    //7 lightsettings;
    //8 shadowParams;
    //9 shadowQualityParams;
    

	
	vec4 light_position				=texelFetch(fLightList,poffs+0);
	
	vec4 light_diffuse				=texelFetch(fLightList,poffs+1);
	
	vec4 light_specular				=texelFetch(fLightList,poffs+2);

	vec4 light_attenuation			=texelFetch(fLightList,poffs+3);
	
	vec4 light_spotDirection		=pass.View*texelFetch(fLightList,poffs+4);
	
	vec4 light_spotParams			=texelFetch(fLightList,poffs+5);
	
	uint light_type					=floatBitsToUint(texelFetch(fLightList,poffs+6).x);

	uint light_id					=floatBitsToUint(texelFetch(fLightList,poffs+6).z);

	float light_power				=texelFetch(fLightList,poffs+6).y;
	
	float light_visible				=texelFetch(fLightList,poffs+7).x;
	
	float light_static				=texelFetch(fLightList,poffs+7).y;
	
	float light_shadows				=texelFetch(fLightList,poffs+7).z;
	
	vec4 light_shadowParams			=texelFetch(fLightList,poffs+8);
	
	vec4 light_shadowQualityParams	=texelFetch(fLightList,poffs+9);

	
	bool check=true;
	
	shadowRes_offs=poffs+10+1;
	shadowRes_size=qis(fLightList,shadowRes_offs-1).x;
	if(texelFetch(fLightList,shadowRes_offs-1).y!=1234)
		check=false;
	
	
	shadowMat_offs=shadowRes_offs+shadowRes_size+1;
	shadowMat_size=qis(fLightList,shadowMat_offs-1).x;
	if(texelFetch(fLightList,shadowMat_offs-1).y!=1234)
			check=false;
		
			
	shadowDr_offs=shadowMat_offs+shadowMat_size+1;
	shadowDr_size=qis(fLightList,shadowDr_offs-1).x;
	if(texelFetch(fLightList,shadowDr_offs-1).y!=1234)
		check=false;
		
		
	shadowPssmSplits_offs=shadowDr_offs+shadowDr_size+1;
	shadowPssmSplits_size=qis(fLightList,shadowPssmSplits_offs-1).x;
	if(texelFetch(fLightList,shadowPssmSplits_offs-1).y!=1234)
		check=false;
	
	
	if(!check)
	{
		//color=vec4(1,0,0,0);
		//return;
	}
	
	vec4 ShadowVal=vec4(1);

@end
