#version 410
in vec4 worldNormal;
in vec4 worldPosition;
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
out vec4 fragColor;
uniform int lightsNum;
struct PointLight {
    vec3 position;
    float radius;
    vec3 colour;
}


;
struct DirectionalLight
{
    vec3 dir;
    vec3 colour;
}


;
uniform PointLight lights[10];
uniform DirectionalLight dirLight;
uniform float slider2;
uniform sampler2D diffuseMap;
uniform sampler2D specMap;
uniform sampler2D normMap;
uniform sampler2D glossMap;
uniform vec4 cameraPosition;
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 dirToCamera)
{
    vec3 surfaceToLight = light.position - worldPosition.xyz;
    float diff =max(dot(normal, normalize(surfaceToLight.xyz)), 0);
    vec3 refl = reflect(-normalize(surfaceToLight), normal);
    //lower = more chromatic, higher = sharper
	float specPow = 1;
    float specular1 = pow(max(dot(refl, dirToCamera), 0), specPow);
    float dist = length(surfaceToLight);
    float att = clamp(1.0 - dist*dist/(light.radius*light.radius), 0.0, 1.0);
    att *= att;
    vec3 diffuse = diff * vec3(texture(diffuseMap, vTexCoord));
    vec3 specular = specular1 * vec3(texture(specMap, vTexCoord));
    diffuse *= att;
    specular *= att;
    return((diffuse + specular) * light.colour);
}


vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 dirToCamera)
{
    vec3 lightDir = normalize(-light.dir);
    float diff = max(dot(normal, lightDir), 0);
    vec3 refl = reflect(-lightDir, normal);
    //lower = more chromatic, higher = sharper
	float specPow = 10;
    float specular1 = pow(max(dot(dirToCamera, refl), 0), specPow);
    vec3 diffuse = diff * vec3(texture(diffuseMap, vTexCoord));
    vec3 specular = specular1 * vec3(texture(specMap, vTexCoord));
    return((diffuse + specular) * light.colour);
}
void main()
{
    mat3 TBN = mat3(normalize(vTangent), normalize(vBiTangent), normalize(worldNormal));
    vec4 N = texture(normMap, vTexCoord);
    N = normalize(N * 2.0 - 1.0);
    //normals based off of normalmap
	vec3 basiclyNormal= normalize(TBN * N.xyz);
    vec3 toCamera = normalize(cameraPosition.xyz - worldPosition.xyz);
    vec3 result = CalcDirectionalLight(dirLight, basiclyNormal, toCamera);
    for(int i = 0; i < lightsNum; i++)
	{
        result += CalcPointLight(lights[i], basiclyNormal, toCamera);
    }
	//fragColor = vec4(lightsNum,0,0,1);
    fragColor = vec4(result + texture(glossMap, vTexCoord).rgb * 2, 1);
    // fragColor = vec4(specularmap.rgb * specular + texture(diffuseMap, vTexCoord).rgb * brightness + texture(glossMap, vTexCoord).xyz * 3, 1);
	// fragColor = vec4(lights[0].radius, 0, 0, 1);
    // fragColor = vec4(basiclyNormal.xyz, 1 );
} 