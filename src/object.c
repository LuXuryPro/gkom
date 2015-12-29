#include <GL/glew.h>
#include "object.h"
#include "mesh.h"
#include <stdlib.h>

struct Object * init_Object()
{
    struct Object * object = (struct Object*)malloc(sizeof(struct Object));
    object->mesh = NULL;
    object->program_id = 0;
    object->texture_id = 0;
    return object;
}

void free_Object(struct Object * object)
{
    if (object->mesh != NULL)
        free_Mesh(object->mesh);
    if (object->program_id != 0)
        glDeleteProgram(object->program_id);
    if (object->texture_id)
        glDeleteTextures(1, &object->texture_id);
}
