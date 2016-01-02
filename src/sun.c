#include <GL/glew.h>
#include <GL/gl.h>
#include "sun.h"
#include <stdlib.h>
#include <SOIL/SOIL.h>
#include "shader.h"


struct Sun * sun_init() {
    struct Sun * sun = malloc(sizeof(struct Sun));
    sun->object = object_init();
    sun->object->program_id = compile_program("shaders/sun.vert", "shaders/sun.frag");
    sun->object->mesh = create_sphere_Mesh();
    sun->object->attribute_coord = glGetAttribLocation(sun->object->program_id, "coord");
    sun->object->uniform_mvp = glGetUniformLocation(sun->object->program_id, "mvp");
    sun->object->uniform_model = glGetUniformLocation(sun->object->program_id, "model");
    sun->object->uniform_texture = glGetUniformLocation(sun->object->program_id, "ourTexture");
    glGenTextures(1, &sun->object->texture_id);
    glBindTexture(GL_TEXTURE_2D, sun->object->texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    int width, height;
    unsigned char* image = SOIL_load_image("res/sun.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    return sun;
}

void sun_render(struct Sun * sun, Matrix4f pv, float frame) {
    Matrix4f model;
    mat4f_init_identity(model);
    struct Vector4f axis = {1,0,0,1};
    mat4f_rot(model, &axis, 90);
    axis.x = 0;
    axis.y = 1;
    axis.z = 0;
    Matrix4f rotation;
    mat4f_rot(rotation, &axis, frame/5);
    mat4f_mul(rotation, model, model);
    Matrix4f mvp;
    mat4f_mul(pv, model, mvp);
    object_render(sun->object, mvp, model);
}
