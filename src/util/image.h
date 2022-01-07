#ifndef IMAGE_H
#define IMAGE_H

#include "../util/utils.h"

#include <cstdio>

class image {
public:
    image() {}
    image(int image_width, int image_height)
        : image_width(image_width), image_height(image_height) {
        img = new color[image_width*image_height];
    }
    ~image() {
        delete[] img;
    }

    void set(color data, int x, int y) {
        img[x*image_width + y] = data;
    }

    void set(double r, double g, double b, int x, int y) {
        img[x*image_width + y] = color(r, g, b);
    }

    void print_ppm() {
        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
        for (int i = 0; i < image_height*image_width; i++) {
            std::cout << img[i] << "\n";
        }
        std::cout << std::flush;
    }

public:
    int image_width;
    int image_height;
    color* img;
};

#endif