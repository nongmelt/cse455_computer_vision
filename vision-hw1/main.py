from uwimg import *


def extract_freq(im, sigma, preserve=1):
    f = make_gaussian_filter(sigma)

    lfreq = convolve_image(im, f, preserve)
    return lfreq, im - lfreq


im_ron = load_image("data/ron.png")
im_dd = load_image("data/dumbledore.png")

lfreq_r, hfreq_r = extract_freq(im_ron, 2.2)
lfreq_dd, hfreq_dd = extract_freq(im_dd, 3)

dd_ron = lfreq_dd + hfreq_r

clamp_image(dd_ron)
save_image(dd_ron, "output/ronbledore")

lfreq_r, hfreq_r = extract_freq(im_ron, 5)
lfreq_dd, hfreq_dd = extract_freq(im_dd, 3)
ron_dd = lfreq_r + hfreq_dd

clamp_image(ron_dd)
save_image(ron_dd, "output/dumbleron")

im = load_image("data/dog.jpg")
im = convolve_image(im, make_gaussian_filter(2), 1)
color_s = colorize_sobel(im)

save_image(color_s, "output/sobel")
