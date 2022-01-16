#include "_scene.h"

scene house_scene() {
    scene sce;

    sce.objs = parse_obj("meshs/house/cottage_obj.obj", "meshs/house/textrues", true, true);

    sce.image_width = 200;
    sce.aspect_ratio = 1.0;
    sce.image_height = static_cast<int>(sce.image_width / sce.aspect_ratio);

    sce.lookfrom = point3(278, 278, 278);
    sce.lookat = point3(0, 0, 0);
    sce.vup = vec3(0,1,0);
    sce.vfov = 40.0;
    sce.dist_to_focus = 10;
    sce.aperture = 0;
    sce.time0 = 0;
    sce.time1 = 1;

    sce.background = color(0.7, 0.7, 0.7);
    
    sce.samples_per_pixel = 100;
    sce.max_depth = 50;

    sce.resetcam();

    return sce;
}