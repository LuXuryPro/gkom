/*
 * Node contains object and its frame of reference.
 * Some object may contain child nodes.
 * */
#ifndef NODE_H
#define NODE_H
#include "object.h"
#include "math3d.h"

struct Node
{
    struct Object object;
    Matrix4f matrix;
    struct FrameOfReference frame_of_reference;
    int num_childs;
    struct Node * childs;
};

struct Node * node_init();
void node_load_identity(struct Node * node);
void node_translate(struct Node * node, struct Vector4f * vector);
void node_rotate(struct Node * node, struct Vector4f * vector, float angle);
void node_scale(struct Node * node, float scale);
void node_render(struct Node * node);

#endif
