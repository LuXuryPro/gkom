/*
 * Mesh contains verticles, indices, normals, uv_coords and vertex colors
 * */
#ifndef MESH_H
#define MESH_H
#include "math3d.h"
#include <GL/gl.h>

struct Face {
    GLushort a,b,c;
};

struct vertex {
    float x,y,z;
};

struct  UV {
    float u,v;
};

struct Mesh {
    struct vertex * vertex_array;
    struct vertex * vertex_normals;
    struct Face * indices_array;
    GLuint vbo_vertices,
           vbo_colors ,
           ibo_elements,
           vbo_text_coords;
    unsigned int num_verticles;
    unsigned int num_faces;
};

struct Mesh * create_cube_Mesh();
struct Mesh * create_sphere_Mesh();

void free_Mesh(struct Mesh * mesh);

#endif
