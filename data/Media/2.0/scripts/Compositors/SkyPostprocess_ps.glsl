#version 330
#extension GL_EXT_texture_array : enable
varying vec2 UV;
uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D specular;
uniform sampler2D glow;
//uniform sampler2D shaded;

in mat4 worldViewProjPS;
uniform mat4 worldViewProj;
in vec2 oUV0;
in block
{
    vec3 cameraDir;
} inPs;
out vec4 fragColour;


void main(     )
{
	vec2 UV0=vec2(oUV0.x,oUV0.y);
	vec4 ambient=vec4(.005,.005,0.01,.2);
	vec4 normalv=texture2D( normal,(UV0)).rgba;
	vec4 diffusev=texture2D( diffuse,(UV0)).rgba;
	vec4 specularv=texture2D( specular,(UV0)).rgba;
	
	vec4 glowv=texture2D( glow,(UV0)).rgba;
	
	vec4 h=vec4(1,1,1,1);
	
	vec3 total_light_contrib = max(0.0,dot(vec4(0.7,0.7,0.7,0), normalv)) *vec3(0.5,0.5,0.1);
	fragColour =vec4(sin(lenght(oUV0)),tan(lenght(oUV0)),cos(lenght(oUV0)),0);
	//fragColour=(ambient*diffusev)+glowv;
	
	

}

