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
    mat4(double m00, double m01, double m02, double m03,
	     double m10, double m11, double m12, double m13,
		 double m20, double m21, double m22, double m23,
		 double m30, double m31, double m32, double m33)
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
        return vec4(e[4*idx+0], e[4*idx+1], e[4*idx+2], e[4*idx+3]);
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

    mat4 inverse(bool* pbIsSingular = NULL, double epsilon = 0.0) const;

public:
    double e[16];

public:
    inline static mat4 identity();
	inline static mat4 translation(double x, double y, double z);
	inline static mat4 translation(const vec3& rTranslation);
	inline static mat4 rotateX(double degree);
	inline static mat4 rotateY(double degree);
	inline static mat4 rotateZ(double degree);
	inline static mat4 rotation(const vec3& rDirection, double degree);
	inline static mat4 scaling(double sx, double sy, double sz);
	inline static mat4 scaling(double s);
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

inline static double determinant3x3(double m00, double m01, double m02,
                                    double m10, double m11, double m12,
                                    double m20, double m21, double m22) {
    return m00*m11*m22 + m10*m21*m02 + m20*m01*m12 - m02*m11*m20 - m01*m10*m22 - m00*m12*m21;
}

mat4 mat4::inverse(bool* pbIsSingular, double epsilon) const {
	double m00 = e[ 0 ];
	double m01 = e[ 1 ];
	double m02 = e[ 2 ];
	double m03 = e[ 3 ];

	double m10 = e[ 4 ];
	double m11 = e[ 5 ];
	double m12 = e[ 6 ];
	double m13 = e[ 7 ];

	double m20 = e[ 8 ];
	double m21 = e[ 9 ];
	double m22 = e[ 10 ];
	double m23 = e[ 11 ];

	double m30 = e[ 12 ];
	double m31 = e[ 13 ];
	double m32 = e[ 14 ];
	double m33 = e[ 15 ];

    double cofactor00 =  determinant3x3( m11, m12, m13, m21, m22, m23, m31, m32, m33 );
    double cofactor01 = -determinant3x3( m12, m13, m10, m22, m23, m20, m32, m33, m30 );
    double cofactor02 =  determinant3x3( m13, m10, m11, m23, m20, m21, m33, m30, m31 );
    double cofactor03 = -determinant3x3( m10, m11, m12, m20, m21, m22, m30, m31, m32 );
    
    double cofactor10 = -determinant3x3( m21, m22, m23, m31, m32, m33, m01, m02, m03 );
    double cofactor11 =  determinant3x3( m22, m23, m20, m32, m33, m30, m02, m03, m00 );
    double cofactor12 = -determinant3x3( m23, m20, m21, m33, m30, m31, m03, m00, m01 );
    double cofactor13 =  determinant3x3( m20, m21, m22, m30, m31, m32, m00, m01, m02 );
    
    double cofactor20 =  determinant3x3( m31, m32, m33, m01, m02, m03, m11, m12, m13 );
    double cofactor21 = -determinant3x3( m32, m33, m30, m02, m03, m00, m12, m13, m10 );
    double cofactor22 =  determinant3x3( m33, m30, m31, m03, m00, m01, m13, m10, m11 );
    double cofactor23 = -determinant3x3( m30, m31, m32, m00, m01, m02, m10, m11, m12 );
    
    double cofactor30 = -determinant3x3( m01, m02, m03, m11, m12, m13, m21, m22, m23 );
    double cofactor31 =  determinant3x3( m02, m03, m00, m12, m13, m10, m22, m23, m20 );
    double cofactor32 = -determinant3x3( m03, m00, m01, m13, m10, m11, m23, m20, m21 );
    double cofactor33 =  determinant3x3( m00, m01, m02, m10, m11, m12, m20, m21, m22 );

	double determinant = m00 * cofactor00 + m01 * cofactor01 + m02 * cofactor02 + m03 * cofactor03;

	bool isSingular = ( fabs( determinant ) < epsilon );
	if( isSingular )
	{
		if( pbIsSingular != NULL )
		{
			*pbIsSingular = true;
		}
		return mat4();
	}
	else
	{
		if( pbIsSingular != NULL )
		{
			*pbIsSingular = false;
		}

		double reciprocalDeterminant = 1.0 / determinant;

		return mat4
			(
				cofactor00 * reciprocalDeterminant, cofactor10 * reciprocalDeterminant, cofactor20 * reciprocalDeterminant, cofactor30 * reciprocalDeterminant,
				cofactor01 * reciprocalDeterminant, cofactor11 * reciprocalDeterminant, cofactor21 * reciprocalDeterminant, cofactor31 * reciprocalDeterminant,
				cofactor02 * reciprocalDeterminant, cofactor12 * reciprocalDeterminant, cofactor22 * reciprocalDeterminant, cofactor32 * reciprocalDeterminant,
				cofactor03 * reciprocalDeterminant, cofactor13 * reciprocalDeterminant, cofactor23 * reciprocalDeterminant, cofactor33 * reciprocalDeterminant
			);
	}
}

mat4 mat4::identity() {
    return mat4(
        1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
    );
}

mat4 mat4::translation(double x, double y, double z) {
    return mat4(
        1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
    );
}

mat4 mat4::translation(const vec3& rTranslation) {
    return mat4(
        1, 0, 0, rTranslation.x(),
		0, 1, 0, rTranslation.y(),
		0, 0, 1, rTranslation.z(),
		0, 0, 0, 1
    );
}

mat4 mat4::rotateX(double degree) {
    double radians = degrees_to_radians(degree);
	double c = cos(radians);
	double s = sin(radians);
    return mat4(
        1, 0, 0, 0,
		0, c,-s, 0,
		0, s, c, 0,
		0, 0, 0, 1
    );
}

mat4 mat4::rotateY(double degree) {
    double radians = degrees_to_radians(degree);
	double c = cos(radians);
	double s = sin(radians);
    return mat4(
        c, 0, s, 0,
		0, 1, 0, 0,
	   -s, 0, c, 0,
		0, 0, 0, 1
    );
}

mat4 mat4::rotateZ(double degree) {
    double radians = degrees_to_radians(degree);
	double c = cos(radians);
	double s = sin(radians);
    return mat4(
        c,-s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
    );
}

mat4 mat4::rotation(const vec3& rDirection, double degree) {
    vec3 normalizedDirection = unit_vector(rDirection);
	double x = normalizedDirection.x();
	double y = normalizedDirection.y();
	double z = normalizedDirection.z();
	
    double radians = degrees_to_radians(degree);
	double cosTheta = cos(radians);
	double sinTheta = sin(radians);

	return mat4
	(
		x*x*(1.0-cosTheta) + cosTheta, y*x*(1.0-cosTheta)-z*sinTheta, z*x*(1.0-cosTheta)+y*sinTheta, 0.0,
		x*y*(1.0-cosTheta)+z*sinTheta, y*y*(1.0-cosTheta) + cosTheta, z*y*(1.0-cosTheta)-x*sinTheta, 0.0,
		x*z*(1.0-cosTheta)-y*sinTheta, y*z*(1.0-cosTheta)+x*sinTheta, z*z*(1.0-cosTheta) + cosTheta, 0.0,
		0.0,                           0.0,                           0.0,                           1.0
	);
}

mat4 mat4::scaling(double sx, double sy, double sz) {
    return mat4(
       sx,  0,  0,  0,
	    0, sy,  0,  0,
	    0,  0, sz,  0,
	    0,  0,  0,  1
    );
}

mat4 mat4::scaling(double s) {
    return mat4(
        s, 0, 0, 0,
		0, s, 0, 0,
		0, 0, s, 0,
		0, 0, 0, 1
    );
}

inline vec4 operator*(const mat4 mat, const vec4 vec) {
    return vec4(dot(mat.getRow(0),vec), dot(mat.getRow(1),vec), dot(mat.getRow(2),vec), dot(mat.getRow(3),vec));
}

inline mat4 operator*(const mat4 u, const mat4 v) {
    return mat4(
        dot(u.getRow(0),v.getCol(0)), dot(u.getRow(0),v.getCol(1)), dot(u.getRow(0),v.getCol(2)), dot(u.getRow(0),v.getCol(3)),
        dot(u.getRow(1),v.getCol(0)), dot(u.getRow(1),v.getCol(1)), dot(u.getRow(1),v.getCol(2)), dot(u.getRow(1),v.getCol(3)),
        dot(u.getRow(2),v.getCol(0)), dot(u.getRow(2),v.getCol(1)), dot(u.getRow(2),v.getCol(2)), dot(u.getRow(2),v.getCol(3)),
        dot(u.getRow(3),v.getCol(0)), dot(u.getRow(3),v.getCol(1)), dot(u.getRow(3),v.getCol(2)), dot(u.getRow(3),v.getCol(3))
    );
}

// transforms a 3D point using a matrix, returning a 3D point
vec3 transformPoint(const mat4 &mat, const vec3 &point) {
    return (mat * vec4(point, 1)).xyz();
}

// transform a 3D directino using a matrix, returning a direction
vec3 transformDirection(const mat4 &mat, const vec3 &dir) {
    return (mat * vec4(dir, 0)).xyz();
}

#endif