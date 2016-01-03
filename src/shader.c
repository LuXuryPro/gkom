#include <GL/glew.h>
#include "shader.h"
#include "file_util.h"
#include <stdlib.h>


GLuint compile_program(const char * vertex_shader_file_name, const char * fragment_shader_file_name)
{
    char * vertex_shader_source_code = read_text_file(vertex_shader_file_name);
    if (vertex_shader_file_name == NULL)
        return -1;
    GLint status;
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, (const GLchar * const*)&vertex_shader_source_code, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint log_lenght;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &log_lenght);
        char * vertex_shader_log = (char*)malloc(log_lenght + 1);
        glGetShaderInfoLog(vs, log_lenght, NULL, vertex_shader_log);
        free(vertex_shader_source_code);
        printf("In file: %s\n%s\n", vertex_shader_file_name, vertex_shader_log);
        exit(-1);
    }

    char * fragment_shader_source_code = read_text_file(fragment_shader_file_name);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, (const GLchar * const*)&fragment_shader_source_code, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint log_lenght;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &log_lenght);
        char * fragment_shader_log= (char*)malloc(log_lenght + 1);
        glGetShaderInfoLog(fs, log_lenght, NULL, fragment_shader_log);
        free(vertex_shader_source_code);
        free(fragment_shader_source_code);
        printf("In file: %s\n%s\n", fragment_shader_file_name, fragment_shader_log);
        exit(-1);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint log_lenght;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_lenght);
        char * link_log = (char*)malloc(log_lenght + 1);
        glGetProgramInfoLog(program, log_lenght, NULL, link_log);
        glDeleteProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);
        printf("In Linker\n%s\n", link_log);
        exit(-1);
    }

    free(vertex_shader_source_code);
    free(fragment_shader_source_code);
    glDetachShader(program, vs);
    glDetachShader(program, fs);
    return program;


}
