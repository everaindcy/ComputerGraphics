#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
using std::string;

#include "hittable.h"
#include "triangle.h"
#include "bvh.h"

class mesh : public hittable {
public:
    mesh() {}
    mesh(const char* filename, shared_ptr<material> m)
        : mp(m) {
        create_mesh(filename);
    }

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(
        double time0, double time1, aabb& output_box) const override;

public:
    shared_ptr<bvh_node> triangles;
    shared_ptr<material> mp;

private:
    void create_mesh(const char*);
};

bool mesh::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    return triangles->hit(r, t_min, t_max, rec);
}

bool mesh::bounding_box(double time0, double time1, aabb& output_box) const {
    return triangles->bounding_box(time0, time1, output_box);
}

void mesh::create_mesh(const char* filename) {
    auto f = fopen(filename, "r");
    if(f == NULL) {
        std::cerr << "Error: failed to open " << filename << "\n" << std::flush;
        return; 
    }

    std::vector<point3> points;

    char token[10];
    if(EOF == fscanf(f, "%s", token)) {
        std::cerr << "Error: meet EOF at inappropriately time\n" << std::flush;
        return;
    }
    while(token[0] == 'v') {
        double a, b, c;
        if(EOF == fscanf(f, "%lf%lf%lf", &a, &b, &c)) {
            std::cerr << "Error: meet EOF at inappropriately time\n" << std::flush;
            return;
        }
        points.push_back(point3(a, b, c));

        if(EOF == fscanf(f, "%s", token)) {
            std::cerr << "Error: meet EOF at inappropriately time\n" << std::flush;
            return;
        }
    }

    hittable_list tris;

    while(token[0] == 'f') {
        int a, b, c;
        if(EOF == fscanf(f, "%d%d%d", &a, &b, &c)) {
            std::cerr << "Error: meet EOF at inappropriately time\n" << std::flush;
            return;
        }
        tris.add(make_shared<triangle>(points[a-1], points[b-1], points[c-1], mp));

        if(EOF == fscanf(f, "%s", token)) {
            break;
        }
    }

    triangles = make_shared<bvh_node>(tris, 0,0);
}

#endif