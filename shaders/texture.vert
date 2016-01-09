#version 130
attribute vec3 coord;
uniform mat4 mvp;
varying vec2 texCoords;

void main(void) {
  gl_Position = mvp * vec4(coord, 1.0f);
  texCoords = coord.xy*0.5+ 0.5;
}
