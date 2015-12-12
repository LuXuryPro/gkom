#version 130
varying vec3 textcoords;
uniform samplerCube cube_texture;

void main(void) {
    gl_FragColor = texture(cube_texture, textcoords);
}
