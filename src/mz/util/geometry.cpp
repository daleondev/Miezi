module;
#include "mz/core/core.h"
module mz.util.geometry;

import std;
import glm;

import mz.core.logging;

namespace mz {

    template<VectorType T>
    Mat3 Vec<T>::skewSymmetricCross() const requires (Size == 3)
    {
        const Vec3& v = *this;
        return {
            {  0.0f, v[2],-v[1] },
            { -v[2], 0.0f, v[0] },
            {  v[1],-v[0], 0.0f }
        };
    }
    template Mat3 Vec3::skewSymmetricCross() const requires (Size == 3);

    template<VectorType T>
    Mat3 Vec<T>::rotationTo(const Vec3& vec) const requires (Size == 3)
    { 
        return glm::toMat3(glm::rotation(**this, vec)); 
    }
    template Mat3 Vec3::rotationTo(const Vec3& vec) const requires (Size == 3);

    template<VectorType T>
    Vec3 Vec<T>::projected(const Mat4 model, const Mat4 proj, const Vec4 viewPort) const requires (Size == 3)
    {
        return glm::project(**this, model, proj, viewPort);
    }

    template<VectorType T>
    Vec3& Vec<T>::project(const Mat4 model, const Mat4 proj, const Vec4 viewPort) requires (Size == 3)
    {
        return (*this = glm::project(**this, model, proj, viewPort));
    }

    template<VectorType T>
    Vec4 Vec<T>::cross(const Vec4& B, const Vec4& C) const requires (Size == 4)
    { 
        //              --                           --
        //              | (-1)^0 * DET[A^0, B^0, C^0] |
        // X(A, B, C) = | (-1)^1 * DET[A^1, B^1, C^1] |
        //              | (-1)^2 * DET[A^0, B^2, C^2] |
        //              | (-1)^3 * DET[A^3, B^3, C^3] |
        //              --                           --
        const Vec4& A = *this;
        return {
            +glm::determinant(glm::mat3( {A[1], B[1], C[1]}, {A[2], B[2], C[2]}, {A[3], B[3], C[3]} )),
            -glm::determinant(glm::mat3( {A[0], B[0], C[0]}, {A[2], B[2], C[2]}, {A[3], B[3], C[3]} )),
            +glm::determinant(glm::mat3( {A[0], B[0], C[0]}, {A[1], B[1], C[1]}, {A[3], B[3], C[3]} )),
            -glm::determinant(glm::mat3( {A[0], B[0], C[0]}, {A[1], B[1], C[1]}, {A[2], B[2], C[2]} ))
        };
    }
    template Vec4 Vec4::cross(const Vec4& B, const Vec4& C) const requires (Size == 4);

}

//template<VectorType T>
//Mat3 Vec<T>::rotationTo_alt(const Vec3& vec) const requires (Size == 3)
//{
//    const Vec3 v = cross(vec);
//    const Scalar c = dot(vec);

//    const auto vx = v.skewSymmetricCross_alt();
//    const auto r = Mat3(1.0f) + vx + vx*vx * (1/(1+c));
//    return r;
//}
//template Mat3 Vec3::rotationTo_alt(const Vec3& vec) const requires (Size == 3);