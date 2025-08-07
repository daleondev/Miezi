module;
#include "mz/core/core.h"
// #include <../../../external/glm/glm/gtc/matrix_transform.hpp>
// #include <../../../external/glm/glm/gtc/matrix_inverse.hpp>
// #include <../../../external/glm/glm/gtx/euler_angles.hpp>
// #include <../../../external/glm/glm/gtx/string_cast.hpp>
// #include <../../../external/glm/glm/gtx/quaternion.hpp>
// #include <../../../external/glm/glm/gtc/type_ptr.hpp>
// #include <../../../external/glm/glm/gtc/epsilon.hpp>
// #include <../../../external/glm/glm/gtc/random.hpp>
// #include <../../../external/glm/glm/glm.hpp>
export module mz.util.geometry;

import std;
import glm;

import mz.core.logging;

namespace mz {

    //------------------------------------------------------
    //                      Vector
    //------------------------------------------------------

    template <typename T>
    concept VectorType = std::same_as<T, glm::vec2> || std::same_as<T, glm::vec3> || std::same_as<T, glm::vec4>;

    export template<VectorType T>
    class Vec
    {
    public:
        using Scalar = typename T::value_type;
        static constexpr std::size_t Size = T::length();

        Vec() = default;
        Vec(const Vec& other) : m_vec{ other.m_vec } {}
        Vec(Vec&& other) : m_vec{ std::move(other.m_vec) } { }
        Vec(const T& vec) : m_vec{ vec } {}
        Vec(T&& vec) : m_vec{ std::move(vec) } { }
        Vec(const Scalar scalar) : m_vec{ scalar } {}

        float length() const { return glm::length(m_vec); }

        Vec normalized() const { return glm::normalize(m_vec); }
        Vec& normalize()
        { 
            m_vec = glm::normalize(m_vec); 
            return *this;
        }

        float dot(const Vec& other) const { return glm::dot(m_vec, other.m_vec); }
        Vec cross(const Vec& other) const requires (Size < 4) { return glm::cross(m_vec, other.m_vec); }
        Vec cross(const Vec& B, const Vec& C) const requires (Size == 4)
        { 
            //              --                           --
            //              | (-1)^0 * DET[A^0, B^0, C^0] |
            // X(A, B, C) = | (-1)^1 * DET[A^1, B^1, C^1] |
            //              | (-1)^2 * DET[A^0, B^2, C^2] |
            //              | (-1)^3 * DET[A^3, B^3, C^3] |
            //              --                           --
            const Vec& A = *this;
            return Vec({
                +glm::determinant(glm::mat3( {A[1], B[1], C[1]}, {A[2], B[2], C[2]}, {A[3], B[3], C[3]} )),
                -glm::determinant(glm::mat3( {A[0], B[0], C[0]}, {A[2], B[2], C[2]}, {A[3], B[3], C[3]} )),
                +glm::determinant(glm::mat3( {A[0], B[0], C[0]}, {A[1], B[1], C[1]}, {A[3], B[3], C[3]} )),
                -glm::determinant(glm::mat3( {A[0], B[0], C[0]}, {A[1], B[1], C[1]}, {A[2], B[2], C[2]} ))
            });
        }

        Vec operator+() const { return +m_vec; }	
        Vec operator-() const { return -m_vec; }

        Vec operator+(const Vec& other) const { return m_vec + other.m_vec; }	
        Vec operator-(const Vec& other) const { return m_vec - other.m_vec; }	
        Vec operator*(const Vec& other) const { return m_vec * other.m_vec; }	
        Vec operator/(const Vec& other) const { return m_vec / other.m_vec; }
        Vec operator%(const Vec& other) const { return cross(other); }

        friend Vec operator+(const Vec& vec, const Scalar scalar) { return vec.m_vec + scalar; }	
        friend Vec operator-(const Vec& vec, const Scalar scalar) { return vec.m_vec - scalar; }	
        friend Vec operator*(const Vec& vec, const Scalar scalar) { return vec.m_vec * scalar; }	
        friend Vec operator/(const Vec& vec, const Scalar scalar) { return vec.m_vec / scalar; }
        friend Vec operator+(const Scalar scalar, const Vec& vec) { return scalar + vec.m_vec; }	
        friend Vec operator-(const Scalar scalar, const Vec& vec) { return scalar - vec.m_vec; }	
        friend Vec operator*(const Scalar scalar, const Vec& vec) { return scalar * vec.m_vec; }	
        friend Vec operator/(const Scalar scalar, const Vec& vec) { return scalar / vec.m_vec; }

        Scalar& operator[](const std::size_t i) { return m_vec[i]; }
        const Scalar& operator[](const std::size_t i) const { return m_vec[i]; }
        
        operator T&() { return m_vec; }
        operator const T&() const { return m_vec; }

        const T& operator()() const { return m_vec; }

        std::string toString() const { return glm::to_string(m_vec); }
        void print() const { MZ_TRACE("{}", toString()); }

        const Scalar* data() { return glm::value_ptr(m_vec); }

    private:
        T m_vec;

    };

    export using Vec2 = Vec<glm::vec2>;
    export using Vec3 = Vec<glm::vec3>;
    export using Vec4 = Vec<glm::vec4>;

    //------------------------------------------------------
    //                      Matrix
    //------------------------------------------------------

    template <typename T>
    concept MatrixType = std::same_as<T, glm::mat2> || std::same_as<T, glm::mat3> || std::same_as<T, glm::mat4>;

    export template<MatrixType T = glm::mat4>
    class Mat
    {
    public:
        using Scalar = typename T::value_type;
        static constexpr std::size_t Size = T::length();

        Mat() = default;
        Mat(const Mat& other) : m_mat{ other.m_mat } {}
        Mat(Mat&& other) : m_mat{ std::move(other.m_mat) } { }
        Mat(const T& mat) : m_mat{ mat } {}
        Mat(T&& mat) : m_mat{ std::move(mat) } { }
        Mat(const Scalar scalar) : m_mat{ scalar } {}

        Vec<glm::vec<Size, Scalar>> xAxis() const { return m_mat[0]; }
        Vec<glm::vec<Size, Scalar>> yAxis() const requires (Size > 2) { return m_mat[1]; }
        Vec<glm::vec<Size, Scalar>> zAxis() const requires (Size > 3) { return m_mat[2]; }
        Vec<glm::vec<Size, Scalar>> translation() const { return m_mat[Size-1]; }

        const Scalar* data() { return glm::value_ptr(m_mat); }

    private:
        T m_mat;

    };

    export using Mat2 = Mat<glm::mat2>;
    export using Mat3 = Mat<glm::mat3>;
    export using Mat4 = Mat<glm::mat4>;

}