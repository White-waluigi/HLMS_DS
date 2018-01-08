#version 330
#extension GL_EXT_texture_array : enable
varying vec2 UV;
uniform sampler2D final;
uniform sampler2D depth;
in vec2 oUV0;

out vec4 fragColour;

void main(     )
{
	fragColour=vec4(texture(depth,oUV0));

}

