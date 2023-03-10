#version 330

in vec4 vertex;
in vec3 normal;
uniform mat4 worldViewProj;
out   mat4 worldViewProjPS;
attribute vec2 uv0;
varying vec2 oUV0;


out gl_PerVertex
{
    vec4 gl_Position;
};

out block
{
    vec3 cameraDir;
} outVs;

void main()
{
    gl_Position = (  worldViewProj * vertex).xyww;
    outVs.cameraDir.xyz = normal.xyz;
    oUV0 = uv0;
    worldViewProjPS=worldViewProj;
}
