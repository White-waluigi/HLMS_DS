#version 330

in vec4 vertex;
in vec3 normal;
uniform mat4 worldViewProj;
uniform vec4 texture_size;


out   mat4 worldViewProjPS;
attribute vec2 uv0;
varying vec2 oUV0;

varying vec2 v_rgbNW;
varying vec2 v_rgbNE;
varying vec2 v_rgbSW;
varying vec2 v_rgbSE;
varying vec2 v_rgbM;
varying vec2 vUv;
varying vec2 iResolution;


out gl_PerVertex
{
    vec4 gl_Position;
};

out block
{
    vec3 cameraDir;
} outVs;
void texcoords(vec2 fragCoord, vec2 resolution,
			out vec2 v_rgbNW, out vec2 v_rgbNE,
			out vec2 v_rgbSW, out vec2 v_rgbSE,
			out vec2 v_rgbM) {
	vec2 inverseVP = 1.0 / resolution.xy;
	v_rgbNW = (fragCoord + vec2(-1.0, -1.0)) * inverseVP;
	v_rgbNE = (fragCoord + vec2(1.0, -1.0)) * inverseVP;
	v_rgbSW = (fragCoord + vec2(-1.0, 1.0)) * inverseVP;
	v_rgbSE = (fragCoord + vec2(1.0, 1.0)) * inverseVP;
	v_rgbM = vec2(fragCoord * inverseVP);
}
void main()
{
	
    gl_Position = (  worldViewProj * vertex).xyww;
    outVs.cameraDir.xyz = normal.xyz;
    oUV0 = uv0;
    worldViewProjPS=worldViewProj;
    
    
    iResolution=vec2(texture_size.xy);
   	vUv = (gl_Position.xy*vec2(1.0,-1.0) + 1.0) * 0.5;
   	vUv.y = vUv.y;
  	vec2 fragCoord = vUv * iResolution;
  	 texcoords(fragCoord, iResolution, v_rgbNW, v_rgbNE, v_rgbSW, v_rgbSE, v_rgbM);
}
