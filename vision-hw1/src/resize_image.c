#include <math.h>
#include <stdio.h>

#include "image.h"

float nn_interpolate(image im, float x, float y, int c) {
    return get_pixel(im, round(x), round(y), c);
}

image nn_resize(image im, int w, int h) {
    image im_resize = make_image(w, h, im.c);
    float w_factor = im.w / (float)w;
    float h_factor = im.h / (float)h;

    for (int k = 0; k < im.c; ++k) {
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                set_pixel(im_resize, i, j, k,
                          nn_interpolate(im, i * w_factor + 0.5 * (w_factor - 1),
                                         j * h_factor + 0.5 * (h_factor - 1), k));
            }
        }
    }
    return im_resize;
}

float bilinear_interpolate(image im, float x, float y, int c) {
    int x1 = floor(x);
    int x2 = x1 + 1;
    int y1 = floor(y);
    int y2 = y1 + 1;
    float v1 = get_pixel(im, x1, y1, c);
    float v2 = get_pixel(im, x2, y1, c);
    float v3 = get_pixel(im, x1, y2, c);
    float v4 = get_pixel(im, x2, y2, c);
    float q1 = v1 * (x2 - x) + v2 * (x - x1);
    float q2 = v3 * (x2 - x) + v4 * (x - x1);
    float q = q1 * (y2 - y) + q2 * (y - y1);
    return q;
}

image bilinear_resize(image im, int w, int h) {
    image im_resize = make_image(w, h, im.c);
    float w_factor = im.w / (float)w;
    float h_factor = im.h / (float)h;
    for (int k = 0; k < im.c; ++k) {
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                set_pixel(im_resize, i, j, k,
                          bilinear_interpolate(im, i * w_factor + 0.5 * (w_factor - 1),
                                               j * h_factor + 0.5 * (h_factor - 1), k));
            }
        }
    }
    return im_resize;
}
