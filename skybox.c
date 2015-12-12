#include <GL/glew.h>
#include "skybox.h"
#include <SOIL/SOIL.h>
#include "shader.h"
#include "math3d.h"

struct Skybox * init_Skybox()
{
    struct Skybox * skybox = (struct Skybox*)malloc(sizeof(struct Skybox));
    skybox->mesh = create_cube_Mesh();
    skybox->program = compile_program("shaders/skybox.vs", "shaders/skybox.fs");
    skybox->attribute_coord = glGetAttribLocation(skybox->program, "coord");
    skybox->mvp = glGetUniformLocation(skybox->program, "mvp");
    skybox->texture_uniform = glGetUniformLocation(skybox->program, "cube_texture");
    glGenTextures(1, &skybox->textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->textureID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    const char * textures[] = {
        "res/skybox/sky_pos_x.tga",
        "res/skybox/sky_neg_x.tga",
        "res/skybox/sky_pos_y.tga",
        "res/skybox/sky_neg_y.tga",
        "res/skybox/sky_pos_z.tga",
        "res/skybox/sky_neg_z.tga",
    };

    int i;
    int width, height, channels;

    for (i = 0; i < 6; i++)
    {
        unsigned char* image = SOIL_load_image(textures[i], &width, &height, &channels, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
            GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }

    return  skybox;
}


void free_skybox(struct Skybox * skybox)
{
    glDeleteTextures(1, &skybox->textureID);
    free(skybox);
}

void render_Skybox(struct Skybox * skybox, Matrix4f pv)
{
    glDepthMask (GL_FALSE);
    Matrix4f mvp;
    Matrix4f model = {0};
    model[0] = 100;
    model[5] = 100;
    model[10] = 100;
    model[15] = 1;
    mat4f_mul(pv, model, mvp);
    glUseProgram(skybox->program);
    glActiveTexture (GL_TEXTURE0);
    glUniform1i(skybox->texture_uniform, 0);
    glBindTexture (GL_TEXTURE_CUBE_MAP, skybox->textureID);
    glUniformMatrix4fv(skybox->mvp, 1 ,GL_FALSE, mvp);
    glEnableVertexAttribArray(skybox->attribute_coord);
    glBindBuffer(GL_ARRAY_BUFFER, skybox->mesh->vbo_vertices);
    glVertexAttribPointer(
            skybox->attribute_coord, // attribute
            3,                 // number of elements per vertex, here (x,y)
            GL_FLOAT,          // the type of each element
            GL_FALSE,          // take our values as-is
            0,                 // no extra data between each position
            0 // pointer to the C array
            );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox->mesh->ibo_elements);
    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(skybox->attribute_coord);
    glDepthMask (GL_TRUE);

}

void load_Skybox()
{}
