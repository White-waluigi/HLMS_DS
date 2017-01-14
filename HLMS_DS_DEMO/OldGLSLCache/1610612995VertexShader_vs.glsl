

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


in vec4 vertex;
in vec4 colour;


in vec2 uv0;

in uint drawId;




out block
{

	
		flat uint drawId;
		vec4 colour;		
			vec2 uv0;			

} outVs;


// START UNIFORM DECLARATION

//Uniforms that change per pass
layout(binding = 0) uniform PassBuffer
{
	//Vertex shader
	mat4 viewProj[2];
		
} pass;


//Uniforms that change per Item/Entity
layout(binding = 2) uniform InstanceBuffer
{
	//.x =
	//Contains the material's start index.
	//
	//.y =
	//shadowConstantBias. Send the bias directly to avoid an
	//unnecessary indirection during the shadow mapping pass.
	//Must be loaded with uintBitsToFloat
	//
	//.z =
	//Contains 0 or 1 to index into pass.viewProj[]. Only used
	//if hlms_identity_viewproj_dynamic is set.
	uvec4 materialIdx[4096];
} instance;

layout(binding = 0) uniform samplerBuffer worldMatBuf;


// END UNIFORM DECLARATION


	


void main()
{
	
	
		mat4 worldViewProj;
		worldViewProj = UNPACK_MAT4( worldMatBuf, drawId );
	


	gl_Position = worldViewProj * vertex;





	outVs.colour = colour;




	
	outVs.uv0.xy = uv0.xy ;

	outVs.drawId = drawId;


	
}
