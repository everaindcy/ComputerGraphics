#ifndef MAT3_H
#define MAT3_H

#include <cmath>
#include <iostream>

#include "../util.h"
#include "vec3.h"

using std::sqrt;

class mat3 {
public:
    mat3() : e{0,0,0, 0,0,0, 0,0,0} {}
    mat3(double m00, double m01, double m02,
	     double m10, double m11, double m12,
		 double m20, double m21, double m22)
        : e{m00, m01, m02, m10, m11, m12, m20, m21, m22}
        {}
    mat3(vec3 v0, vec3 v1, vec3 v2, bool isCol = true) {
        e[0]  = v0[0]; e[1]  = v1[0]; e[2]  = v2[0];
        e[3]  = v0[1]; e[4]  = v1[1]; e[5]  = v2[1];
        e[6]  = v0[2]; e[7]  = v1[2]; e[8] = v2[2];
        if (!isCol) this->transpose();
    }

    vec3 getRow(int idx) const {
        return vec3(e[3*idx+0], e[3*idx+1], e[3*idx+2]);
    }
    vec3 getCol(int idx) const {
        return vec3(e[idx+0], e[idx+3], e[idx+6]);
    }
    
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }
    double operator()(int i, int j) const { return e[i*4+j]; }
    double& operator()(int i, int j) { return e[i*4+j]; }
    
    void transpose();
    mat3 transposed() const;

    double determinant() const;

public:
    double e[9];
};

void mat3::transpose() {
    for(int i = 0; i < 2; i++) {
        for(int j = i+1; j < 3; j++) {
            std::swap(e[i*3+j], e[j*3+i]);
        }
    }
}

inline mat3 mat3::transposed() const {
    return mat3(getRow(0), getRow(1), getRow(2));
}

double mat3::determinant() const {
    double m00 = e[0];
	double m01 = e[1];
	double m02 = e[2];

	double m10 = e[3];
	double m11 = e[4];
	double m12 = e[5];

	double m20 = e[6];
	double m21 = e[7];
	double m22 = e[8];
    return m00*m11*m22 + m10*m21*m02 + m20*m01*m12 - m02*m11*m20 - m01*m10*m22 - m00*m12*m21;
}

#endif