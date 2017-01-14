#version 330 core
#extension GL_ARB_shading_language_420pack: require

out gl_PerVertex
{
	vec4 gl_Position;
};

layout(std140) uniform;


mat4 UNPACK_MAT4( samplerBuffer matrixBuf, uint pixelIdx )
{
        vec4 row0 = texelFetch( matrixBuf, int((pixelIdx) << 2u) );
        vec4 row1 = texelFetch( matrixBuf, int(((pixelIdx) << 2u) + 1u) );
        vec4 row2 = texelFetch( matrixBuf, int(((pixelIdx) << 2u) + 2u) );
        vec4 row3 = texelFetch( matrixBuf, int(((pixelIdx) << 2u) + 3u) );
    return mat4( row0.x, row1.x, row2.x, row3.x,
                 row0.y, row1.y, row2.y, row3.y,
                 row0.z, row1.z, row2.z, row3.z,
                 row0.w, row1.w, row2.w, row3.w );
}


mat4x3 UNPACK_MAT4x3( samplerBuffer matrixBuf, uint pixelIdx )
{
        vec4 row0 = texelFetch( matrixBuf, int((pixelIdx) << 2u) );
        vec4 row1 = texelFetch( matrixBuf, int(((pixelIdx) << 2u) + 1u) );
        vec4 row2 = texelFetch( matrixBuf, int(((pixelIdx) << 2u) + 2u) );
        return mat4x3( row0.x, row1.x, row2.x,
                       row0.y, row1.y, row2.y,
                       row0.z, row1.z, row2.z,
                       row0.w, row1.w, row2.w );
}


in vec4 vertex;

in vec4 qtangent;





    

in vec2 uv0; 
in uint drawId;



out block
{

        flat uint drawId;
		vec4 pos;
		vec3 normal;
		vec3 vertex;
		vec4 position;
		vec3 tangent;
		vec4 worldPos;
		vec4 glPosition;
		float depth;
		
			flat float biNormalReflection;
				
			
		vec2 uv0;		
				

} outVs;

// START UNIFORM DECLARATION

struct ShadowData{
		vec4 size;
		vec4 lightID;
		vec4 depthrange;
		mat4 ShadowCamMat;
};  


layout(binding = 0) uniform PassBuffer
{
	vec4 debug;
	
	mat4 Proj;
	
	mat4 View;
	
	mat4 InvView;
	
	vec4 time;
	
		
	float nearClip;
	float farClip;
	float screenx;
	float screeny;
	
	vec4 farCorners;
	
	vec4 flip;
	
	float near;
	float far;
	float Invdepthrange;
	float depthrange;
	
	vec4 test;
	

	
} pass;


layout(binding = 0) uniform samplerBuffer worldMatBuf;





vec3 xAxis( vec4 qQuat )
{
	float fTy  = 2.0 * qQuat.y;
	float fTz  = 2.0 * qQuat.z;
	float fTwy = fTy * qQuat.w;
	float fTwz = fTz * qQuat.w;
	float fTxy = fTy * qQuat.x;
	float fTxz = fTz * qQuat.x;
	float fTyy = fTy * qQuat.y;
	float fTzz = fTz * qQuat.z;

	return vec3( 1.0-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy );
}


vec3 yAxis( vec4 qQuat )
{
	float fTx  = 2.0 * qQuat.x;
	float fTy  = 2.0 * qQuat.y;
	float fTz  = 2.0 * qQuat.z;
	float fTwx = fTx * qQuat.w;
	float fTwz = fTz * qQuat.w;
	float fTxx = fTx * qQuat.x;
	float fTxy = fTy * qQuat.x;
	float fTyz = fTz * qQuat.y;
	float fTzz = fTz * qQuat.z;

	return vec3( fTxy-fTwz, 1.0-(fTxx+fTzz), fTyz+fTwx );
}

 


//layout(binding = 0) uniform instanceBuffer
//{
//	vec4 colour;

	//mat4 view;
	//mat4 model;
	//mat4 proj;

//} instance;


//layout(binding = 4) uniform indexBuffer
//{
//	uvec4 colour; //kD.w is alpha_test_threshold
//	uvec4 viewProj0;
//	uvec4 viewProj1;
//	uvec4 viewProj2;
//	uvec4 viewProj3;
	
//} test;



out vec4 vcolor;
void main()
{

	outVs.drawId=drawId; 
	mat4 final;
//	final[0]=uintBitsToFloat(test.viewProj0);
//	final[1]=uintBitsToFloat(test.viewProj1);
	//final[2]=uintBitsToFloat(test.viewProj2);
	//final[3]=uintBitsToFloat(test.viewProj3);
	
	mat4 testm=mat4(	1.358,0,0,0,
						0, 2.41421, 0, 0,
						0, 0, -1.0004, -0.40008,
						0, 0, -1, 0 );
	
	// mat4x3 worldMat = UNPACK_MAT4x3( worldMatBuf, drawId << 1u);
	// mat4x3 worldMat = UNPACK_MAT4x3( worldMatBuf, drawId<< 1u );
    //mat4 worldView = UNPACK_MAT4( worldMatBuf, (drawId << 1u) + 1u );
    //vec4 worldPos = vec4( (worldView*vertex) );



    mat4 worldMat = UNPACK_MAT4( worldMatBuf, drawId<<1);
	
    mat4 worldView = UNPACK_MAT4( worldMatBuf, (drawId<<1) + 1u );
	
	vec4	worldPos = vec4( (worldView*vertex) );
	//worldPos = vec4( (worldMat * vertex).xyz, 1.0f );
	
  

 
 
 	worldPos.xyz =  vec4( (worldMat * vertex) ).xyz;
 	worldPos.w =  1.0;
 	
  
 	
	outVs.worldPos= worldPos;


	

		
	
	vcolor =vec4(0.5,float(drawId)/100.0,mod(float(drawId)/10.0,1.0),0);
	vcolor=worldPos;
	vcolor=worldPos;
	
	


    	//vcolor.xyz =vec3(0.5,1,0);
    	
    
    //gl_Position =final*vertex;
   // gl_Position = pass.viewProj*worldPos;
    

	
		outVs.uv0 = uv0;    
    
    outVs.vertex=vertex.xyz;

	//Decode qTangent to TBN with reflection
	vec3 normal		= xAxis( normalize( qtangent ) );
	vec3 tangent	= yAxis( qtangent );
	outVs.biNormalReflection = sign( qtangent.w ); //We ensure in C++ qtangent.w is never 0
		
		
	//Lighting is in view space
	//outVs.pos		= (worldView * vertex).xyz;

  
  outVs.tangent	= mat3(worldView) * tangent;
  

	outVs.pos		=pass.View*worldPos;
    outVs.normal	= mat3(worldView) * normal;

    gl_Position = pass.Proj *(outVs.pos);

    //outVs.glPosition=gl_Position;
	//outVs.glPosition.x = (length(outVs.pos.xyz))*pass.depthrange.x;
	outVs.glPosition =gl_Position;
	//outVs.depth =gl_Position.z*pass.Invdepthrange;
	
            	  vcolor=vertex;
    
    

		
	    



		
    }
    
