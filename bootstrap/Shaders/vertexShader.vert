#version 410
layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;
layout(location=2) in vec2 texCoord;
layout(location=3) in vec4 tangent;
out vec4 worldNormal;
out vec4 worldPosition;
out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vTangent; 
out vec3 vBiTangent;
uniform vec4 cameraPosition;
uniform mat4 MVP;
uniform mat4 M;
void main()
{
vTexCoord = texCoord;
vNormal = normal.xyz;
vTangent = normalize(M * vec4(tangent.xyz,0)).xyz; 
worldNormal = normalize( M * vec4(normal.xyz, 0));
vBiTangent = cross(worldNormal.xyz, vTangent);
worldPosition =  M * position;
gl_Position = MVP * position;
}