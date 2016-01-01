#version 130
attribute vec3 coord;
attribute vec2 text_coords;

uniform mat4 mvp;
varying vec3 texCoords;

void main(void) {
  gl_Position = mvp * vec4(coord, 1.0);
  texCoords = coord;
}
