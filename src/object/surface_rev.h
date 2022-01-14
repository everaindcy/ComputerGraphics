#ifndef SURFACE_REV_H
#define SURFACE_REV_H

#define Resolution 100

#include "hittable.h"
#include "mesh.h"
#include "../util/curve/curve.h"

class surface_rev : public hittable {
public:
    surface_rev() {}
    surface_rev(shared_ptr<curve> c, shared_ptr<material> m)
        : cv(c), mat_ptr(m) {gen_mesh();};

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(
        double time0, double time1, aabb& output_box) const override;

public:
    shared_ptr<curve> cv;
    shared_ptr<material> mat_ptr;
    
    shared_ptr<hittable> surface_mesh;

private:
    void gen_mesh();
};

void surface_rev::gen_mesh() {
    // Definition for drawable surface.
    typedef std::tuple<unsigned, unsigned, unsigned> Tup3u;
    // Surface is just a struct that contains vertices, normals, and
    // faces.  VV[i] is the position of vertex i, and VN[i] is the normal
    // of vertex i.  A face is a triple i,j,k corresponding to a triangle
    // with (vertex i, normal i), (vertex j, normal j), ...
    // Currently this struct is computed every time when canvas refreshes.
    // You can store this as member function to accelerate rendering.
    struct Surface {
        std::vector<point3> VV;
        std::vector<vec3> VN;
        std::vector<Tup3u> VF;
    } surface;

    std::vector<curvePoint> curvePoints;
    cv->discretize(Resolution, curvePoints);

    const int steps = Resolution/2;
    for (unsigned int ci = 0; ci < curvePoints.size(); ++ci) {
        const curvePoint &cp = curvePoints[ci];
        for (unsigned int i = 0; i < steps; ++i) {
            double t = (double)i / steps;
            double degree = 360 * t;
            point3 pnew = transformPoint(mat4::rotateY(degree), cp.v);
            vec3 pNormal = cross(cp.dir, vec3(0,0,1));
            vec3 nnew = transformDirection(mat4::rotateY(degree), pNormal);
            surface.VV.push_back(pnew);
            surface.VN.push_back(nnew);
            int i1 = (i + 1 == steps) ? 0 : i + 1;
            if (ci != curvePoints.size() - 1) {
                surface.VF.emplace_back((ci + 1) * steps + i, ci * steps + i1, ci * steps + i);
                surface.VF.emplace_back((ci + 1) * steps + i, (ci + 1) * steps + i1, ci * steps + i1);
            }
        }
    }

    hittable_list tris;
    for (unsigned i = 0; i < surface.VF.size(); i++) {
        // tris.add(make_shared<triangle>(surface.VV[std::get<0>(surface.VF[i])],
        //                                surface.VV[std::get<1>(surface.VF[i])],
        //                                surface.VV[std::get<2>(surface.VF[i])],
        //                                mat_ptr));
        tris.add(make_shared<triangle>(surface.VV[std::get<0>(surface.VF[i])],
                                       surface.VV[std::get<1>(surface.VF[i])],
                                       surface.VV[std::get<2>(surface.VF[i])],
                                       surface.VN[std::get<0>(surface.VF[i])],
                                       surface.VN[std::get<1>(surface.VF[i])],
                                       surface.VN[std::get<2>(surface.VF[i])],
                                       mat_ptr));
    }

    surface_mesh = make_shared<mesh>(tris);
}

bool surface_rev::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    return surface_mesh->hit(r, t_min, t_max, rec);
}

bool surface_rev::bounding_box(double time0, double time1, aabb& output_box) const {
    return surface_mesh->bounding_box(time0, time1, output_box);
}

#endif