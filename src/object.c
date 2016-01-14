#include <GL/glew.h>
#include "object.h"
#include "mesh.h"
#include <stdlib.h>
#include "shader.h"

struct Object * object_init()
{
    struct Object * object = (struct Object*)malloc(sizeof(struct Object));
    object->mesh = NULL;
    object->program_id = 0;
    object->attribute_tangent = -1;
    object->texture_id = 0;
    object->depth_texture_id = -1;
    object->texture_uniform = 0;
    object->uniform_mvp = 0;
    object->uniform_normal_texture = -1;
    object->normal_texture_id = 0;
    object->size = 1;
    return object;
}

struct Object * object_plane_init() {
    struct Object * object = object_init();
    object->program_id = compile_program("shaders/texture.vert", "shaders/texture.frag");
    object->mesh = create_plane_mesh();
    object->attribute_coord = glGetAttribLocation(object->program_id, "coord");
    object->uniform_mvp = glGetUniformLocation(object->program_id, "mvp");
    object->uniform_texture = glGetUniformLocation(object->program_id, "ourTexture");
    return object;
}

void render_plane_object(struct Object * object, Matrix4f mvp, GLuint texture_id) {
    glEnable(GL_TEXTURE_2D);
    glUseProgram(object->program_id);
    glUniformMatrix4fv(object->uniform_mvp, 1 ,GL_FALSE, mvp);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glUniform1i(object->uniform_texture, 0);
    if (object->attribute_coord != -1) {
        glEnableVertexAttribArray(object->attribute_coord);
        glBindBuffer(GL_ARRAY_BUFFER, object->mesh->vbo_vertices);
        glVertexAttribPointer(
                object->attribute_coord, // attribute
                3,                 // number of elements per vertex, here (x,y)
                GL_FLOAT,          // the type of each element
                GL_FALSE,          // take our values as-is
                0,                 // no extra data between each position
                0 // pointer to the C array
                );
    }
    int size;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawArrays(object->mesh->mode, 0, size/(3*sizeof(float)));
    glDisableVertexAttribArray(object->attribute_coord);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void object_render_simple(struct Object * object, Matrix4f mvp, Matrix4f model) {
    glUseProgram(object->program_id_simple);
    GLuint attribute_coord = glGetAttribLocation(object->program_id_simple, "coord");
    GLuint uniform_mvp = glGetUniformLocation(object->program_id_simple, "mvp");
    glUniformMatrix4fv(uniform_mvp, 1 ,GL_FALSE, mvp);
    glEnableVertexAttribArray(attribute_coord);
    glBindBuffer(GL_ARRAY_BUFFER, object->mesh->vbo_vertices);
    glVertexAttribPointer(
            attribute_coord, // attribute
            3,                 // number of elements per vertex, here (x,y)
            GL_FLOAT,          // the type of each element
            GL_FALSE,          // take our values as-is
            0,                 // no extra data between each position
            0 // pointer to the C array
            );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->mesh->ibo_elements);
    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(object->mesh->mode, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    glDisableVertexAttribArray(attribute_coord);
}

void object_render(struct Object * object, Matrix4f mvp, Matrix4f model,
        Matrix4f light_pv) {
    glEnable(GL_TEXTURE_2D);
    glUseProgram(object->program_id);
    glUniformMatrix4fv(object->uniform_model, 1 ,GL_FALSE, model);
    glUniformMatrix4fv(object->uniform_mvp, 1 ,GL_FALSE, mvp);
    glUniformMatrix4fv(object->uniform_light_pv, 1 ,GL_FALSE, light_pv);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, object->texture_id);
    glUniform1i(object->uniform_texture, 0);
    if (object->normal_texture_id != -1) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, object->normal_texture_id);
        glUniform1i(object->uniform_normal_texture, 1);
    }
    if (object->depth_texture_id != -1) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, object->depth_texture_id);
        glUniform1i(object->uniform_depth_texture, 2);
    }
    if (object->attribute_coord != -1) {
        glEnableVertexAttribArray(object->attribute_coord);
        glBindBuffer(GL_ARRAY_BUFFER, object->mesh->vbo_vertices);
        glVertexAttribPointer(
                object->attribute_coord, // attribute
                3,                 // number of elements per vertex, here (x,y)
                GL_FLOAT,          // the type of each element
                GL_FALSE,          // take our values as-is
                0,                 // no extra data between each position
                0 // pointer to the C array
                );
    }
    if (object->attribute_tangent != -1) {
        glEnableVertexAttribArray(object->attribute_tangent);
        glBindBuffer(GL_ARRAY_BUFFER, object->mesh->vbo_tangents);
        glVertexAttribPointer(
                object->attribute_tangent, // attribute
                3,                 // number of elements per vertex, here (x,y)
                GL_FLOAT,          // the type of each element
                GL_FALSE,          // take our values as-is
                0,                 // no extra data between each position
                0 // pointer to the C array
                );
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->mesh->ibo_elements);
    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(object->mesh->mode, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    if (object->attribute_coord != -1)
        glDisableVertexAttribArray(object->attribute_coord);
    if (object->attribute_tangent != -1)
        glDisableVertexAttribArray(object->attribute_tangent);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

}
void object_render_line(struct Object * object, Matrix4f mvp, Matrix4f model) {
    glUseProgram(object->program_id);
    glUniformMatrix4fv(object->uniform_mvp, 1 ,GL_FALSE, mvp);
    glEnableVertexAttribArray(object->attribute_coord);
    glBindBuffer(GL_ARRAY_BUFFER, object->mesh->vbo_vertices);
    glVertexAttribPointer(
            object->attribute_coord, // attribute
            3,                 // number of elements per vertex, here (x,y)
            GL_FLOAT,          // the type of each element
            GL_FALSE,          // take our values as-is
            0,                 // no extra data between each position
            0 // pointer to the C array
            );
    int size;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawArrays(object->mesh->mode, 0, size/(3*sizeof(float)));
    glDisableVertexAttribArray(object->attribute_coord);
}

void free_object(struct Object * object)
{
    if (object->mesh != NULL)
        free_Mesh(object->mesh);
    if (object->program_id != 0)
        glDeleteProgram(object->program_id);
    if (object->texture_id)
        glDeleteTextures(1, &object->texture_id);
}
