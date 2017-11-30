#version 410
in vec2 fTexCoord;
out vec4 FragColour;
uniform sampler2D target1;
uniform sampler2D target2;
uniform float amount;
void main()
{
    FragColour = vec4(texture(target2, fTexCoord).xyz, 1);
    FragColour *= vec4(texture(target1, fTexCoord).xyz, 1);
}


 

