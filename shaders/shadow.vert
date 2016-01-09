#version 130
attribute vec3 coord;
uniform mat4 mvp;

void main(void) {
  gl_Position = mvp * vec4(coord, 1.0f);
}

