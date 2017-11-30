#version 410
layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;
layout(location=2) in vec2 texCoord;
layout(location=3) in vec4 tangent;
layout(location=4) in vec4 weights;
layout(location=5) in vec4 indices;

out vec4 worldNormal;
out vec4 worldPosition;
out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vTangent; 
out vec3 vBiTangent;
uniform vec4 cameraPosition;
uniform mat4 MVP;
uniform mat4 M;
const int MAX_BONES = 128;
uniform mat4 bones[MAX_BONES];
void main()
{
ivec4 index = ivec4(indices);
vec4 P = bones[ index.x ] * position * weights.x;
P += bones[ index.y ] * position * weights.y;
P += bones[ index.z ] * position * weights.z;
P += bones[ index.w ] * position * weights.w;

vec4 norm = vec4(normal.xyz, 0);
vec4 N = bones[ index.x ] * norm * weights.x;
N += bones[ index.y ] * norm * weights.y;
N += bones[ index.z ] * norm * weights.z;
N += bones[ index.w ] * norm * weights.w;

vTexCoord = texCoord;
vNormal = normal.xyz;
vTangent = normalize(M * vec4(tangent.xyz,0)).xyz; 
worldNormal = normalize( M * N );
vBiTangent = cross(worldNormal.xyz, vTangent);
worldPosition =  M * P;
gl_Position = MVP * P;
}