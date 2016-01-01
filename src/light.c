#include "light.h"
#include <stdlib.h>
#include <math.h>

int between(int middle, int left, int right) {
    if (left > middle || right <= middle)
        return 0;
    return 1;
}

struct Light * init_Light() {
    struct Light * light = malloc(sizeof(struct Light));
    light->hsv.hue = 64;
    light->hsv.saturation = 100;
    light->hsv.value = 60;
    return light;
}

void hsv_to_rgb(struct HSV *hsv, struct RGB *rgb) {
    int c = hsv->value * hsv->saturation;
    int x = c * (1 - abs((hsv->hue/60) % 2 - 1));
    int m = hsv->value - c;
    int r_p, g_p, b_p;
    if (between(hsv->hue, 0, 60)) {
        r_p = c;
        g_p = x;
        b_p = 0;
    }
    else if (between(hsv->hue, 60, 120)) {
        r_p = x;
        g_p = c;
        b_p = 0;
    }
    else if (between(hsv->hue, 120, 180)) {
        r_p = 0;
        g_p = c;
        b_p = x;
    }
    else if (between(hsv->hue, 180, 240)) {
        r_p = 0;
        g_p = x;
        b_p = c;
    }
    else if (between(hsv->hue, 240, 300)) {
        r_p = x;
        g_p = 0;
        b_p = c;
    }
    else if (between(hsv->hue, 300, 360)) {
        r_p = c;
        g_p = 0;
        b_p = x;
    }
    rgb->r = (r_p + m) * 255;
    rgb->g = (r_p + m) * 255;
    rgb->b = (r_p + m) * 255;
}
