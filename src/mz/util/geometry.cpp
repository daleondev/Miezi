module;
#include "mz/core/core.h"
module mz.util.geometry;

import std;
import glm;

import mz.core.logging;

namespace mz {

    template<VectorType T>
    Mat3 Vec<T>::skewSymmetricCross_alt() const requires (Size == 3)
    {
        const Vec3& v = *this;
        return {
            {  0.0f, v[2],-v[1] },
            { -v[2], 0.0f, v[0] },
            {  v[1],-v[0], 0.0f }
        };
    }
    template Mat3 Vec3::skewSymmetricCross_alt() const;

    template<VectorType T>
    Mat3 Vec<T>::rotationTo_alt(const Vec3& vec) const requires (Size == 3)
    {
        const Vec3 v = cross(vec);
        const Scalar c = dot(vec);

        const auto vx = v.skewSymmetricCross_alt();
        const auto r = Mat3(1.0f) + vx + vx*vx * (1/(1+c));
        return r;
    }
    template Mat3 Vec3::rotationTo_alt(const Vec3& vec) const;

    template<VectorType T>
    Mat3 Vec<T>::rotationTo(const Vec3& vec) const requires (Size == 3)
    { 
        return glm::toMat3(glm::rotation(*this, vec)); 
    }
    template Mat3 Vec3::rotationTo(const Vec3& vec) const;

    template<VectorType T>
    Vec3 Vec<T>::projected(const Vec3& obj, const Mat4 model, const Mat4 proj, const Vec4 viewPort) const requires (Size == 3)
    {
        return glm::project(obj, model, proj, viewPort);
    }

    template<VectorType T>
    Vec3& Vec<T>::project(const Vec3& obj, const Mat4 model, const Mat4 proj, const Vec4 viewPort) requires (Size == 3)
    {
        return (*this = glm::project(obj, model, proj, viewPort));
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
    template Vec4 Vec4::cross(const Vec4& B, const Vec4& C) const;

    template<VectorType T>
    Vec<T>::Scalar Vec<T>::maxValue() const 
    {
        constexpr std::size_t n = Size == 4 ? 3 : Size;
        Scalar maxVal = std::numeric_limits<Scalar>::lowest();
        for (std::size_t i = 0; i < n; ++i) maxVal = glm::max(maxVal, (*this)[i]);
        return maxVal;
    }
    template Vec2::Scalar Vec2::maxValue() const;
    template Vec3::Scalar Vec3::maxValue() const;
    template Vec4::Scalar Vec4::maxValue() const;

    template<VectorType T>
    Vec<T>::Scalar Vec<T>::minValue() const 
    {
        constexpr std::size_t n = Size == 4 ? 3 : Size;
        Scalar minVal = std::numeric_limits<Scalar>::max();
        for (std::size_t i = 0; i < n; ++i) minVal = glm::min(minVal, (*this)[i]);
        return minVal;
    }
    template Vec2::Scalar Vec2::minValue() const;
    template Vec3::Scalar Vec3::minValue() const;
    template Vec4::Scalar Vec4::minValue() const;

    template<VectorType T>
    std::string Vec<T>::toPrettyString(const bool printType) const
    { 
        std::stringstream ss;
        if (printType) ss << "vec" << Size << ' ';
        ss << '(';
        for (std::size_t i = 0; i < Size; ++i)
            ss << std::format("{:8.4f}{}", (*this)[i], (i == Size-1 ? " " : ", "));
        ss << ')';
        return ss.str();
    }
    template std::string Vec2::toPrettyString(const bool printType) const;
    template std::string Vec3::toPrettyString(const bool printType) const;
    template std::string Vec4::toPrettyString(const bool printType) const;

}

