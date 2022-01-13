#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"

class triangle : public hittable {
public:
    triangle() {}
    triangle(point3 a, point3 b, point3 c, shared_ptr<material> m)
        : mp(m), v{a, b, c}, normal(unit_vector(cross(b-a,c-a))) {}
    
    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(
        double time0, double time1, aabb& output_box) const override;

public:
    shared_ptr<material> mp;
    point3 v[3];
    vec3 normal;
};

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // if (dot(r.direction(), normal) >= 0) {
    //     return false;
    // }

    const point3 &a = v[0];
    const point3 &b = v[1];
    const point3 &c = v[2];

    vec3 S = a - r.origin();
    vec3 E1 = a - b;
    vec3 E2 = a - c;
    vec3 R = r.direction();

    double norm = mat3(R, E1, E2).determinant();
    if (norm == 0) {
        return false;
    }
    double t = mat3(S, E1, E2).determinant()/norm;
    double beta = mat3(R, S, E2).determinant()/norm;
    double gamma = mat3(R, E1, S).determinant()/norm;
    if (t < 0 || t < t_min || t > t_max) {
        return false;
    }
    if (beta < 0 || beta > 1 || gamma < 0 || gamma > 1 || beta + gamma > 1) {
        return false;
    }

    rec.t = t;
    rec.p = r.at(rec.t);
    rec.set_face_normal(r, normal);
    rec.u = beta;
    rec.v = gamma;
    rec.mat_ptr = mp;

    return true;
}

bool triangle::bounding_box(double time0, double time1, aabb& output_box) const {
    point3 min, max;
    min = max = v[0];

    min[0] = fmin(min[0], v[1][0]);
    min[1] = fmin(min[1], v[1][1]);
    min[2] = fmin(min[2], v[1][2]);
    max[0] = fmax(max[0], v[1][0]);
    max[1] = fmax(max[1], v[1][1]);
    max[2] = fmax(max[2], v[1][2]);

    min[0] = fmin(min[0], v[2][0]);
    min[1] = fmin(min[1], v[2][1]);
    min[2] = fmin(min[2], v[2][2]);
    max[0] = fmax(max[0], v[2][0]);
    max[1] = fmax(max[1], v[2][1]);
    max[2] = fmax(max[2], v[2][2]);

    min[0] -= 0.0001;
    min[1] -= 0.0001;
    min[2] -= 0.0001;
    max[0] += 0.0001;
    max[1] += 0.0001;
    max[2] += 0.0001;

    output_box = aabb(min, max);
    return true;
}

class smoothTriangle : public hittable {
public:
    smoothTriangle() {}
    smoothTriangle(point3 a, point3 b, point3 c, shared_ptr<material> m)
        : mp(m), v{a, b, c}, this_normal(unit_vector(cross(b-a,c-a))) {}
    smoothTriangle(point3 a, point3 b, point3 c, vec3 norm_a, vec3 norm_b, vec3 norm_c, shared_ptr<material> m)
        : mp(m), v{a, b, c}, normal{norm_a, norm_b, norm_c}, this_normal(unit_vector(cross(b-a,c-a))) {}

    void set_normal(vec3 norm_a, vec3 norm_b, vec3 norm_c) {
        normal[0] = norm_a;
        normal[1] = norm_b;
        normal[2] = norm_c;
    }
    
    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(
        double time0, double time1, aabb& output_box) const override;

public:
    shared_ptr<material> mp;
    point3 v[3];
    vec3 normal[3];
    vec3 this_normal;
};

bool smoothTriangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // if (dot(r.direction(), this_normal) >= 0) {
    //     return false;
    // }

    const point3 &a = v[0];
    const point3 &b = v[1];
    const point3 &c = v[2];

    vec3 S = a - r.origin();
    vec3 E1 = a - b;
    vec3 E2 = a - c;
    vec3 R = r.direction();

    double norm = mat3(R, E1, E2).determinant();
    if (norm == 0) {
        return false;
    }
    double t = mat3(S, E1, E2).determinant()/norm;
    double beta = mat3(R, S, E2).determinant()/norm;
    double gamma = mat3(R, E1, S).determinant()/norm;
    if (t < 0 || t < t_min || t > t_max) {
        return false;
    }
    if (beta < 0 || beta > 1 || gamma < 0 || gamma > 1 || beta + gamma > 1) {
        return false;
    }

    double alpha = 1 - beta - gamma;
    vec3 outward_normal = unit_vector(alpha*normal[0] + beta*normal[1] + gamma*normal[2]);

    rec.t = t;
    rec.p = r.at(rec.t);
    rec.set_face_normal(r, outward_normal);
    rec.u = beta;
    rec.v = gamma;
    rec.mat_ptr = mp;

    return rec.front_face == (dot(r.direction(), this_normal) <= 0);
}

bool smoothTriangle::bounding_box(double time0, double time1, aabb& output_box) const {
    point3 min, max;
    min = max = v[0];

    min[0] = fmin(min[0], v[1][0]);
    min[1] = fmin(min[1], v[1][1]);
    min[2] = fmin(min[2], v[1][2]);
    max[0] = fmax(max[0], v[1][0]);
    max[1] = fmax(max[1], v[1][1]);
    max[2] = fmax(max[2], v[1][2]);

    min[0] = fmin(min[0], v[2][0]);
    min[1] = fmin(min[1], v[2][1]);
    min[2] = fmin(min[2], v[2][2]);
    max[0] = fmax(max[0], v[2][0]);
    max[1] = fmax(max[1], v[2][1]);
    max[2] = fmax(max[2], v[2][2]);

    min[0] -= 0.0001;
    min[1] -= 0.0001;
    min[2] -= 0.0001;
    max[0] += 0.0001;
    max[1] += 0.0001;
    max[2] += 0.0001;

    output_box = aabb(min, max);
    return true;
}

#endif