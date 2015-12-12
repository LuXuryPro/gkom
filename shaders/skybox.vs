#version 130
attribute vec3 coord;
uniform mat4 mvp;
varying vec3 textcoords;

void main(void) {
  gl_Position = mvp * vec4(coord, 1.0);
  textcoords = coord;
}
