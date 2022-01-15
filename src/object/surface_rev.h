#ifndef SURFACE_REV_H
#define SURFACE_REV_H

#define Resolution_default 100

#include "hittable.h"
#include "mesh.h"
#include "../util/curve/curve.h"

class surface_rev : public hittable {
public:
    surface_rev() {}
    surface_rev(shared_ptr<curve> c, shared_ptr<material> m, bool use_mesh = true, int resolution = Resolution_default)
        : cv(c), mat_ptr(m), use_mesh(use_mesh), resolution(resolution) {
        if (use_mesh) {gen_display_mesh();}
        else {gen_box_mesh();}
    }
    surface_rev(shared_ptr<curve> c, shared_ptr<hittable> _in_mesh, shared_ptr<hittable> _out_mesh, shared_ptr<material> m)
        : cv(c), mat_ptr(m), in_mesh(_in_mesh), out_mesh(_out_mesh), use_mesh(false) {}

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(
        double time0, double time1, aabb& output_box) const override;

public:
    shared_ptr<curve> cv;
    shared_ptr<material> mat_ptr;
    
    // in_mesh is in the surface, out_mesh is out of it
    shared_ptr<hittable> in_mesh;
    shared_ptr<hittable> out_mesh;

    bool use_mesh;
    int resolution;

private:
    void gen_display_mesh();
    void gen_box_mesh();
};

void surface_rev::gen_display_mesh() {
    // Definition for drawable surface.
    typedef std::tuple<unsigned, unsigned, unsigned> Tup3u;
    struct Surface {
        std::vector<point3> VV;
        std::vector<vec3> VN;
        std::vector<vec3> VT;
        std::vector<Tup3u> VF;
    } surface;

    std::vector<curvePoint> curvePoints;
    cv->discretize(resolution, curvePoints);

    const int steps = resolution/2;
    for (unsigned int ci = 0; ci < curvePoints.size(); ++ci) {
        const curvePoint &cp = curvePoints[ci];
        for (unsigned int i = 0; i < steps; ++i) {
            double t = (double)i / steps;
            double degree = 360 * t;
            point3 pnew = transformPoint(mat4::rotateY(degree), cp.v);
            vec3 pNormal = unit_vector(cross(cp.dir, vec3(0,0,1)));
            vec3 nnew = transformDirection(mat4::rotateY(degree), pNormal);
            surface.VV.push_back(pnew);
            surface.VN.push_back(nnew);
            surface.VT.push_back(vec3(t, cp.t, 0));
            int i1 = (i + 1 == steps) ? 0 : i + 1;
            if (ci != curvePoints.size() - 1) {
                surface.VF.emplace_back((ci + 1) * steps + i, ci * steps + i1, ci * steps + i);
                surface.VF.emplace_back((ci + 1) * steps + i, (ci + 1) * steps + i1, ci * steps + i1);
            }
        }
    }

    hittable_list tris_in;
    for (unsigned i = 0; i < surface.VF.size(); i++) {
        tris_in.add(make_shared<triangle>(surface.VV[std::get<0>(surface.VF[i])],
                                          surface.VV[std::get<1>(surface.VF[i])],
                                          surface.VV[std::get<2>(surface.VF[i])],
                                          surface.VN[std::get<0>(surface.VF[i])],
                                          surface.VN[std::get<1>(surface.VF[i])],
                                          surface.VN[std::get<2>(surface.VF[i])],
                                          surface.VT[std::get<0>(surface.VF[i])],
                                          surface.VT[std::get<1>(surface.VF[i])],
                                          surface.VT[std::get<2>(surface.VF[i])],
                                          mat_ptr, true, true));
    }
    in_mesh = make_shared<mesh>(tris_in);
}

void surface_rev::gen_box_mesh() {
    // Definition for drawable surface.
    typedef std::tuple<unsigned, unsigned, unsigned> Tup3u;
    struct Surface {
        std::vector<point3> VV;
        std::vector<vec3> VN;
        std::vector<vec3> VT;
        std::vector<Tup3u> VF;
    } surface;

    std::vector<curvePoint> curvePoints;
    cv->discretize(resolution, curvePoints);

    const int steps = resolution/2;
    for (unsigned int ci = 0; ci < curvePoints.size(); ++ci) {
        const curvePoint &cp = curvePoints[ci];
        for (unsigned int i = 0; i < steps; ++i) {
            double t = (double)i / steps;
            double degree = 360 * t;
            point3 pnew = transformPoint(mat4::rotateY(degree), cp.v);
            vec3 pNormal = unit_vector(cross(cp.dir, vec3(0,0,1)));
            vec3 nnew = transformDirection(mat4::rotateY(degree), pNormal);
            surface.VV.push_back(pnew);
            surface.VN.push_back(nnew);
            surface.VT.push_back(vec3(t, cp.t, 0));
            int i1 = (i + 1 == steps) ? 0 : i + 1;
            if (ci != curvePoints.size() - 1) {
                surface.VF.emplace_back((ci + 1) * steps + i, ci * steps + i1, ci * steps + i);
                surface.VF.emplace_back((ci + 1) * steps + i, (ci + 1) * steps + i1, ci * steps + i1);
            }
        }
    }

    double scaler = 1/cos(2*pi/steps) + DELTA;
    hittable_list tris_in, tris_out;
    for (unsigned i = 0; i < surface.VF.size(); i++) {
        tris_in.add(make_shared<triangle>(surface.VV[std::get<0>(surface.VF[i])],
                                          surface.VV[std::get<1>(surface.VF[i])],
                                          surface.VV[std::get<2>(surface.VF[i])],
                                          surface.VN[std::get<0>(surface.VF[i])],
                                          surface.VN[std::get<1>(surface.VF[i])],
                                          surface.VN[std::get<2>(surface.VF[i])],
                                          surface.VT[std::get<0>(surface.VF[i])],
                                          surface.VT[std::get<1>(surface.VF[i])],
                                          surface.VT[std::get<2>(surface.VF[i])],
                                          mat_ptr, false, true));
        tris_out.add(make_shared<triangle>(surface.VV[std::get<0>(surface.VF[i])]*scaler,
                                           surface.VV[std::get<1>(surface.VF[i])]*scaler,
                                           surface.VV[std::get<2>(surface.VF[i])]*scaler,
                                           surface.VN[std::get<0>(surface.VF[i])],
                                           surface.VN[std::get<1>(surface.VF[i])],
                                           surface.VN[std::get<2>(surface.VF[i])],
                                           surface.VT[std::get<0>(surface.VF[i])],
                                           surface.VT[std::get<1>(surface.VF[i])],
                                           surface.VT[std::get<2>(surface.VF[i])],
                                           mat_ptr, false, true));
    }
    in_mesh = make_shared<mesh>(tris_in);
    out_mesh = make_shared<mesh>(tris_out);
}

bool surface_rev::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    if (use_mesh) {
        return in_mesh->hit(r, t_min, t_max, rec);
    }

    hit_record rec_in, rec_out;
    bool hit_in = in_mesh->hit(r, t_min, t_max, rec_in);
    // if (hit_in && (rec_in.v > 1-DELTA || rec_in.v < DELTA)) hit_in = false;
    bool hit_out = out_mesh->hit(r, t_min, t_max, rec_out);
    // if (hit_out && (rec_out.v > 1-DELTA || rec_out.v < DELTA)) hit_out = false;

    if (!hit_in && !hit_out) return false;

    bool do_next = /* hit_in && hit_out */ true;
    double next_t_min = t_min; // use this to find another hit point of the surface

    double t_cv_in  = rec_in.v;
    double t_r_in   = rec_in.t;
    double t_cv_out = rec_out.v;
    double t_r_out  = rec_out.t;
    if (hit_in) {
        next_t_min = fmax(next_t_min, t_r_in);
        hit_in = cv->hit_if_rec(r, t_cv_in, t_r_in, 10.0/resolution);
        if (t_r_in < t_min | t_r_in > t_max) hit_in = false;
    }
    if (hit_out) {
        next_t_min = fmax(next_t_min, t_r_out);
        hit_out = cv->hit_if_rec(r, t_cv_out, t_r_out, 10.0/resolution);
        if (t_r_out < t_min | t_r_out > t_max) hit_out = false;
    }

    double t_r, t_cv;
    if (!hit_in && !hit_out) {
        if (do_next) {
            return hit(r, next_t_min+DELTA, t_max, rec); // find another hit point
        } else {
            return false;
        }
    }
    if (hit_in && !hit_out) {
        t_r = t_r_in;
        t_cv = t_cv_in;
    }
    if (!hit_in && hit_out) {
        t_r = t_r_out;
        t_cv = t_cv_out;
    }
    if (hit_in && hit_out) {
        if (t_r_in < t_r_out) {
            t_r = t_r_in;
            t_cv = t_cv_in;
        } else {
            t_r = t_r_out;
            t_cv = t_cv_out;
        }
    }

    auto hit_point_cv = cv->getPoint(t_cv);
    auto d = hit_point_cv[0];
    auto y = hit_point_cv[1];
    auto hit_dir_cv = cv->getDir(t_cv);

    rec.t = t_r;
    rec.p = r.at(rec.t);
    auto phi = (d <= 0) ? (atan2(-rec.p.z(), rec.p.x()) + pi) : (atan2(rec.p.z(), -rec.p.x()) + pi);
    auto outward_normal = unit_vector(transformDirection(mat4::rotateY(180*phi/pi), cross(hit_dir_cv,vec3(0,0,1))));
    rec.set_face_normal(r, outward_normal);
    rec.u = phi/(2*pi);
    rec.v = t_cv;
    rec.mat_ptr = mat_ptr;

    return true;
}

bool surface_rev::bounding_box(double time0, double time1, aabb& output_box) const {
    if (use_mesh) {
        return in_mesh->bounding_box(time0, time1, output_box);
    } else {
        return out_mesh->bounding_box(time0, time1, output_box);
    }
}


    // auto rx = r.origin()[0];
    // auto ry = r.origin()[1];
    // auto rz = r.origin()[2];
    // auto dx = r.direction()[0];
    // auto dy = r.direction()[1];
    // auto dz = r.direction()[2];

    // double t_r;
    // if (!near_zero(dy)) {
    //     t_r = (y-ry)/dy;
    // }
    // if (near_zero(dy) || !near_zero((rx+dx*t_r)*(rx+dx*t_r) + (rz+dz*t_r)*(rz+dz*t_r) - d*d)) {
    //     auto a = dx*dx+dz*dz;
    //     auto b = rx*dx+rz*dz;
    //     auto c = rx*rx+rz*rz-d*d;
    //     auto del2 = b*b-a*c;
    //     if (del2 < 0) return false;
    //     auto del = sqrt(del2);
    //     t_r = (-b-del)/a;
    //     if (t_r < t_min) {t_r = (-b+del)/a;}
    // }
    // if (!near_zero(ry+dy*t_r-y)) return false;

#endif