#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "../util/utils.h"
#include "../object/hittable/hittable_list.h"

class scene {
public:
    scene() {
        image_width = 400;
        aspect_ratio = 16.0/9.0;
        image_height = static_cast<int>(image_width / aspect_ratio);

        lookfrom = point3(0,0,10);
        lookat = point3(0,0,0);
        vup = vec3(0,1,0);
        vfov = 40;
        dist_to_focus = 10;
        aperture = 0;
        time0 = 0;
        time1 = 1;

        background = color(0,0,0);

        samples_per_pixel = 100;
        max_depth = 50;

        cam = new camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);

        objs = hittable_list();
    }

    void setcam(
        point3 lookfrom,
        point3 lookat,
        vec3   vup,
        double vfov, // vertical field-of-view in degrees
        double aspect_ratio,
        double aperture,
        double focus_dist,
        double _time0 = 0,
        double _time1 = 0
    ) {
        this->lookfrom = lookfrom;
        this->lookat = lookat;
        this->vup = vup;
        this->aspect_ratio = aspect_ratio;
        this->image_height = static_cast<int>(image_width / aspect_ratio);
        this->aperture = aperture;
        this->dist_to_focus = focus_dist;
        this->time0 = _time0;
        this->time1 = _time1;

        delete cam;
        cam = new camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, _time0, _time1);
    }

    void resetcam() {
        delete cam;
        cam = new camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, time0, time1);
    }

public:
    camera *cam;
    hittable_list objs;

public:
    // image

    int image_width;
    int image_height;
    double aspect_ratio;

    // camera

    point3 lookfrom;
    point3 lookat;
    vec3 vup;
    double vfov;
    double dist_to_focus;
    double aperture;
    double time0;
    double time1;

    // scene

    color background;

    // parameter

    int samples_per_pixel;
    int max_depth;
};

#endif