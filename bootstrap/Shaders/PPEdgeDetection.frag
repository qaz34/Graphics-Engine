#version 410
in vec2 fTexCoord;
out vec4 FragColour;
uniform sampler2D target;
uniform float amount;
mat3 sx = mat3( 
    1.0, 2.0, 1.0, 
    0.0, 0.0, 0.0, 
   -1.0, -2.0, -1.0 
);
mat3 sy = mat3( 
    1.0, 0.0, -1.0, 
    2.0, 0.0, -2.0, 
    1.0, 0.0, -1.0 
);
void main()
{
    vec3 diffuse = texture(target, fTexCoord.st).rgb;
    mat3 I;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            vec3 sample1  = texelFetch(target, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0 ).rgb;
            I[i][j] = length(sample1); 
    }
}

float gx = dot(sx[0], I[0]) + dot(sx[1], I[1]) + dot(sx[2], I[2]); 
float gy = dot(sy[0], I[0]) + dot(sy[1], I[1]) + dot(sy[2], I[2]);

float g = sqrt(pow(gx, 2.0)+pow(gy, 2.0));
//g = smoothstep(0.4, 0.6, g);
FragColour = vec4(diffuse + vec3(-g)/ amount, 1.0);
} 

