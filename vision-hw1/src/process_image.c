#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "image.h"

float TOLERANCE = 0.0001f;
int clamp_padding(int boundary, int x) {
    return (x >= boundary) ? (boundary - 1) : ((x < 0) ? 0 : x);
}

int index_pixel(image im, int x, int y, int c) {
    return x + im.w * y + im.w * im.h * c;
}

float get_pixel(image im, int x, int y, int c) {
    x = clamp_padding(im.w, x);
    y = clamp_padding(im.h, y);
    c = clamp_padding(im.c, c);

    return im.data[index_pixel(im, x, y, c)];
}

void set_pixel(image im, int x, int y, int c, float v) {
    x = clamp_padding(im.w, x);
    y = clamp_padding(im.h, y);
    c = clamp_padding(im.c, c);

    im.data[index_pixel(im, x, y, c)] = v;
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
            r = get_pixel(im, i, j, 0);
            g = get_pixel(im, i, j, 1);
            b = get_pixel(im, i, j, 2);
            set_pixel(gray, i, j, 0, 0.299 * r + 0.587 * g + 0.114 * b);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v) {
    for (int j = 0; j < im.h; ++j) {
        for (int i = 0; i < im.w; ++i) {
            set_pixel(im, i, j, c, get_pixel(im, i, j, c) + v);
        }
    }
}

void scale_image(image im, int c, float v) {
    for (int j = 0; j < im.h; ++j) {
        for (int i = 0; i < im.w; ++i) {
            set_pixel(im, i, j, c, get_pixel(im, i, j, c) * v);
        }
    }
}

void clamp_image(image im) {
    for (int i = 0; i < im.w * im.h * im.c; ++i) {
        if (im.data[i] < 0.0f) {
            im.data[i] = 0.0f;
        } else if (im.data[i] > 1.0f) {
            im.data[i] = 1.0f;
        }
    }
}

// These might be handy
float three_way_max(float a, float b, float c) {
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

float three_way_min(float a, float b, float c) {
    return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

void rgb_to_hsv(image im) {
    float h, s, v, r, g, b, c;

    for (int j = 0; j < im.h; ++j) {
        for (int i = 0; i < im.w; ++i) {
            r = get_pixel(im, i, j, 0);
            g = get_pixel(im, i, j, 1);
            b = get_pixel(im, i, j, 2);

            v = three_way_max(r, g, b);
            c = v - three_way_min(r, g, b);
            s = (v != 0) ? (c / v) : 0;

            if (c != 0.0f) {
                if (fabsf(v - r) < TOLERANCE) {
                    h = (g - b) / (c);
                } else if (fabsf(v - g) < TOLERANCE) {
                    h = (b - r) / (c) + 2;
                } else if (fabsf(v - b) < TOLERANCE) {
                    h = (r - g) / (c) + 4;
                }
                h = (h < 0) ? (h / 6 + 1) : (h / 6);
            } else {
                s = 0;
                h = 0;
            }

            set_pixel(im, i, j, 0, h);
            set_pixel(im, i, j, 1, s);
            set_pixel(im, i, j, 2, v);
        }
    }
}

void hsv_to_rgb(image im) {
    float h, s, v, c, m, x;
    for (int j = 0; j < im.h; ++j) {
        for (int i = 0; i < im.w; ++i) {
            h = get_pixel(im, i, j, 0) * 6;
            s = get_pixel(im, i, j, 1);
            v = get_pixel(im, i, j, 2);

            c = v * s;
            m = v - c;
            x = c * (1 - fabs(fmod(h, 2) - 1));

            float r = 0, g = 0, b = 0;
            if (h < 1) {
                r = c;
                g = x;
            } else if (h < 2) {
                r = x;
                g = c;
            } else if (h < 3) {
                g = c;
                b = x;
            } else if (h < 4) {
                g = x;
                b = c;
            } else if (h < 5) {
                r = x;
                b = c;
            } else if (h < 6) {
                r = c;
                b = x;
            }

            set_pixel(im, i, j, 0, r + m);
            set_pixel(im, i, j, 1, g + m);
            set_pixel(im, i, j, 2, b + m);
        }
    }
}
