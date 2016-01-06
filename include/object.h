/*
 * Object contains mesh and material info
 *
 * */
#ifndef OBJECT_H
#define OBJECT_H
#include "mesh.h"
#include "math3d.h"

struct Object {
    struct Mesh * mesh;
    GLuint program_id;
    GLuint texture_id;
    GLuint normal_texture_id;
    GLuint attribute_coord;
    GLuint texture_uniform;
    GLint uniform_model;
    GLint uniform_mvp;
    GLint uniform_texture;
    GLint uniform_normal_texture;
    float size;
};

struct Object * object_init();
void object_render(struct Object * object, Matrix4f mvp, Matrix4f model);
void object_render_line(struct Object * object, Matrix4f mvp, Matrix4f model);
void free_object(struct Object * object);

#endif
