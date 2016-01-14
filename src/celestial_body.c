#include <GL/glew.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <SOIL/SOIL.h>
#include "shader.h"
#include "celestial_body.h"

struct Sun * sun_init() {
    struct Sun * sun = malloc(sizeof(struct Sun));
    sun->object = object_init();
    sun->object->size = 5;
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
    mat4f_scale(model, sun->object->size);
    Matrix4f rotation;
    struct Vector4f axis = {1,0,0,1};
    mat4f_rot(rotation, &axis, 90);
    mat4f_mul(rotation, model, model);
    axis.x = 0;
    axis.y = 1;
    axis.z = 0;
    mat4f_rot(rotation, &axis, frame/400);
    mat4f_mul(rotation, model, model);
    Matrix4f mvp;
    mat4f_mul(pv, model, mvp);
    object_render(sun->object, mvp, model, mvp);
}


struct Earth * earth_init() {
    struct Earth * earth = malloc(sizeof(struct Earth));
    earth->object = object_init();
    earth->object->size = 1;
    earth->object->program_id = compile_program("shaders/planet.vert", "shaders/planet.frag");
    earth->object->program_id_simple = compile_program("shaders/shadow.vert",
            "shaders/shadow.frag");
    earth->object->mesh = create_sphere_Mesh();
    earth->object->attribute_coord = glGetAttribLocation(earth->object->program_id, "coord");
    earth->object->uniform_mvp = glGetUniformLocation(earth->object->program_id, "mvp");
    earth->object->uniform_model = glGetUniformLocation(earth->object->program_id, "model");
    earth->object->uniform_texture = glGetUniformLocation(earth->object->program_id, "ourTexture");
    earth->object->uniform_normal_texture = glGetUniformLocation(earth->object->program_id, "normal_map");
    earth->object->uniform_depth_texture = glGetUniformLocation(earth->object->program_id, "shadowMap");
    earth->object->uniform_light_pv = glGetUniformLocation(earth->object->program_id, "light_space_matrix");

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

    glGenTextures(1, &earth->object->normal_texture_id);
    glBindTexture(GL_TEXTURE_2D, earth->object->normal_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    image = SOIL_load_image("res/earth_normal.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    earth->object->attribute_tangent = glGetAttribLocation(earth->object->program_id, "tangent");

    earth->orbit = object_init();
    earth->orbit->size = 1;
    earth->orbit->program_id = compile_program("shaders/orbit.vert", "shaders/orbit.frag");
    earth->orbit->mesh = create_ring_Mesh();
    earth->orbit->attribute_coord = glGetAttribLocation(earth->orbit->program_id, "coord");
    earth->orbit->uniform_mvp = glGetUniformLocation(earth->orbit->program_id, "mvp");

    return earth;
}

void earth_render(struct Earth* earth, Matrix4f pv, float frame,
        struct Moon * moon,
        int simple, Matrix4f light_pv) {
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
    translation_vector.x = 20*cos(frame);
    translation_vector.y = 0;
    translation_vector.z = 20*sin(frame);
    mat4f_translate(orbit, &translation_vector);
    mat4f_mul(rotation, model, model);
    mat4f_mul(orbit, model, model);
    Matrix4f mvp;
    mat4f_mul(pv, model, mvp);
    if (simple) {
        object_render_simple(earth->object, mvp, model);
        moon_render(moon, pv, frame, &translation_vector, 1, light_pv);
        return;
    }
    object_render(earth->object, mvp, model, light_pv);

    mat4f_init_identity(model);
    axis.x = 1;
    axis.y = 0;
    axis.z = 0;
    mat4f_rot(model, &axis, 90);
    Matrix4f scale;
    mat4f_scale(scale, 20);
    mat4f_mul(scale, model, model);

    mat4f_mul(pv, model, mvp);
    object_render_line(earth->orbit, mvp, model);
    moon_render(moon, pv, frame, &translation_vector, 0, light_pv);
}

struct Moon * moon_init() {
    struct Moon * moon = malloc(sizeof(struct Earth));
    moon->object = object_init();
    moon->object->program_id = compile_program("shaders/planet.vert", "shaders/planet.frag");
    moon->object->program_id_simple = compile_program("shaders/shadow.vert",
            "shaders/shadow.frag");
    moon->object->mesh = create_sphere_Mesh();
    moon->object->attribute_coord = glGetAttribLocation(moon->object->program_id, "coord");
    moon->object->uniform_mvp = glGetUniformLocation(moon->object->program_id, "mvp");
    moon->object->uniform_model = glGetUniformLocation(moon->object->program_id, "model");
    moon->object->uniform_texture = glGetUniformLocation(moon->object->program_id, "ourTexture");
    moon->object->uniform_normal_texture = glGetUniformLocation(moon->object->program_id, "normal_map");
    moon->object->uniform_depth_texture = glGetUniformLocation(moon->object->program_id, "shadowMap");
    moon->object->uniform_light_pv = glGetUniformLocation(moon->object->program_id, "light_space_matrix");


    glGenTextures(1, &moon->object->texture_id);
    glBindTexture(GL_TEXTURE_2D, moon->object->texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    int width, height;
    unsigned char* image = SOIL_load_image("res/moon.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &moon->object->normal_texture_id);
    glBindTexture(GL_TEXTURE_2D, moon->object->normal_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    image = SOIL_load_image("res/moon_normal.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    moon->object->attribute_tangent = glGetAttribLocation(moon->object->program_id, "tangent");

    moon->orbit = object_init();
    moon->orbit->size = 1;
    moon->orbit->program_id = compile_program("shaders/orbit.vert", "shaders/orbit.frag");
    moon->orbit->mesh = create_ring_Mesh();
    moon->orbit->attribute_coord = glGetAttribLocation(moon->orbit->program_id, "coord");
    moon->orbit->uniform_mvp = glGetUniformLocation(moon->orbit->program_id, "mvp");
    return moon;
}

void moon_render(struct Moon * moon, Matrix4f pv, float frame,
        struct Vector4f * parent_pos, int simple, Matrix4f light_pv) {
    Matrix4f model;
    Matrix4f scale;
    mat4f_scale(scale, 0.1);
    mat4f_init_identity(model);
    mat4f_mul(model, scale, model);
    struct Vector4f axis = {1,0,0,1};
    Matrix4f rotation;
    mat4f_rot(rotation, &axis, 90);
    mat4f_mul(rotation, model, model);
    axis.x = 0;
    axis.y = 1;
    axis.z = 0;
    mat4f_rot(rotation, &axis, frame*10000);
    mat4f_mul(rotation, model, model);
    Matrix4f orbit;
    struct Vector4f translation_vector;
    translation_vector.x = 5*cos(frame*100);
    translation_vector.y = 0;
    translation_vector.z = 5*sin(frame*100);
    mat4f_translate(orbit, &translation_vector);
    mat4f_mul(orbit, model, model);
    Matrix4f parent;
    mat4f_translate(parent, parent_pos);
    mat4f_mul(parent, model, model);
    Matrix4f mvp;
    mat4f_mul(pv, model, mvp);
    if (simple) {
        object_render_simple(moon->object, mvp, model);
        return;
    }
    object_render(moon->object, mvp, model, light_pv);

    mat4f_init_identity(model);
    axis.x = 1;
    axis.y = 0;
    axis.z = 0;
    mat4f_rot(model, &axis, 90);
    mat4f_scale(scale, 5);
    mat4f_mul(scale, model, model);
    mat4f_mul(parent, model, model);

    mat4f_mul(pv, model, mvp);
    object_render_line(moon->orbit, mvp, model);
}
