#include <GL/glew.h>
#include <GL/gl.h>
#include "earth.h"
#include <stdlib.h>
#include <SOIL/SOIL.h>
#include "shader.h"


struct Earth * earth_init() {
    struct Earth * earth = malloc(sizeof(struct Earth));
    earth->object = object_init();
    earth->object->program_id = compile_program("shaders/planet.vert", "shaders/planet.frag");
    earth->object->mesh = create_sphere_Mesh();
    earth->object->attribute_coord = glGetAttribLocation(earth->object->program_id, "coord");
    earth->object->uniform_mvp = glGetUniformLocation(earth->object->program_id, "mvp");
    earth->object->uniform_model = glGetUniformLocation(earth->object->program_id, "model");
    earth->object->uniform_texture = glGetUniformLocation(earth->object->program_id, "ourTexture");
    glGenTextures(1, &earth->object->texture_id);
    glBindTexture(GL_TEXTURE_2D, earth->object->texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    int width, height;
    unsigned char* image = SOIL_load_image("res/earth.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    return earth;
}

void earth_render(struct Earth* earth, Matrix4f pv, float frame) {
    Matrix4f model;
    mat4f_init_identity(model);
    struct Vector4f axis = {1,0,0,1};
    mat4f_rot(model, &axis, 90);
    axis.x = 0;
    axis.y = 1;
    axis.z = 0;
    Matrix4f rotation;
    mat4f_rot(rotation, &axis, frame*365.0);
    Matrix4f orbit;
    struct Vector4f translation_vector;
    translation_vector.x = 10*cos(frame);
    translation_vector.y = 0;
    translation_vector.z = 10*sin(frame);
    mat4f_translate(orbit, &translation_vector);
    mat4f_mul(rotation, model, model);
    mat4f_mul(orbit, model, model);
    Matrix4f mvp;
    mat4f_mul(pv, model, mvp);
    object_render(earth->object, mvp, model);
}
