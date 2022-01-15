#ifndef NEWTON_H
#define NEWTON_H

bool newton(double &ans, std::function<double(double)> f, std::function<double(double)> df, int depth = 10, double eps = 1e-8) {
    for (int i = 0; i < depth; i++) {
        double fx = f(ans);
        if (fx < eps && fx > -eps) {
            return true;
        }
        double dfx = df(ans);
        // if (dfx < eps && dfx > -eps) {
        //     return false;
        // }
        ans -= fx/dfx;
    }
    return false;
}

#endif