/*
 * Node contains object and its frame of reference.
 * Some object may contain child nodes.
 * */
#ifndef NODE_H
#define NODE_H

struct Node
{
    struct FrameOfReference frame_of_reference;
    int num_childs;
    struct Node * childs;
};



#endif
