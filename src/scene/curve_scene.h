#include "_scene.h"

static hittable_list curve_objs() {
    hittable_list objects;

    auto earth_texture = make_shared<image_texture>("textures/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);

    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto metal_material = make_shared<metal>(color(0.9,0.8,0.7), 0);

    double a[100] = {-2, -4, 0, -2, -3, -5};
    double b[100] = {2, 0, 0, -2, -5, -3};
    int n = 4;
    auto curve_face = make_shared<surface_rev>(make_shared<bezierCurve2D>(a, b, n), metal_material, false, 50);
    objects.add(curve_face);
    
    auto sphere_material = make_shared<lambertian>(color(0.7, 0.2, 0.3));
    objects.add(make_shared<sphere>(point3(-2, -1.5, 2), 0.5, sphere_material));
    
    sphere_material = make_shared<lambertian>(color(0.1, 0.3, 0.6));
    objects.add(make_shared<sphere>(point3(1.2, -1.5, 2), 0.5, sphere_material));

    auto ground_texture = make_shared<checker_texture>(color(1,1,1), color(0.3,0.3,0.3));
    auto ground_surface = make_shared<lambertian>(ground_texture);
    auto ground = make_shared<plane>(point3(0, 1, 0), -2, ground_surface);
    objects.add(ground);

    return objects;
}

static hittable_list bottle_objs() {
    hittable_list objects;

    auto light_mtl = make_shared<diffuse_light>(color(20, 20, 20));
    auto light = make_shared<sphere>(point3(2.0, 1.5, 5), 0.5, light_mtl);
    objects.add(light);
    
    auto ground_texture = make_shared<checker_texture>(color(1,1,1), color(0.3,0.3,0.3));
    auto ground_surface = make_shared<lambertian>(ground_texture);
    auto ground = make_shared<plane>(point3(0, 1, 0), -2, ground_surface);
    // auto ground = make_shared<sphere>(point3(0, -1000, 0), 1000, ground_surface);
    objects.add(ground);

    auto metal_material = make_shared<metal>(color(0.9,0.8,0.7), 0);
    auto bottle_texture = make_shared<image_texture>("textures/bottle.jpg");
    auto bottle_mtl = make_shared<lambertian>(bottle_texture);
    auto glass_mtl = make_shared<dielectric>(1.5);

    double a[100] = {-2, -4, 0, -2, -3, -5};
    double b[100] = {2, 0, 0, -2, -5, -3};
    int n = 4;
    auto curve_face = make_shared<surface_rev>(make_shared<bezierCurve2D>(a, b, n), bottle_mtl, false, 50);
    objects.add(curve_face);
    // auto curve_out_face = make_shared<surface_rev>(make_shared<bezierCurve2D>(a, b, n), glass_mtl, false, 50);
    // objects.add(make_shared<transform>(mat4::scaling(1.01, 1, 1.01), curve_out_face));

    // objects.add(make_shared<xz_rect>(-0.5, 0.5, -0.5, 0.5, 0.01001, make_shared<lambertian>(color(0.3, 0.2, 0.7))));

    return objects;
}

scene curve_scene() {
    scene sce;

    sce.objs = bottle_objs();

    sce.image_width = 1080;
    sce.aspect_ratio = 16.0/9.0;
    sce.image_height = static_cast<int>(sce.image_width / sce.aspect_ratio);

    sce.lookfrom = point3(0,10,14);
    sce.lookat = point3(0,2,0);
    sce.vup = vec3(0,1,0);
    sce.vfov = 30.0;
    sce.dist_to_focus = 10;
    sce.aperture = 0;
    sce.time0 = 0;
    sce.time1 = 1;

    sce.background = color(0.70, 0.80, 1.00);
    
    sce.samples_per_pixel = 200;
    sce.max_depth = 50;

    sce.resetcam();

    return sce;
}

scene curve_scene2() {
    scene sce;

    sce.objs = bottle_objs();

    sce.image_width = 1000;
    sce.aspect_ratio = 10.0/9.0;
    sce.image_height = static_cast<int>(sce.image_width / sce.aspect_ratio);

    sce.lookfrom = point3(0,0,9);
    sce.lookat = point3(0,0,0);
    sce.vup = vec3(0,1,0);
    sce.vfov = 40.0;
    sce.dist_to_focus = 10;
    sce.aperture = 0;
    sce.time0 = 0;
    sce.time1 = 1;

    sce.background = color(0.14*3, 0.16*3, 0.20*3);
    
    sce.samples_per_pixel = 5000;
    sce.max_depth = 200;

    sce.resetcam();

    return sce;
}