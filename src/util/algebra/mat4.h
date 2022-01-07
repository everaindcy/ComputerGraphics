#ifndef MAT4_H
#define MAT4_H

#include <cmath>
#include <iostream>

#include "../util.h"
#include "vec3.h"
#include "vec4.h"
#include "mat3.h"

using std::sqrt;

class mat4 {
public:
    mat4() : e{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0} {}
    mat4(float m00, float m01, float m02, float m03,
	     float m10, float m11, float m12, float m13,
		 float m20, float m21, float m22, float m23,
		 float m30, float m31, float m32, float m33)
        : e{m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33}
        {}
    mat4(vec4 v0, vec4 v1, vec4 v2, vec4 v3, bool isCol = true) {
        e[0]  = v0[0]; e[1]  = v1[0]; e[2]  = v2[0]; e[3]  = v3[0];
        e[4]  = v0[1]; e[5]  = v1[1]; e[6]  = v2[1]; e[7]  = v3[1];
        e[8]  = v0[2]; e[9]  = v1[2]; e[10] = v2[2]; e[11] = v3[2];
        e[12] = v0[3]; e[13] = v1[3]; e[14] = v2[3]; e[15] = v3[3];
        if (!isCol) this->transpose();
    }

    vec4 getRow(int idx) const {
        return vec4(e[idx+0], e[idx+1], e[idx+2], e[idx+3]);
    }
    vec4 getCol(int idx) const {
        return vec4(e[idx+0], e[idx+4], e[idx+8], e[idx+12]);
    }
    
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }
    double operator()(int i, int j) const { return e[i*4+j]; }
    double& operator()(int i, int j) { return e[i*4+j]; }

    void transpose();
    mat4 transposed() const;

    mat4 inverse() const;

public:
    double e[16];
};

void mat4::transpose() {
    for(int i = 0; i < 3; i++) {
        for(int j = i+1; j < 4; j++) {
            std::swap(e[i*4+j], e[j*4+i]);
        }
    }
}

inline mat4 mat4::transposed() const {
    return mat4(getRow(0), getRow(1), getRow(2), getRow(3));
}

inline vec4 operator*(const mat4 mat, const vec4 vec) {
    return vec4(dot(mat.getRow(0),vec), dot(mat.getRow(3),vec), dot(mat.getRow(3),vec), dot(mat.getRow(3),vec));
}

#endif