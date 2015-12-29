#ifndef SKYBOX_H
#define SKYBOX_H
#include <GL/gl.h>
#include <stdlib.h>
#include "mesh.h"

struct Skybox {
    GLuint textureID;
    struct Mesh * mesh;
    GLuint program;
    GLuint attribute_coord;
    GLuint mvp;
    GLuint texture_uniform;
};

struct Skybox * init_Skybox();
void render_Skybox(struct Skybox * skybox, Matrix4f pv);
#endif
