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

scene curve_scene() {
    scene sce;

    sce.objs = curves_no_mesh();

    sce.image_width = 1080;
    sce.aspect_ratio = 16.0/9.0;
    sce.image_height = static_cast<int>(sce.image_width / sce.aspect_ratio);

    sce.lookfrom = point3(0,10,14);
    sce.lookat = point3(0,0,0);
    sce.vup = vec3(0,1,0);
    sce.vfov = 25.0;
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