#ifndef TRANSFORM
#define TRANSFORM

#include "hittable.h"

// transforms a 3D point using a matrix, returning a 3D point
static vec3 transformPoint(const mat4 &mat, const vec3 &point) {
    return (mat * vec4(point, 1)).xyz();
}

// transform a 3D directino using a matrix, returning a direction
static vec3 transformDirection(const mat4 &mat, const vec3 &dir) {
    return (mat * vec4(dir, 0)).xyz();
}

class transform : public hittable {
public:
    transform() {}
    transform(const mat4 &m, shared_ptr<hittable> obj) : trans(m.inverse()), obj(obj) {}

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(
        double time0, double time1, aabb& output_box) const override;

public:
    mat4 trans;
    shared_ptr<hittable> obj;
};

bool transform::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    ray r_trans(transformPoint(trans, r.origin()), transformDirection(trans, r.direction()));
    bool hitting = obj->hit(r_trans, t_min, t_max, rec);
    if (hitting) {
        rec.p = r.at(rec.t);
        rec.normal = unit_vector(transformDirection(trans.transposed(), rec.normal));
    }
    return hitting;
}

bool transform::bounding_box (double time0, double time1, aabb& output_box) const {
    bool have_box = obj->bounding_box(time0, time1, output_box);

    if (have_box) {
        double x1 = output_box.min().x();
        double x2 = output_box.max().x();
        double y1 = output_box.min().y();
        double y2 = output_box.max().y();
        double z1 = output_box.min().z();
        double z2 = output_box.max().z();

        vec3 min, max, temp;
        min = max = transformPoint(trans, vec3(x1, y1, z1));
        temp = transformPoint(trans, vec3(x1, y1, z2));
        min[0] = fmin(min[0], temp[0]);
        min[1] = fmin(min[1], temp[1]);
        min[2] = fmin(min[2], temp[2]);
        max[0] = fmax(max[0], temp[0]);
        max[1] = fmax(max[1], temp[1]);
        max[2] = fmax(max[2], temp[2]);
        temp = transformPoint(trans, vec3(x1, y2, z1));
        min[0] = fmin(min[0], temp[0]);
        min[1] = fmin(min[1], temp[1]);
        min[2] = fmin(min[2], temp[2]);
        max[0] = fmax(max[0], temp[0]);
        max[1] = fmax(max[1], temp[1]);
        max[2] = fmax(max[2], temp[2]);
        temp = transformPoint(trans, vec3(x1, y2, z2));
        min[0] = fmin(min[0], temp[0]);
        min[1] = fmin(min[1], temp[1]);
        min[2] = fmin(min[2], temp[2]);
        max[0] = fmax(max[0], temp[0]);
        max[1] = fmax(max[1], temp[1]);
        max[2] = fmax(max[2], temp[2]);
        temp = transformPoint(trans, vec3(x2, y1, z1));
        min[0] = fmin(min[0], temp[0]);
        min[1] = fmin(min[1], temp[1]);
        min[2] = fmin(min[2], temp[2]);
        max[0] = fmax(max[0], temp[0]);
        max[1] = fmax(max[1], temp[1]);
        max[2] = fmax(max[2], temp[2]);
        temp = transformPoint(trans, vec3(x2, y1, z2));
        min[0] = fmin(min[0], temp[0]);
        min[1] = fmin(min[1], temp[1]);
        min[2] = fmin(min[2], temp[2]);
        max[0] = fmax(max[0], temp[0]);
        max[1] = fmax(max[1], temp[1]);
        max[2] = fmax(max[2], temp[2]);
        temp = transformPoint(trans, vec3(x2, y2, z1));
        min[0] = fmin(min[0], temp[0]);
        min[1] = fmin(min[1], temp[1]);
        min[2] = fmin(min[2], temp[2]);
        max[0] = fmax(max[0], temp[0]);
        max[1] = fmax(max[1], temp[1]);
        max[2] = fmax(max[2], temp[2]);
        temp = transformPoint(trans, vec3(x2, y2, z2));
        min[0] = fmin(min[0], temp[0]);
        min[1] = fmin(min[1], temp[1]);
        min[2] = fmin(min[2], temp[2]);
        max[0] = fmax(max[0], temp[0]);
        max[1] = fmax(max[1], temp[1]);
        max[2] = fmax(max[2], temp[2]);

        output_box = aabb(min, max);
    }

    return have_box;
}

#endif