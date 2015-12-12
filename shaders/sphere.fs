#version 120
varying vec3 c_f;

void main(void) {
    gl_FragColor = vec4(c_f.x + 0.5, c_f.y + 0.5, c_f.z + 0.5 , 1.0);
}
