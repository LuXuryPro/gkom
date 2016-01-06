#version 120
attribute vec3 coord;
uniform mat4 mvp;
uniform mat4 model;

void main(void) {
  gl_Position = mvp * vec4(coord, 1.0);
}
