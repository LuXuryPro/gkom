#ifndef OBJECT_H
#define OBJECT_H
#include "mesh.h"

struct Object {
    struct Mesh * mesh;
    GLuint program;
};


#endif
