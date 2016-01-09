#version 130
attribute vec3 coord;
attribute vec3 tangent;

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 light_space_matrix;

varying vec3 texCoords;
varying vec3 normal;
varying vec3 up;
varying vec3 tangent_v;
varying vec3 frag_coord;
varying vec4 light_space_frag_coord;

void main(void) {
  gl_Position = mvp * vec4(coord, 1.0f);
  frag_coord = vec3(model * vec4(coord, 1.0f));
  normal = normalize(vec3(model * vec4(coord, 0)));
  tangent_v = normalize(vec3(model * vec4(tangent, 0)));
  up = normalize(cross(normal, tangent_v));
  light_space_frag_coord = light_space_matrix * vec4(frag_coord, 1.0);
  texCoords = coord;
}

