#include <GL/glew.h>
#include <GL/gl.h>
#include "mesh.h"
#include <stdlib.h>

struct Mesh * init_Mesh() {
    struct Mesh * mesh = (struct Mesh*)malloc(sizeof(struct Mesh));
    mesh->indices_array = NULL;
    mesh->vertex_array = NULL;
    mesh->vertex_normals = NULL;
    return  mesh;
}

/*Create unit size cube*/
struct Mesh * create_cube_Mesh()
{
    struct Mesh * mesh = init_Mesh();
    GLfloat cube_vertices[] = {
        -1.0, -1.0,  1.0,
        1.0, -1.0,  1.0,
        1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,

        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,
        1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
    };
    glGenBuffers(1, &mesh->vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    GLushort cube_elements[] = {
        0, 1, 2,
        2, 3, 0,
        1, 5, 6,
        6, 2, 1,
        7, 6, 5,
        5, 4, 7,
        4, 0, 3,
        3, 7, 4,
        4, 5, 1,
        1, 0, 4,
        3, 2, 6,
        6, 7, 3,
    };
    glGenBuffers(1, &mesh->ibo_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
    return  mesh;
}


/*Create unit size sphere*/
struct Mesh * create_sphere_Mesh()
{
    int segments = 30;
    int stacks = 30;
    float phi = 0;
    float delta_phi = RADIANS(360)/segments;
    float delta_theta = RADIANS(180)/(stacks-1);
    float theta = RADIANS(90) - delta_theta;
    struct Mesh * mesh = (struct Mesh*)malloc(sizeof(struct Mesh));
    mesh->num_verticles = segments*(stacks-2) + 2;
    mesh->num_faces = 2*(stacks + 1)*segments;
    mesh->vertex_array = (struct vertex*)malloc(mesh->num_verticles*sizeof(struct vertex));
    mesh->indices_array = (struct Face*)malloc(mesh->num_faces*sizeof(struct Face));
    float r = 1;
    mesh->vertex_array[0].x = 0;
    mesh->vertex_array[0].y = 0;
    mesh->vertex_array[0].z = r;

    int j;

    for (j = 0; j < mesh->num_faces; j++)
    {
        mesh->indices_array[j].a = 0;
        mesh->indices_array[j].b = 0;
        mesh->indices_array[j].c = 0;
    }
    for (j = 0; j < stacks-2; j++) {
        int i;
        phi = 0;
        for (i = 0; i < segments; i++) {
            mesh->vertex_array[1 + j * segments + i].x = r*cos(phi)*cos(theta);
            mesh->vertex_array[1 + j * segments + i].y = r*sin(phi)*cos(theta);
            mesh->vertex_array[1 + j * segments + i].z = sin(theta);
            phi += delta_phi;
        }
        theta -= delta_theta;
    }
    mesh->vertex_array[mesh->num_verticles - 1].x = 0;
    mesh->vertex_array[mesh->num_verticles - 1].y = 0;
    mesh->vertex_array[mesh->num_verticles - 1].z = -r;

    //mesh->vertex_array[8].x = 0;
    //mesh->vertex_array[8].y = 0;
    //mesh->vertex_array[8].z = 0;
    for (j = 0; j < segments; j++)
    {
        mesh->indices_array[j].a = 0;
        if (j == segments - 1)
            mesh->indices_array[j].b = 1;
        else
            mesh->indices_array[j].b = (j+2);
        mesh->indices_array[j].c = (j+1);
    }
    int k = j;
    for (j = 0; j < stacks-3; j++) {
        int i;
        for (i = 1; i <= segments; i++) {
            mesh->indices_array[k].a = segments * j + i;
            mesh->indices_array[k].c = segments * j + i + segments;
            if (i == segments)
                mesh->indices_array[k].b = segments * j + i + 1;
            else
                mesh->indices_array[k].b = segments * j + i + segments + 1;
            k++;

            mesh->indices_array[k].a = segments * j + i;
            if (i == segments) {
                mesh->indices_array[k].b = (segments * j + 1);
                mesh->indices_array[k].c = (segments * j + i + 1);
            }
            else {
                mesh->indices_array[k].b = (segments * j + i + 1);
                mesh->indices_array[k].c = (segments * j + i + segments + 1);
            }
            k++;
        }
    }
    for (j = 0; j < segments; j++)
    {
        mesh->indices_array[k].a = mesh->num_verticles - 1;
        if (j == segments - 1)
            mesh->indices_array[k].b = mesh->num_verticles - segments - 1;
        else
            mesh->indices_array[k].b = mesh->num_verticles - segments + j;
        mesh->indices_array[k].c = mesh->num_verticles - segments + j - 1;
        k++;
    }



    glGenBuffers(1, &mesh->vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, mesh->num_verticles*sizeof(struct vertex), mesh->vertex_array, GL_STATIC_DRAW);
    glGenBuffers(1, &mesh->ibo_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->num_faces*sizeof(struct Face), mesh->indices_array, GL_STATIC_DRAW);
    return mesh;
}
