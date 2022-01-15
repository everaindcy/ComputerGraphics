#ifndef BEZIER_H
#define BEZIER_H

#include "curve.h"
#include "../newton.h"

#include <vector>

class bezierCurve2D : public curve {
public:
    bezierCurve2D() = default;
    bezierCurve2D(double* px, double* py, int n);
    bezierCurve2D(point3* p, int n);
    ~bezierCurve2D() {delete[] px; delete[] py; delete[] dx; delete[] dy;};

    virtual point3 getPoint(double t) const override;
    virtual vec3 getDir(double t) const override;
    virtual void discretize(int resolution, std::vector<curvePoint>& data) override;
    virtual bool hit_if_rec(const ray &r, double &t, double max_dt) override;

public:
    int n; // the num of points
    double *px, *py;
    double *dx, *dy;
    // curve(t) = (x(t), y(t))
    // x(t) = dx[0] + dx[1]*x + dx[2]*x^2 + ... + dx[n-1]*x^(n-1)
    // y(t) = dy[0] + dy[1]*y + dy[2]*y^2 + ... + dy[n-1]*y^(n-1)

private:
    vec3 P(int i, int k, float t) {
        if (k == 0) {
            return point3(px[i], py[i], 0);
        }
        if (available[i][k]) {
            return dat[i][k];
        }
        available[i][k] = true;
        return dat[i][k] = (1-t)*P(i, k-1, t) + t*P(i+1, k-1, t);
    }
    std::vector<std::vector<bool> > available;
    std::vector<std::vector<vec3> > dat;
};

bezierCurve2D::bezierCurve2D(double* _px, double* _py, int _n) {
    n = _n;
    px = new double[n];
    py = new double[n];
    dx = new double[n];
    dy = new double[n];
    for (int i = 0; i < n; i++) {
        px[i] = _px[i];
        py[i] = _py[i];
    }

    for (int i = 0; i < n; i++) {
        dx[i] = px[0];
        dy[i] = py[0];
        for (int j = 0; j < n; j++) {
            px[j] = px[j+1] - px[j];
            py[j] = py[j+1] - py[j];
        }
    }
    double n_down = 1, fac = 1, nxt = n-1;
    for (int i = 0; i < n; i++, nxt--) {
        fac = fac * (i==0 ? 1 : i);
        dx[i] *= n_down/fac;
        dy[i] *= n_down/fac;
        n_down *= nxt;
    }

    for (int i = 0; i < n; i++) {
        px[i] = _px[i];
        py[i] = _py[i];
    }

}

bezierCurve2D::bezierCurve2D(point3* _p, int _n) {
    n = _n;
    px = new double[n];
    py = new double[n];
    dx = new double[n];
    dy = new double[n];
    for (int i = 0; i < n; i++) {
        px[i] = _p[i][0];
        py[i] = _p[i][1];
    }

    for (int i = 0; i < n; i++) {
        dx[i] = px[0];
        dy[i] = py[0];
        for (int j = 0; j < n; j++) {
            px[j] = px[j+1] - px[j];
            py[j] = py[j+1] - py[j];
        }
    }
    double n_down = 1, fac = 1, nxt = n-1;
    for (int i = 0; i < n; i++, nxt--) {
        fac = fac * (i==0 ? 1 : i);
        dx[i] *= n_down/fac;
        dy[i] *= n_down/fac;
        n_down *= nxt;
    }

    for (int i = 0; i < n; i++) {
        px[i] = _p[i][0];
        py[i] = _p[i][1];
    }

}

point3 bezierCurve2D::getPoint(double t) const {
    double x = 0, y = 0, t_power = 1;
    for (int i = 0; i < n; i++) {
        x += dx[i] * t_power;
        y += dy[i] * t_power;
        t_power *= t;
    }
    return point3(x, y, 0);
}

vec3 bezierCurve2D::getDir(double t) const {
    double x = 0, y = 0, t_power = 1;
    for(int i = 1; i < n; i++)
    {
        x += dx[i] * i * t_power;
        y += dy[i] * i * t_power;
        t_power *= t;
    }
    return vec3(x, y, 0);
}

void bezierCurve2D::discretize(int resolution, std::vector<curvePoint>& data) {
    data.clear();

    available.resize(n);
    for (auto &v : available) v.resize(n);
    dat.resize(n);
    for (auto &v : dat) v.resize(n);

    for (int i = 0; i <= resolution; i++) {

        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                available[j][k] = false;
            }
        }

        double t = (double)i / (double)resolution;
        data.push_back(curvePoint{P(0, n-1, t), unit_vector(P(1, n-2, t) - P(0, n-2, t)), t});
    }
}

// this t is on the curve
bool bezierCurve2D::hit_if_rec(const ray &r, double &t, double max_dt) {
    auto f = [=](double tt)->double {
        auto rx = r.origin()[0];
        auto ry = r.origin()[1];
        auto rz = r.origin()[2];
        auto dx = r.direction()[0];
        auto dy = r.direction()[1];
        auto dz = r.direction()[2];
        auto p = this->getPoint(tt);
        auto x = p[0];
        auto y = p[1];

        auto xt = rx + (dx/dy)*(y-ry);
        auto zt = rz + (dz/dy)*(y-ry);
        return xt*xt + zt*zt - x*x;
    };
    auto df = [=](double tt)->double {
        auto rx = r.origin()[0];
        auto ry = r.origin()[1];
        auto rz = r.origin()[2];
        auto dx = r.direction()[0];
        auto dy = r.direction()[1];
        auto dz = r.direction()[2];
        auto p = this->getPoint(tt);
        auto x = p[0];
        auto y = p[1];
        auto dir = this->getDir(tt);
        auto x1 = dir[0];
        auto y1 = dir[1];

        auto xt = rx + (dx/dy)*(y-ry);
        auto zt = rz + (dz/dy)*(y-ry);
        return 2*((dx/dy)*xt*y1 + (dz/dy)*zt*y1 - x*x1);
    };

    double t_origin = t;
    bool hitting = newton(t, f, df);
    if (abs(t-t_origin) > max_dt | t > 1 | t < 0) {
        return false;
    }
    return hitting;
}

#endif