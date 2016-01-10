#include "depth_map.h"
#include <stdlib.h>

struct DepthMap * depth_map_init() {
    struct DepthMap * depth_map = malloc(sizeof(struct DepthMap));
    depth_map->shadow_map_width = depth_map->shadow_map_height = 2048;
    glGenFramebuffers(1, &depth_map->depthMapFBO);
    glGenTextures(1, &depth_map->depthMap);
    glBindTexture(GL_TEXTURE_2D, depth_map->depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            depth_map->shadow_map_width, depth_map->shadow_map_height, 0,
            GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map->depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
            depth_map->depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return depth_map;
}

void depth_map_bind_for_write(struct DepthMap * depth_map) {
    glViewport(0, 0, depth_map->shadow_map_width,
            depth_map->shadow_map_height);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map->depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
}
