#ifndef CURVE_H
#define CURVE_H

#include "../algebra/vec3.h"

#include <vector>
#include <utility>

#include <algorithm>

// out side of a curve is at right of the curve

struct curvePoint {
    point3 v;
    vec3 dir; // unit
};

class curve {
public:
    virtual point3 getPoint(double t) const = 0;
    virtual vec3 getDir(double t) const = 0;
    virtual void discretize(int resolution, std::vector<curvePoint>& data) = 0;
};


#endif