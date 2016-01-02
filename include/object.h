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
    GLuint attribute_coord;
    GLuint texture_uniform;
    GLuint uniform_model;
    GLuint uniform_mvp;
    GLuint uniform_texture;
};

struct Object * object_init();
void object_render(struct Object * object, Matrix4f mvp, Matrix4f model);
void free_object(struct Object * object);

#endif
