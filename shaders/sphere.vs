#version 120
attribute vec3 coord;
uniform mat4 mvp;
varying vec3 c_f;

void main(void) {
  gl_Position = mvp * vec4(coord, 1.0);
  c_f = coord;
}
