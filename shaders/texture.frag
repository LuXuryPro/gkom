#version 130
uniform sampler2D ourTexture;
varying vec2 texCoords;

float near_plane = 1;
float far_plane = 20;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main(void) {
    float depth = texture(ourTexture, texCoords).r;
    gl_FragColor = vec4(vec3(LinearizeDepth(depth)/far_plane), 1.0);
}
