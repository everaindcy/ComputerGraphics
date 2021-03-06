#ifndef CURVE_H
#define CURVE_H

#include "../../util/utils.h"

#include <vector>
#include <utility>

#include <algorithm>

// out side of a curve is at right of the curve

struct curvePoint {
    point3 v;
    vec3 dir; // unit
    double t;
};

class curve {
public:
    virtual point3 getPoint(double t) const = 0;
    virtual vec3 getDir(double t) const = 0;
    virtual void discretize(int resolution, std::vector<curvePoint>& data) = 0;
    virtual bool hit_if_rec(const ray &r, double &t, double &t1, double max_dt) = 0;
};


#endif