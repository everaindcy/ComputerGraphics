#ifndef PLANE_H
#define PLANE_H

#include "hittable.h"

// the plane is dot(p, dir) == d
class plane : public hittable {
public:
    plane() {}
    plane(double a, double b, double c, double d, shared_ptr<material> m)
        : dir(vec3(a, b, c)), d(d), mat_ptr(m) {}
    plane(vec3 dir, double d, shared_ptr<material> m)
        : dir(dir), d(d), mat_ptr(m) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        return false;
    }

public:
    vec3 dir;
    double d;
    shared_ptr<material> mat_ptr;
};

bool plane::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    if (near_zero(dot(unit_vector(r.direction()), unit_vector(dir)))) {
        return false;
    }
    rec.t = (d - dot(dir, r.origin()))/dot(dir, r.direction());
    if (rec.t < t_min || rec.t > t_max) return false;
    rec.p = r.at(rec.t);
    vec3 outward_normal = unit_vector(dir);
    rec.set_face_normal(r, outward_normal);
    rec.u = 0;
    rec.v = 0;
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif