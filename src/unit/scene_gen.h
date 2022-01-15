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

        case 7:
            sce.objs = cornell_smoke();
            sce.aspect_ratio = 1.0;
            sce.image_width = 600;
            sce.samples_per_pixel = 200;
            sce.lookfrom = point3(278, 278, -800);
            sce.lookat = point3(278, 278, 0);
            sce.vfov = 40.0;
            break;

        case 8:
            sce.objs = final_scene();
            sce.aspect_ratio = 1.0;
            sce.image_width = 800;
            sce.samples_per_pixel = 10000;
            sce.background = color(0,0,0);
            sce.lookfrom = point3(478, 278, -600);
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

        case 12:
            sce.objs = bunny();
            sce.background = color(0.70, 0.80, 1.00);
            sce.lookfrom = point3(0,0.5,1);
            sce.lookat = point3(0,0.2,0);
            sce.vfov = 20.0;
            break;

        case 13:
            sce.objs = smooth_cube();
            sce.background = color(0.70, 0.80, 1.00);
            sce.lookfrom = point3(6,6,6);
            sce.lookat = point3(0,0,0);
            sce.vfov = 20.0;
            break;

        case 14:
            sce.objs = smooth_bunny();
            sce.background = color(0.70, 0.80, 1.00);
            sce.lookfrom = point3(0,0.5,1);
            sce.lookat = point3(0,0.2,0);
            sce.vfov = 20.0;
            break;

        case 15:
            sce.objs = curves();
            sce.background = color(0.70, 0.80, 1.00);
            sce.lookfrom = point3(0,10,14);
            sce.lookat = point3(0,0,0);
            sce.image_width = 800;
            sce.vfov = 25.0;
            break;

        case 16:
            sce.objs = bunny2();
            sce.background = color(0.7,0.8,0.9);
            sce.lookfrom = point3(0,0.2,0.8);
            sce.lookat = point3(0,0.2,0);
            sce.vfov = 40.0;
            sce.image_width = 800;
            sce.aspect_ratio = 16.0/9.0;
            sce.aperture = 0.03;
            sce.dist_to_focus = 0.8;
            break;
        
        case 17:
            sce.objs = parse_obj("meshs/bunny_1k.obj", "meshs/", false, false, make_shared<lambertian>(color(0.7, 0.3, 0.2)));
            sce.background = color(0.7,0.8,0.9);
            sce.lookfrom = point3(0,0.2,0.8);
            sce.lookat = point3(0,0.2,0);
            sce.vfov = 40.0;
            sce.image_width = 800;
            sce.aspect_ratio = 16.0/9.0;
            sce.aperture = 0.03;
            sce.dist_to_focus = 0.8;
            break;

        default:
            sce.background = color(0.0, 0.0, 0.0);
            break;
    }

    sce.image_height = static_cast<int>(sce.image_width / sce.aspect_ratio);
    sce.resetcam();
}

#endif