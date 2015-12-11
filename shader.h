#ifndef SHADER_H
#define SHADER_H
#include <GL/gl.h>

/*Pass vertex shader filename and fragment shader file name to create a compiled
 * glProgram object. If compilation fails it will exit program calling exit(-1)*/
GLuint compile_program(const char * vertex_shader_file_name, const char * fragment_shader_file_name);






#endif
