#include "_scene.h"

static hittable_list cornell_box_objs() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(163, 393, 177, 382, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    objects.add(make_shared<transform>(mat4::translation(265,0,295)*mat4::rotateY(15), box1));

    shared_ptr<hittable> box2 = make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    objects.add(make_shared<transform>(mat4::translation(130,0,65)*mat4::rotateY(-18), box2));

    return objects;
}

static hittable_list cornell_box_objs2() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(163, 393, 177, 382, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    objects.add(make_shared<transform>(mat4::translation(265,0,295)*mat4::rotateY(15), box1));

    shared_ptr<hittable> box2 = make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<transform>(mat4::translation(130,0,65)*mat4::rotateY(-18), box2);
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    return objects;
}

static hittable_list cornell_box_objs3() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .25));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(10, 10, 10));
    auto light1 = make_shared<diffuse_light>(color(0, 10, 10));
    auto light2 = make_shared<diffuse_light>(color(10, 0, 10));
    auto light3 = make_shared<diffuse_light>(color(10, 10, 0));

    objects.add(make_shared<yz_rect>(50, 200, 177, 382, 554, light));
    objects.add(make_shared<yz_rect>(200, 350, 27, 232, 554, light1));
    objects.add(make_shared<yz_rect>(200, 350, 177, 382, 554, light2));
    objects.add(make_shared<yz_rect>(50, 200, 27, 232, 554, light3));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, red));
    
    shared_ptr<hittable> box0 = make_shared<box>(point3(-1, -1, -1), point3(556, 556, 556), white);
    // objects.add(make_shared<constant_medium>(box0, 0.001, color(1,1,1)));
    
    auto glass_mtl = make_shared<dielectric>(1.5);
    auto bunny = parse_obj_ptr("meshs/dragon.obj", "meshs/", true, true, glass_mtl, true);
    objects.add(make_shared<transform>(mat4::translation(230,130,95)*mat4::scaling(400, 400, 400)*mat4::rotateY(90), bunny));

    return objects;
}

scene cornell_box_scene() {
    scene sce;

    sce.objs = cornell_box_objs3();

    sce.image_width = 1000;
    sce.aspect_ratio = 1.0;
    sce.image_height = static_cast<int>(sce.image_width / sce.aspect_ratio);

    sce.lookfrom = point3(278, 278, -800);
    sce.lookat = point3(278, 278, 0);
    sce.vup = vec3(0,1,0);
    sce.vfov = 40.0;
    sce.dist_to_focus = 10;
    sce.aperture = 0;
    sce.time0 = 0;
    sce.time1 = 1;

    sce.background = color(0, 0, 0);
    
    sce.samples_per_pixel = 500;
    sce.max_depth = 50;

    sce.resetcam();

    return sce;
}