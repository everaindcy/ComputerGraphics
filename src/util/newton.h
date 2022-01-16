#ifndef NEWTON_H
#define NEWTON_H

bool newton(double &ans, std::function<double(double)> f, std::function<double(double)> df, int depth = 10, double eps = 1e-8) {
    for (int i = 0; i < depth; i++) {
        double fx = f(ans);
        if (fx < eps && fx > -eps) {
            return true;
        }
        double dfx = df(ans);
        if (fabs(dfx) < eps) {
            return false;
        }
        ans -= fx/dfx;
    }
    return false;
}

bool newton2x2(double &x, double &y,
        std::function<double(double, double)> f,
        std::function<double(double, double)> g,
        std::function<double(double, double)> dfx,
        std::function<double(double, double)> dfy,
        std::function<double(double, double)> dgx,
        std::function<double(double, double)> dgy,
        int depth = 30, double eps = 1e-8
    ) {
    for (int i = 0; i < depth; i++) {
        double fxy = f(x, y);
        double gxy = g(x, y);
        if (fabs(fxy) < eps && fabs(gxy) < eps) {
            return true;
        }
        double dfxxy = dfx(x, y);
        double dfyxy = dfy(x, y);
        double dgxxy = dgx(x, y);
        double dgyxy = dgy(x, y);
        double det = dfxxy*dgyxy - dfyxy*dgxxy;
        if (fabs(det) < eps) {
            return false;
        }
        double a =  dgyxy/det;
        double b = -dfyxy/det;
        double c = -dgxxy/det;
        double d =  dfxxy/det;
        x -= a*fxy + b*gxy;
        y -= c*fxy + d*gxy;
    }
    return false;
}

#endif