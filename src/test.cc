#include "util/curve/bezier.h"

int main() {
    double a[100] = {-2, -4, 0, -2, -3, -5};
    double b[100] = {2, 0, 0, -2, -5, -3};

    bezierCurve2D cu(a, b, 6);

    std::vector<curvePoint> cp;
    cu.discretize(10, cp);
}