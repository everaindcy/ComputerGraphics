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
    mesh(const hittable_list& triangles) {
        this->triangles = make_shared<bvh_node>(triangles, 0,0);
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

class smoothMesh : public hittable {
public:
    smoothMesh() {}
    smoothMesh(const char* filename, shared_ptr<material> m)
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

bool smoothMesh::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    return triangles->hit(r, t_min, t_max, rec);
}

bool smoothMesh::bounding_box(double time0, double time1, aabb& output_box) const {
    return triangles->bounding_box(time0, time1, output_box);
}

void smoothMesh::create_mesh(const char* filename) {
    auto f = fopen(filename, "r");
    if(f == NULL) {
        std::cerr << "Error: failed to open " << filename << "\n" << std::flush;
        return; 
    }

    std::vector<point3> points;
    std::vector<vec3> normals;

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
        normals.push_back(vec3(0,0,0));

        if(EOF == fscanf(f, "%s", token)) {
            std::cerr << "Error: meet EOF at inappropriately time\n" << std::flush;
            return;
        }
    }

    std::vector<int> as, bs, cs;

    while(token[0] == 'f') {
        int a, b, c;
        if(EOF == fscanf(f, "%d%d%d", &a, &b, &c)) {
            std::cerr << "Error: meet EOF at inappropriately time\n" << std::flush;
            return;
        }

        as.push_back(a-1);
        bs.push_back(b-1);
        cs.push_back(c-1);
        
        vec3 this_normal = unit_vector(cross(points[b-1]-points[a-1],points[c-1]-points[a-1]));
        normals[a-1] += this_normal;
        normals[b-1] += this_normal;
        normals[c-1] += this_normal;

        if(EOF == fscanf(f, "%s", token)) {
            break;
        }
    }

    hittable_list tris;

    for(int i = 0; i < as.size(); i++) {
        tris.add(make_shared<smoothTriangle>(points[as[i]], points[bs[i]], points[cs[i]],
            unit_vector(normals[as[i]]), unit_vector(normals[bs[i]]), unit_vector(normals[cs[i]]), mp));
    }

    triangles = make_shared<bvh_node>(tris, 0,0);
}

#endif