#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "image.h"

int clamp_padding(int boundary, int x) {
    return (x >= boundary) ? (boundary - 1) : ((x < 0) ? 0 : x);
}

float get_pixel(image im, int x, int y, int c) {
    x = clamp_padding(im.w, x);
    y = clamp_padding(im.h, y);
    c = clamp_padding(im.c, c);

    int dst_index = x + im.w * y + im.w * im.h * c;
    return im.data[dst_index];
}

void set_pixel(image im, int x, int y, int c, float v) {
    if (x < 0 || y < 0 || c < 0 || x >= im.w || y >= im.h || c >= im.c)
        return;

    int dst_index = x + im.w * y + im.w * im.h * c;
    im.data[dst_index] = v;
}

image copy_image(image im) {
    image copy = make_image(im.w, im.h, im.c);
    memcpy(copy.data, im.data, im.w * im.h * im.c * sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im) {
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    float r, g, b;

    // luma
    for (int j = 0; j < im.h; ++j) {
        for (int i = 0; i < im.w; ++i) {
            r = im.data[i + im.w * j + im.w * im.h * 0];
            g = im.data[i + im.w * j + im.w * im.h * 1];
            b = im.data[i + im.w * j + im.w * im.h * 2];
            gray.data[i + im.w * j] = 0.299 * r + 0.587 * g + 0.114 * b;
        }
    }
    return gray;
}

void shift_image(image im, int c, float v) {
    // TODO Fill this in
}

void clamp_image(image im) {
    // TODO Fill this in
}

// These might be handy
float three_way_max(float a, float b, float c) {
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

float three_way_min(float a, float b, float c) {
    return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

void rgb_to_hsv(image im) {
    // TODO Fill this in
}

void hsv_to_rgb(image im) {
    // TODO Fill this in
}
