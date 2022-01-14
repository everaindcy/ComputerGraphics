#ifndef WORLD_GEN_H
#define WORLD_GEN_H

#include "../util/utils.h"
#include "../object/objects.h"
#include "../texture/textures.h"
#include "../material/materials.h"

#include <iostream>

hittable_list random_scene() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    hittable_list ball_list;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0,.5), 0);
                    ball_list.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    ball_list.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    ball_list.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
    world.add(make_shared<bvh_node>(ball_list, 0.0, 1.0));

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return objects;
}

hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("textures/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    return hittable_list(globe);
}

hittable_list triangles() {
    hittable_list objects;

    auto earth_texture = make_shared<image_texture>("textures/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    objects.add(make_shared<sphere>(point3(0,0,0), 2, earth_surface));

    objects.add(make_shared<triangle>(point3(0,4,-1), point3(0,-4,-1), point3(2.5,0,-1), make_shared<metal>(color(0.9,0.8,0.7), 0)));

    return objects;
}

hittable_list rotate_earth() {
    auto earth_texture = make_shared<image_texture>("textures/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);
    auto earth = make_shared<transform>(mat4::translation(0.3,0.3,0.3)*mat4::rotation(vec3(1,1,1),60)*mat4::identity(), globe);

    return hittable_list(earth);
}

hittable_list simple_light() {
    hittable_list objects;

    auto pertext = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
    objects.add(make_shared<sphere>(point3(0,7,0), 2, difflight));

    return objects;
}

hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    objects.add(make_shared<transform>(mat4::translation(265,0,295)*mat4::rotateY(15), box1));

    shared_ptr<hittable> box2 = make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    objects.add(make_shared<transform>(mat4::translation(130,0,65)*mat4::rotateY(-18), box2));

    return objects;
}

hittable_list bunny2() {
    auto ground_texture = make_shared<checker_texture>(color(1,1,1), color(0.9,0.3,0.8));
    auto ground_surface = make_shared<lambertian>(ground_texture);
    auto ground = make_shared<sphere>(point3(0, -1000, 0), 1000.07, ground_surface);

    auto ball0_surface = make_shared<metal>(color(0.8, 0.8, 0.9), 0.0);
    auto ball1_surface = make_shared<lambertian>(color(0.2, 0.9, 0.6));
    auto ball2_surface = make_shared<lambertian>(color(0.9, 0.2, 0.6));
    auto ball0 = make_shared<sphere>(point3(0.3,0.57,-1), 0.5, ball0_surface);
    auto ball1 = make_shared<sphere>(point3(6,1,-2), 1, ball1_surface);

    auto mesh_surface = make_shared<lambertian>(color(0.3, 0.3, 0.8));
    auto bunny = make_shared<mesh>("meshs/bunny_1k.obj", mesh_surface);

    hittable_list objects;
    objects.add(ground);
    objects.add(ball0);
    objects.add(bunny);

    return objects;
}

hittable_list bunny() {
    hittable_list objects;

    auto earth_texture = make_shared<image_texture>("textures/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    objects.add(make_shared<sphere>(point3(0,1,1), 0.5, earth_surface));

    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto sphere_material = make_shared<metal>(color(0.9,0.8,0.7), 0.3);

    auto bunny = make_shared<mesh>("meshs/bunny_1k.obj", sphere_material);
    objects.add(bunny);

    return objects;
}

hittable_list smooth_cube() {
    auto earth_texture = make_shared<image_texture>("textures/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto sphere_material = make_shared<metal>(color(0.9,0.8,0.7), 0.3);

    auto bunny = make_shared<mesh>("meshs/cube.obj", earth_surface);

    return hittable_list(bunny);
}

hittable_list smooth_bunny() {
    hittable_list objects;

    auto earth_texture = make_shared<image_texture>("textures/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    // objects.add(make_shared<sphere>(point3(0,1,1), 0.5, earth_surface));

    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto sphere_material = make_shared<metal>(color(0.9,0.8,0.7), 0.3);

    auto bunny = make_shared<mesh>("meshs/bunny_1k.obj", sphere_material);
    objects.add(bunny);

    return objects;
}

hittable_list cornell_smoke() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<transform>(mat4::translation(265,0,295)*mat4::rotateY(15),box1);

    shared_ptr<hittable> box2 = make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<transform>(mat4::translation(130,0,65)*mat4::rotateY(-18),box2);

    objects.add(make_shared<constant_medium>(box1, 0.01, color(0,0,0)));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    return objects;
}

hittable_list final_scene() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1,101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30,0,0);
    auto moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(point3(360,150,145), 70, make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    objects.add(make_shared<constant_medium>(boundary, .0001, color(1,1,1)));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("textures/earthmap.jpg"));
    objects.add(make_shared<sphere>(point3(400,200,400), 100, emat));
    auto pertext = color(0.35,0.7,0.4);
    objects.add(make_shared<sphere>(point3(220,280,300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0,165), 10, white));
    }

    objects.add(make_shared<transform>(mat4::translation(-100,270,395)*mat4::rotateY(15),make_shared<bvh_node>(boxes2, 0.0, 1.0)));

    return objects;
}

hittable_list curves() {
    hittable_list objects;

    auto earth_texture = make_shared<image_texture>("textures/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);

    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto metal_material = make_shared<metal>(color(0.9,0.8,0.7), 0);

    double a[100] = {-2, -4, 0, -2, -3, -5};
    double b[100] = {2, 0, 0, -2, -5, -3};
    int n = 4;
    auto curve_face = make_shared<surface_rev>(make_shared<bezierCurve2D>(a, b, n), metal_material);
    objects.add(curve_face);
    
    auto sphere_material = make_shared<lambertian>(color(0.7, 0.2, 0.3));
    objects.add(make_shared<sphere>(point3(-2, -1.5, 2), 0.5, sphere_material));
    
    sphere_material = make_shared<lambertian>(color(0.1, 0.3, 0.6));
    objects.add(make_shared<sphere>(point3(1.2, -1.5, 2), 0.5, sphere_material));

    auto ground_texture = make_shared<checker_texture>(color(1,1,1), color(0.3,0.3,0.3));
    auto ground_surface = make_shared<lambertian>(ground_texture);
    auto ground = make_shared<sphere>(point3(0, -1000, 0), 1000-2, ground_surface);
    objects.add(ground);

    return objects;
}

#endif