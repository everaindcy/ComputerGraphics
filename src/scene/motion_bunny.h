#include "_scene.h"

static hittable_list bunny_objs() {
    hittable_list objects;

    hittable_list bunny = parse_obj("meshs/bunny/bunny.obj", "meshs/bunny/", true, true,
                                    make_shared<lambertian>(color(0.3, 0.2, 0.7)), true);
    objects.add(bunny);
    
    auto ground_texture = make_shared<checker_texture>(color(1,1,1), color(0.3,0.3,0.3));
    auto ground_surface = make_shared<lambertian>(ground_texture);
    auto ground = make_shared<plane>(point3(0, 1, 0), 0, ground_surface);
    objects.add(ground);

    return objects;
}

static hittable_list motion_bunny_objs() {
    hittable_list objects;

    auto bunny = parse_obj_ptr("meshs/bunny/bunny.obj", "meshs/bunny/", true, true,
                                    make_shared<lambertian>(color(0.3, 0.2, 0.7)), true);
    
    auto mot = [](double t)->mat4 {
        return mat4::translation(0.2*t,0,0);
    };
    auto motion_bunny = make_shared<motion>(mot,0,1,bunny);
    // objects.add(motion_bunny);
    
    auto ground_texture = make_shared<checker_texture>(color(1,1,1), color(0.3,0.3,0.3));
    auto ground_surface = make_shared<lambertian>(ground_texture);
    auto ground = make_shared<plane>(point3(0, 1, 0), 0.01, ground_surface);
    // auto ground = make_shared<sphere>(point3(0, -1000, 0), 1000, ground_surface);

    objects.add(ground);

    auto metal_material = make_shared<metal>(color(0.9,0.8,0.7), 0);

    double a[100] = {-2, -4, 0, -2, -3, -5};
    double b[100] = {2, 0, 0, -2, -5, -3};
    int n = 4;
    auto curve_face = make_shared<surface_rev>(make_shared<bezierCurve2D>(a, b, n), metal_material, false, 50);
    objects.add(make_shared<transform>(mat4::translation(0, 2, -3), curve_face));

    objects.add(make_shared<xz_rect>(-0.5, 0.5, -0.5, 0.5, 0.01001, make_shared<lambertian>(color(0.3, 0.2, 0.7))));

    return objects;
}

scene motion_bunny_scene() {
    scene sce;

    sce.objs = motion_bunny_objs();

    sce.image_width = 800;
    sce.aspect_ratio = 16.0/9.0;
    sce.image_height = static_cast<int>(sce.image_width / sce.aspect_ratio);

    sce.lookfrom = point3(0,2,5);
    sce.lookat = point3(0,1.4,0);
    sce.vfov = 40.0;
    sce.dist_to_focus = 10;
    sce.aperture = 0;
    sce.time0 = 0;
    sce.time1 = 1;
    sce.vup = vec3(0,1,0);

    sce.background = color(0.70, 0.80, 1.00);
    
    sce.samples_per_pixel = 100;
    sce.max_depth = 50;

    sce.resetcam();

    return sce;
}