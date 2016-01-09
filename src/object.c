#include <GL/glew.h>
#include "object.h"
#include "mesh.h"
#include <stdlib.h>

struct Object * object_init()
{
    struct Object * object = (struct Object*)malloc(sizeof(struct Object));
    object->mesh = NULL;
    object->program_id = 0;
    object->attribute_tangent = -1;
    object->texture_id = 0;
    object->texture_uniform = 0;
    object->uniform_mvp = 0;
    object->uniform_normal_texture = -1;
    object->normal_texture_id = 0;
    object->size = 1;
    return object;
}

void object_render(struct Object * object, Matrix4f mvp, Matrix4f model) {
    glUseProgram(object->program_id);
    glUniformMatrix4fv(object->uniform_model, 1 ,GL_FALSE, model);
    glUniformMatrix4fv(object->uniform_mvp, 1 ,GL_FALSE, mvp);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, object->texture_id);
    glUniform1i(object->uniform_texture, 0);
    if (object->normal_texture_id != -1) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, object->normal_texture_id);
        glUniform1i(object->uniform_normal_texture, 1);
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
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    if (object->attribute_coord)
        glDisableVertexAttribArray(object->attribute_coord);
    if (object->attribute_tangent)
        glDisableVertexAttribArray(object->attribute_tangent);

}
void object_render_line(struct Object * object, Matrix4f mvp, Matrix4f model) {
    glUseProgram(object->program_id);
    glUniformMatrix4fv(object->uniform_model, 1 ,GL_FALSE, model);
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
    glDrawArrays(GL_LINE_LOOP, 0, size/(3*sizeof(float)));
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
