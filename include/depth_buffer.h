#ifndef DEPTH_BUFFER_H
#define DEPTH_BUFFER_H
#include <GL/glew.h>
#include <GL/gl.h>


struct DepthMap {
    GLuint depthMapFBO;
    int shadow_map_width;
    int shadow_map_height;
    GLuint depthMap;
};


struct DepthMap * depth_map_init();
void depth_map_bind_for_write(struct DepthMap * depth_map);

#endif
