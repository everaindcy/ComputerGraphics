#ifndef SCENE_GEN_H
#define SCENE_GEN_H

#include "scene.h"
#include "world_gen.h"

void scene_gen(scene &sce, int idx) {
    switch (idx) {
        case 1:
            sce.objs = random_scene();
            sce.background = color(0.70, 0.80, 1.00);
            sce.lookfrom = point3(13,2,3);
            sce.lookat = point3(0,0,0);
            sce.vfov = 20.0;
            sce.aperture = 0.1;
            break;

        case 2:
            sce.objs = two_spheres();
            sce.background = color(0.70, 0.80, 1.00);
            sce.lookfrom = point3(13,2,3);
            sce.lookat = point3(0,0,0);
            sce.vfov = 20.0;
            break;

        case 4:
            sce.objs = earth();
            sce.background = color(0.70, 0.80, 1.00);
            sce.lookfrom = point3(13,2,3);
            sce.lookat = point3(0,0,0);
            sce.vfov = 20.0;
            break;
        
        case 5:
            sce.objs = simple_light();
            sce.samples_per_pixel = 400;
            sce.background = color(0,0,0);
            sce.lookfrom = point3(26,3,6);
            sce.lookat = point3(0,2,0);
            sce.vfov = 20.0;
            break;
        
        case 6:
            sce.objs = cornell_box();
            sce.aspect_ratio = 1.0;
            sce.image_width = 600;
            sce.samples_per_pixel = 200;
            sce.background = color(0,0,0);
            sce.lookfrom = point3(278, 278, -800);
            sce.lookat = point3(278, 278, 0);
            sce.vfov = 40.0;
            break;

        case 10:
            sce.objs = rotate_earth();
            sce.background = color(0.70, 0.80, 1.00);
            sce.lookfrom = point3(0,0,14);
            sce.lookat = point3(0,0,0);
            sce.vfov = 20.0;
            break;

        case 11:
            sce.objs = triangles();
            sce.background = color(0.70, 0.80, 1.00);
            sce.lookfrom = point3(14,0,14);
            sce.lookat = point3(0,0,0);
            sce.vfov = 20.0;
            break;

        default:
            sce.background = color(0.0, 0.0, 0.0);
            break;
    }

    sce.image_height = static_cast<int>(sce.image_width / sce.aspect_ratio);
    sce.resetcam();
}

#endif