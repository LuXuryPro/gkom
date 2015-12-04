#ifndef MESH_H
#define MESH_H
#include "math3d.h"

struct Face {
    int a,b,c;
};


struct Mesh {
    struct Vector4f * vertex_array;
    struct Vector4f * vertex_normals;
    struct Face * indices_array;
    struct Vector4f * face_normals;
};

#endif
