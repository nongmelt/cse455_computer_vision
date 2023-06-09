from uwimg import *

# 1. Getting and setting pixels
im = load_image("data/dog.jpg")
for row in range(im.h):
    for col in range(im.w):
        set_pixel(im, col, row, 0, 0)
save_image(im, "output/dog_no_red")

# 2. Copying images
im = load_image("data/dog.jpg")
im_copy = copy_image(im)
save_image(im_copy, "output/dog_copy")

# 3. Grayscale image
im = load_image("data/colorbar.png")
graybar = rgb_to_grayscale(im)
save_image(graybar, "output/graybar")

# 4. Shift Image
im = load_image("data/dog.jpg")
shift_image(im, 0, 0.4)
shift_image(im, 1, 0.4)
shift_image(im, 2, 0.4)
save_image(im, "output/overflow")

# 5. Clamp Image
clamp_image(im)
save_image(im, "output/doglight_fixed")

# 6-7. Colorspace and saturation
im = load_image("data/dog.jpg")
rgb_to_hsv(im)
shift_image(im, 1, 0.2)
clamp_image(im)
hsv_to_rgb(im)
save_image(im, "output/dog_saturated")

# 8. Scale image
im = load_image("data/dog.jpg")
rgb_to_hsv(im)
scale_image(im, 1, 2)
clamp_image(im)
hsv_to_rgb(im)
save_image(im, "output/dog_scale_saturated")
