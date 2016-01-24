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
    GLint program_id;
    GLint program_id_simple;
    GLint texture_id;
    GLint normal_texture_id;
    GLint depth_texture_id;
    GLint attribute_coord;
    GLint attribute_tangent;
    GLint texture_uniform;
    GLint uniform_model;
    GLint uniform_mvp;
    GLint uniform_texture;
    GLint uniform_normal_texture;
    GLint uniform_depth_texture;
    GLint uniform_light_pv;
    GLint uniform_light_power;
    float size;
};

struct Object * object_init();
struct Object * object_plane_init();
void render_plane_object(struct Object * object, Matrix4f mvp, GLuint texture_id);
void object_render(struct Object * object, Matrix4f mvp, Matrix4f model,
        Matrix4f light_pv,
        float light_power);
void object_render_simple(struct Object * object, Matrix4f mvp, Matrix4f model);
void object_render_line(struct Object * object, Matrix4f mvp, Matrix4f model);
void free_object(struct Object * object);

#endif
