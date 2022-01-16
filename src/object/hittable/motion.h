#ifndef MOTION_H
#define MOTION_H

#include "hittable.h"
#include "transform.h"

class motion : public hittable {
public:
    motion() {}
    motion(std::function<mat4(double)> mot, double time0, double time1, shared_ptr<hittable> obj)
        : mot(mot), time0(time0), time1(time1), obj(obj), have_box(false) {}
    motion(std::function<mat4(double)> mot, double time0, double time1, shared_ptr<hittable> obj, aabb box)
        : mot(mot), time0(time0), time1(time1), obj(obj), box(box), have_box(true) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
        double t = clamp(r.time(), t_min, t_max);
        shared_ptr<hittable> obj_moved = make_shared<transform>(mot(t), obj);
        return obj_moved->hit(r, t_min, t_max, rec);
    }
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        output_box = box;
        return have_box;
    }

public:
    std::function<mat4(double)> mot;
    double time0, time1;
    shared_ptr<hittable> obj;
    aabb box;

    bool have_box;
};

#endif