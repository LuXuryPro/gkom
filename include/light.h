#ifndef LIGHT_H
#define LIGHT_H


struct RGB {
    char r,g,b;
};

struct HSV {
    int hue;
    int saturation;
    int value;
};

struct Light {
    struct HSV hsv;
};

struct Light * init_Light();
void hsv_to_rgb(struct HSV * hsv, struct RGB * rgb);

#endif
