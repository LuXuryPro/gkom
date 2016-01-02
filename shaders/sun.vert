#version 130
attribute vec3 coord;

uniform mat4 mvp;
uniform mat4 model;

varying vec3 texCoords;
varying vec3 normal;
varying vec3 frag_coord;

void main(void) {
  gl_Position = mvp * vec4(coord, 1.0f);
  frag_coord = vec3(model * vec4(coord, 1.0f));
  normal = vec3(model * vec4(coord, 0));
  texCoords = coord;
}
