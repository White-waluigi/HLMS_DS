

#version 330 core
#extension GL_ARB_shading_language_420pack: require


layout(std140) uniform;
#define FRAG_COLOR		0


layout(location = FRAG_COLOR, index = 0) out vec4 outColour;


// START UNIFORM DECLARATION


struct Material
{
	vec4 alpha_test_threshold;
	vec4 diffuse;

	uvec4 indices0_3;
	uvec4 indices4_7;
};

layout(binding = 1) uniform MaterialBuf
{
	Material m[1024];
} materialArray;


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



// END UNIFORM DECLARATION
in block
{

	
		flat uint drawId;
		vec4 colour;		
			vec2 uv0;			

} inPs;



uniform sampler2D	textureMaps[1];



Material material;

void main()
{
	

	uint materialId	= instance.materialIdx[inPs.drawId].x;
	material = materialArray.m[materialId];

	




	//Load base image
	outColour = texture( textureMaps[0], inPs.uv0.xy ).xxxy;





	outColour *= inPs.colour ;
	


	



	
}


