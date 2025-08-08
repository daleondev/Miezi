module;
#include "mz/core/core.h"
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

        Vec() : m_vec{ 0.0f } {  }
        Vec(const Vec& other) : m_vec{ other.m_vec } {}
        Vec(Vec&& other) : m_vec{ std::move(other.m_vec) } { }
        Vec(const T& vec) : m_vec{ vec } {}
        Vec(T&& vec) : m_vec{ std::move(vec) } { }
        Vec(const Scalar scalar) : m_vec{ scalar } {}

        Scalar maxValue() const 
        {
            constexpr std::size_t n = Size == 4 ? 3 : Size;
            Scalar maxVal = std::numeric_limits<Scalar>::lowest();
            for (std::size_t i = 0; i < n; ++i) maxVal = glm::max(maxVal, m_vec[i]);
            return maxVal;
        }

        Scalar minValue() const 
        {
            constexpr std::size_t n = Size == 4 ? 3 : Size;
            Scalar minVal = std::numeric_limits<Scalar>::max();
            for (std::size_t i = 0; i < n; ++i) minVal = glm::min(minVal, m_vec[i]);
            return minVal;
        }

        Vec& fillRandom() requires (Size == 2)
        {
            m_vec = glm::circularRand(1.0f);
            return *this;
        }
        Vec& fillRandom() requires (Size == 3)
        {
            m_vec = glm::sphericalRand(1.0f);
            return *this;
        }
        Vec& fillRandom() requires (Size == 4)
        {
            m_vec = T(glm::sphericalRand(1.0f), m_vec[Size-1]);
            return *this;
        }

        float length() const { return glm::length(m_vec); }

        Vec normalized() const { return glm::normalize(m_vec); }
        Vec& normalize()
        { 
            m_vec = glm::normalize(m_vec); 
            return *this;
        }

        Scalar dot(const Vec& other) const { return glm::dot(m_vec, other.m_vec); }
        Vec cross(const Vec& other) const requires (Size <= 3) { return glm::cross(m_vec, other.m_vec); }
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

        Scalar& x()                                         { return m_vec.x; }
        Scalar& y()                                         { return m_vec.y; }
        Scalar& z()                 requires (Size >= 3)    { return m_vec.z; }
        Scalar& w()                 requires (Size == 4)    { return m_vec.w; }
        const Scalar& x() const                             { return m_vec.x; }
        const Scalar& y() const                             { return m_vec.y; }
        const Scalar& z() const     requires (Size >= 3)    { return m_vec.z; }
        const Scalar& w() const     requires (Size == 4)    { return m_vec.w; }
        Scalar& r()                 requires (Size >= 3)    { return m_vec.r; }
        Scalar& g()                 requires (Size >= 3)    { return m_vec.g; }
        Scalar& b()                 requires (Size >= 3)    { return m_vec.b; }
        Scalar& a()                 requires (Size == 4)    { return m_vec.a; }
        const Scalar& r() const     requires (Size >= 3)    { return m_vec.r; }
        const Scalar& g() const     requires (Size >= 3)    { return m_vec.g; }
        const Scalar& b() const     requires (Size >= 3)    { return m_vec.b; }
        const Scalar& a() const     requires (Size == 4)    { return m_vec.a; }

        Scalar& operator[](const std::size_t i) { return m_vec[i]; }
        const Scalar& operator[](const std::size_t i) const { return m_vec[i]; }
        
        operator T&() { return m_vec; }
        operator const T&() const { return m_vec; }

        T& operator*() { return m_vec; }
        const T& operator*() const { return m_vec; }

        T* operator&() { return &m_vec; }
        const T* operator&() const { return &m_vec; }
        
        T* operator->() { return &m_vec; }
        const T* operator->() const { return &m_vec; }

        Vec operator+() const { return +m_vec; }	
        Vec operator-() const { return -m_vec; }

        Vec operator+(const Vec& other) const { return m_vec + other.m_vec; }	
        Vec operator-(const Vec& other) const { return m_vec - other.m_vec; }	
        Vec operator*(const Vec& other) const { return m_vec * other.m_vec; }	
        Vec operator/(const Vec& other) const { return m_vec / other.m_vec; }
        Vec operator%(const Vec& other) const { return cross(other); }
        Scalar operator^(const Vec& other) const { return dot(other); }

        Vec operator+(const Scalar scalar) const { return m_vec + scalar; }	
        Vec operator-(const Scalar scalar) const { return m_vec - scalar; }	
        Vec operator*(const Scalar scalar) const { return m_vec * scalar; }	
        Vec operator/(const Scalar scalar) const { return m_vec / scalar; }
   
        friend Vec operator+(const Scalar scalar, const Vec& vec) { return scalar + vec.m_vec; }	
        friend Vec operator-(const Scalar scalar, const Vec& vec) { return scalar - vec.m_vec; }	
        friend Vec operator*(const Scalar scalar, const Vec& vec) { return scalar * vec.m_vec; }	
        friend Vec operator/(const Scalar scalar, const Vec& vec) { return scalar / vec.m_vec; }

        Vec& operator+=(const Vec& other) { m_vec = m_vec + other.m_vec; return *this; }
        Vec& operator-=(const Vec& other) { m_vec = m_vec - other.m_vec; return *this; }
        Vec& operator*=(const Vec& other) { m_vec = m_vec * other.m_vec; return *this; }
        Vec& operator/=(const Vec& other) { m_vec = m_vec / other.m_vec; return *this; }
        Vec& operator%=(const Vec& other) { m_vec = cross(other); return *this; }

        Vec& operator+=(const Scalar scalar) { m_vec = m_vec + scalar; return *this; }
        Vec& operator-=(const Scalar scalar) { m_vec = m_vec - scalar; return *this; }
        Vec& operator*=(const Scalar scalar) { m_vec = m_vec * scalar; return *this; }
        Vec& operator/=(const Scalar scalar) { m_vec = m_vec / scalar; return *this; }

        Vec<glm::vec2> asVec2() const { return glm::vec2(m_vec); }
        Vec<glm::vec3> asVec3() const requires (Size == 2) { return glm::vec3(m_vec, 1.0f); }
        Vec<glm::vec3> asVec3() const requires (Size >= 3) { return glm::vec3(m_vec); }
        Vec<glm::vec4> asVec4() const requires (Size == 2) { return glm::vec4(m_vec, 0.0f, 1.0f); }
        Vec<glm::vec4> asVec4() const requires (Size == 3) { return glm::vec4(m_vec, 1.0f); }
        Vec<glm::vec4> asVec4() const requires (Size >= 4) { return glm::vec4(m_vec); }

        const Scalar* data() const { return glm::value_ptr(m_vec); }

        std::string toString() const { return glm::to_string(m_vec); }
        std::string toPrettyString(const bool printType = true) const 
        { 
            std::stringstream ss;
            if (printType) ss << "vec" << Size << ' ';
            ss << '(';
            for (std::size_t i = 0; i < Size; ++i)
                ss << std::format("{:8.4f}{}", m_vec[i], (i == Size-1 ? " " : ", "));
            ss << ')';
            return ss.str();
        }
        void print() const { MZ_TRACE("{}", toPrettyString()); }

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
    concept MatrixType = std::same_as<T, glm::mat3> || std::same_as<T, glm::mat4>;

    export template<MatrixType T = glm::mat4>
    class Mat
    {
    public:
        using Scalar = typename T::value_type;
        static constexpr std::size_t Size = T::length();

        Mat() : m_data{ 0.0f } {  }
        Mat(const Mat& other) : m_data{ other.m_data } {}
        Mat(Mat&& other) : m_data{ std::move(other.m_data) } { }
        Mat(const T& mat) : m_data{ mat } {}
        Mat(T&& mat) : m_data{ std::move(mat) } { }
        Mat(const Scalar scalar) : m_data{ scalar } {}

        Mat& fromEulerXYZ(const Vec3& eulerXYZ)
        {
            m_data = T(glm::eulerAngleXYZ(eulerXYZ.x(), eulerXYZ.y(), eulerXYZ.z()));
            return *this;
        }

        Mat& fromEulerZYX(const Vec3& eulerZYX)
        {
            m_data = T(glm::eulerAngleZYX(eulerZYX[0], eulerZYX[1], eulerZYX[2]));
            return *this;
        }

        Vec3 toEulerXYZ() const
        {
            Vec3 eulerXYZ;
            glm::extractEulerAngleXYZ(*asMat4(), eulerXYZ.x(), eulerXYZ.y(), eulerXYZ.z());
            return eulerXYZ;
        }

        Vec3 toEulerZYX() const
        {
            Vec3 eulerZYX;
            glm::extractEulerAngleZYX(*asMat4(), eulerZYX[0], eulerZYX[1], eulerZYX[2]);
            return eulerZYX;
        }

        Scalar maxValue() const 
        {
            Scalar maxVal = std::numeric_limits<Scalar>::min();
            for (std::size_t i = 0; i < Size; ++i) maxVal = glm::max(maxVal, m_data.vecs[i].maxValue());
            return maxVal;
        }

        Scalar minValue() const 
        {
            Scalar minVal = std::numeric_limits<Scalar>::max();
            for (std::size_t i = 0; i < Size; ++i) minVal = glm::min(minVal, m_data.vecs[i].minValue());
            return minVal;
        }

        Mat& fillRandom()
        {
            for (std::size_t i = 0; i < Size; ++i)
                m_data.vecs[i].fillRandom();
            return *this;
        }

        Mat& rescale()
        {
            xAxis().normalize();
            yAxis().normalize();
            zAxis().normalize();
            return *this;
        }

        Vec<glm::vec<Size, Scalar>>& xAxis() { return m_data.vecs[0]; }
        Vec<glm::vec<Size, Scalar>>& yAxis() { return m_data.vecs[1]; }
        Vec<glm::vec<Size, Scalar>>& zAxis() { return m_data.vecs[2]; }
        Vec<glm::vec<Size, Scalar>>& translation() { return m_data.vecs[Size-1]; }

        const Vec<glm::vec<Size, Scalar>>& xAxis() const { return m_data.vecs[0]; }
        const Vec<glm::vec<Size, Scalar>>& yAxis() const { return m_data.vecs[1]; }
        const Vec<glm::vec<Size, Scalar>>& zAxis() const { return m_data.vecs[2]; }
        const Vec<glm::vec<Size, Scalar>>& translation() const { return m_data.vecs[Size-1]; }

        Scalar& operator[](const std::size_t row, const std::size_t col) { return m_data.mat[row][col]; }
        const Scalar& operator[](const std::size_t row, const std::size_t col) const { return m_data.mat[row][col]; }

        Vec<glm::vec<Size, Scalar>>& operator[](const std::size_t row) { return m_data.vecs[row]; }
        const Vec<glm::vec<Size, Scalar>>& operator[](const std::size_t row) const { return m_data.vecs[row]; }

        operator T&() { return m_data.mat; }
        operator const T&() const { return m_data.mat; }

        T& operator*() { return m_data.mat; }
        const T& operator*() const { return m_data.mat; }

        T* operator&() { return &m_data.mat; }
        const T* operator&() const { return &m_data.mat; }
        
        T* operator->() { return &m_data.mat; }
        const T* operator->() const { return &m_data.mat; }

        Mat operator+() const { return +m_data.mat; }	
        Mat operator-() const { return -m_data.mat; }

        Mat operator+(const Mat& other) const { return m_data.mat + other.m_data.mat; }	
        Mat operator-(const Mat& other) const { return m_data.mat - other.m_data.mat; }	
        Mat operator*(const Mat& other) const { return m_data.mat * other.m_data.mat; }	
        Mat operator/(const Mat& other) const { return m_data.mat / other.m_data.mat; }

        Mat operator+(const Scalar scalar) const { return m_data.mat + scalar; }	
        Mat operator-(const Scalar scalar) const { return m_data.mat - scalar; }	
        Mat operator*(const Scalar scalar) const { return m_data.mat * scalar; }	
        Mat operator/(const Scalar scalar) const { return m_data.mat / scalar; }
   
        friend Mat operator+(const Scalar scalar, const Mat& mat) { return scalar + mat.m_data.mat; }	
        friend Mat operator-(const Scalar scalar, const Mat& mat) { return scalar - mat.m_data.mat; }	
        friend Mat operator*(const Scalar scalar, const Mat& mat) { return scalar * mat.m_data.mat; }	
        friend Mat operator/(const Scalar scalar, const Mat& mat) { return scalar / mat.m_data.mat; }

        Mat& operator+=(const Mat& other) { m_data.mat = m_data.mat + other.m_data.mat; return *this; }
        Mat& operator-=(const Mat& other) { m_data.mat = m_data.mat - other.m_data.mat; return *this; }
        Mat& operator*=(const Mat& other) { m_data.mat = m_data.mat * other.m_data.mat; return *this; }
        Mat& operator/=(const Mat& other) { m_data.mat = m_data.mat / other.m_data.mat; return *this; }

        Mat& operator+=(const Scalar scalar) { m_data.mat = m_data.mat + scalar; return *this; }
        Mat& operator-=(const Scalar scalar) { m_data.mat = m_data.mat - scalar; return *this; }
        Mat& operator*=(const Scalar scalar) { m_data.mat = m_data.mat * scalar; return *this; }
        Mat& operator/=(const Scalar scalar) { m_data.mat = m_data.mat / scalar; return *this; }

        Vec<glm::vec<Size, Scalar>> operator*(const Vec<glm::vec<Size, Scalar>>& vec) const { return m_data.mat * (*vec); }
        friend Vec<glm::vec<Size, Scalar>> operator/(const Vec<glm::vec<Size, Scalar>>& vec, const Mat& mat) { return (*vec) * mat.m_data.mat; }

        Mat<glm::mat3> asMat3() const { return glm::mat3(m_data.mat); }
        Mat<glm::mat4> asMat4() const { return glm::mat4(m_data.mat); }

        const Scalar* data() const { return glm::value_ptr(m_data.mat); }

        std::string toString() const { return glm::to_string(m_data.mat); }
        std::string toPrettyString(const bool printMatType = true, const bool printVecType = false) const 
        { 
            std::stringstream ss;
            if (printMatType) ss << "mat" << Size << "x" << Size << ' ';
            ss << "(\n";
            for (std::size_t i = 0; i < Size; ++i) {
                ss << '\t';
                ss << m_data.vecs[i].toPrettyString(printVecType);
                ss << (i == Size-1 ? "\n" : ",\n");
            }
            ss << ')';
            return ss.str();
        }
        void print() const { MZ_TRACE("{}", toPrettyString()); }

    private:
        union MatData
        {
            T mat;
            Vec<glm::vec<Size, Scalar>> vecs[Size];
        };

        MatData m_data;

    };

    export using Mat3 = Mat<glm::mat3>;
    export using Mat4 = Mat<glm::mat4>;

}