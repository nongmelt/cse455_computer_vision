#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im) {
    float total = 0;
    for (int k = 0; k < im.c; ++k) {
        for (int j = 0; j < im.h; ++j) {
            for (int i = 0; i < im.w; ++i) {
                total += get_pixel(im, i, j, k);
            }
        }
    }
    for (int k = 0; k < im.c; ++k) {
        for (int j = 0; j < im.h; ++j) {
            for (int i = 0; i < im.w; ++i) {
                set_pixel(im, i, j, k, get_pixel(im, i, j, k) / total);
            }
        }
    }
}

image make_box_filter(int w) {
    image filter = make_image(w, w, 1);
    for (int j = 0; j < w; ++j) {
        for (int i = 0; i < w; ++i) {
            set_pixel(filter, i, j, 0, 1.0f);
        }
    }
    l1_normalize(filter);
    return filter;
}

image convolve_image(image im, image filter, int preserve) {
    assert(filter.c == im.c || filter.c == 1);

    image filtered_im = make_image(im.w, im.h, (preserve == 1) ? (im.c) : 1);

    for (int k = 0; k < im.c; ++k) {
        for (int j = 0; j < im.h; ++j) {
            for (int i = 0; i < im.w; ++i) {
                float q = 0;
                for (int y = 0; y < filter.h; ++y) {
                    for (int x = 0; x < filter.w; ++x) {
                        q += get_pixel(filter, x, y, (filter.c == 1) ? (0) : (k)) *
                             get_pixel(im, i - floor(filter.w / 2) + x, j - filter.h / 2 + y, k);
                    }
                }
                if (preserve == 1) {
                    set_pixel(filtered_im, i, j, k, q);
                } else {
                    set_pixel(filtered_im, i, j, 0, get_pixel(filtered_im, i, j, 0) + q);
                }
            }
        }
    }

    return filtered_im;
}

image make_highpass_filter() {
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, 0.f);
    set_pixel(filter, 0, 1, 0, -1.f);
    set_pixel(filter, 0, 2, 0, 0.f);
    set_pixel(filter, 1, 0, 0, -1.f);
    set_pixel(filter, 1, 1, 0, 4.f);
    set_pixel(filter, 1, 2, 0, -1.f);
    set_pixel(filter, 2, 0, 0, 0.f);
    set_pixel(filter, 2, 1, 0, -1.f);
    set_pixel(filter, 2, 2, 0, 0.f);
    return filter;
}

image make_sharpen_filter() {
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, 0.f);
    set_pixel(filter, 0, 1, 0, -1.f);
    set_pixel(filter, 0, 2, 0, 0.f);
    set_pixel(filter, 1, 0, 0, -1.f);
    set_pixel(filter, 1, 1, 0, 5.f);
    set_pixel(filter, 1, 2, 0, -1.f);
    set_pixel(filter, 2, 0, 0, 0.f);
    set_pixel(filter, 2, 1, 0, -1.f);
    set_pixel(filter, 2, 2, 0, 0.f);
    return filter;
}

image make_emboss_filter() {
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -2.f);
    set_pixel(filter, 0, 1, 0, -1.f);
    set_pixel(filter, 0, 2, 0, 0.f);
    set_pixel(filter, 1, 0, 0, -1.f);
    set_pixel(filter, 1, 1, 0, 1.f);
    set_pixel(filter, 1, 2, 0, 1.f);
    set_pixel(filter, 2, 0, 0, 0.f);
    set_pixel(filter, 2, 1, 0, 1.f);
    set_pixel(filter, 2, 2, 0, 2.f);
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: Highpass, Emboss

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: Highpass

image make_gaussian_filter(float sigma) {
    int kernel_size = round(sigma * 6) + 1;
    image filter = make_image(kernel_size, kernel_size, 1);
    float coeffcient = 1 / (TWOPI * sigma * sigma);
    for (int j = 0; j < filter.h; ++j) {
        for (int i = 0; i < filter.w; ++i) {
            int x = i - floor(filter.w / 2);
            int y = j - floor(filter.h / 2);
            float p = (x * x + y * y) / (2 * sigma * sigma);
            set_pixel(filter, i, j, 0, coeffcient * exp(-p));
        }
    }
    return filter;
}

image add_image(image a, image b) {
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image added_im = make_image(a.w, a.h, a.c);
    for (int k = 0; k < a.c; ++k) {
        for (int j = 0; j < a.h; ++j) {
            for (int i = 0; i < a.w; ++i) {
                set_pixel(added_im, i, j, k, get_pixel(a, i, j, k) + get_pixel(b, i, j, k));
            }
        }
    }
    return added_im;
}

image sub_image(image a, image b) {
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image sub_im = make_image(a.w, a.h, a.c);
    for (int k = 0; k < a.c; ++k) {
        for (int j = 0; j < a.h; ++j) {
            for (int i = 0; i < a.w; ++i) {
                set_pixel(sub_im, i, j, k, get_pixel(a, i, j, k) - get_pixel(b, i, j, k));
            }
        }
    }
    return sub_im;
}

image make_gx_filter() {
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -1.f);
    set_pixel(filter, 0, 1, 0, 0.f);
    set_pixel(filter, 0, 2, 0, 1.f);
    set_pixel(filter, 1, 0, 0, -2.f);
    set_pixel(filter, 1, 1, 0, 0.f);
    set_pixel(filter, 1, 2, 0, 2.f);
    set_pixel(filter, 2, 0, 0, -1.f);
    set_pixel(filter, 2, 1, 0, 0.f);
    set_pixel(filter, 2, 2, 0, 1.f);
    return filter;
}

image make_gy_filter() {
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -1.f);
    set_pixel(filter, 0, 1, 0, -2.f);
    set_pixel(filter, 0, 2, 0, -1.f);
    set_pixel(filter, 1, 0, 0, 0.f);
    set_pixel(filter, 1, 1, 0, 0.f);
    set_pixel(filter, 1, 2, 0, 0.f);
    set_pixel(filter, 2, 0, 0, 1.f);
    set_pixel(filter, 2, 1, 0, 2.f);
    set_pixel(filter, 2, 2, 0, 1.f);
    return filter;
}

void feature_normalize(image im) {
    float minimum = get_pixel(im, 0, 0, 0);
    float maximum = get_pixel(im, 0, 0, 0);
    for (int k = 0; k < im.c; ++k) {
        for (int j = 0; j < im.h; ++j) {
            for (int i = 0; i < im.w; ++i) {
                float v = get_pixel(im, i, j, k);
                minimum = (minimum <= v) ? minimum : v;
                maximum = (maximum >= v) ? maximum : v;
            }
        }
    }
    float range = maximum - minimum;
    for (int k = 0; k < im.c; ++k) {
        for (int j = 0; j < im.h; ++j) {
            for (int i = 0; i < im.w; ++i) {
                set_pixel(im, i, j, k, (range == 0.f) ? 0.f : (get_pixel(im, i, j, k) - minimum) / range);
            }
        }
    }
}

image *sobel_image(image im) {
    image *sobel_im = calloc(2, sizeof(image));
    image gx = convolve_image(im, make_gx_filter(), 0);
    image gy = convolve_image(im, make_gy_filter(), 0);
    sobel_im[0] = make_image(im.w, im.h, 1);
    sobel_im[1] = make_image(im.w, im.h, 1);
    for (int i = 0; i < im.w * im.h; ++i) {
        sobel_im[0].data[i] = sqrtf(gx.data[i] * gx.data[i] + gy.data[i] * gy.data[i]);
        sobel_im[1].data[i] = atan2f(gx.data[i], gy.data[i]);
    }
    free_image(gx);
    free_image(gy);
    return sobel_im;
}

image colorize_sobel(image im) {
    image *sobel_im = sobel_image(im);
    feature_normalize(sobel_im[0]);
    feature_normalize(sobel_im[1]);

    image color_im = make_image(im.w, im.h, im.c);

    for (int j = 0; j < im.h; ++j) {
        for (int i = 0; i < im.w; ++i) {
            set_pixel(color_im, i, j, 0, get_pixel(sobel_im[1], i, j, 0));
            set_pixel(color_im, i, j, 1, get_pixel(sobel_im[0], i, j, 0));
            set_pixel(color_im, i, j, 2, get_pixel(sobel_im[0], i, j, 0));
        }
    }
    hsv_to_rgb(color_im);

    free_image(sobel_im[0]);
    free_image(sobel_im[1]);
    free(sobel_im);
    return color_im;
}
