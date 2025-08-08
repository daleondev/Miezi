module;
#include "mz/core/core.h"
export module mz.util.geometry;

import std;
import glm;

import mz.core.logging;

namespace mz {

    template <typename T>
    concept VectorType = std::same_as<T, glm::vec2> || std::same_as<T, glm::vec3> || std::same_as<T, glm::vec4>;

    export template<VectorType T>
    class Vec;

    export using Vec2 = Vec<glm::vec2>;
    export using Vec3 = Vec<glm::vec3>;
    export using Vec4 = Vec<glm::vec4>;

    template <typename T>
    concept MatrixType = std::same_as<T, glm::mat3> || std::same_as<T, glm::mat4>;

    export template<MatrixType T>
    class Mat;

    export using Mat3 = Mat<glm::mat3>;
    export using Mat4 = Mat<glm::mat4>;

    //------------------------------------------------------
    //                      Vector
    //------------------------------------------------------

    export template<VectorType T>
    class Vec : public T
    {
    public:
        using Scalar = typename T::value_type;
        static constexpr std::size_t Size = T::length();

        using T::T;

        Vec() : T(0.0f) {}
        Vec(const T& vec) : T(vec) {}
        Vec(T&& vec) : T(std::move(vec)) {}

        Mat3 skewSymmetricCross_alt() const requires (Size == 3);
        Mat3 rotationTo_alt(const Vec3& vec) const requires (Size == 3);
        Mat3 rotationTo(const Vec3& vec) const requires (Size == 3);

        Vec3 projected(const Vec3& obj, const Mat4 model, const Mat4 proj, const Vec4 viewPort) const requires (Size == 3);
        Vec3& project(const Vec3& obj, const Mat4 model, const Mat4 proj, const Vec4 viewPort) requires (Size == 3);

        Scalar length() const { return glm::length(*this); }
        Scalar dist(const Vec& other) const { return glm::distance(*this, other); }

        Vec normalized() const { return glm::normalize(*this); }
        Vec& normalize() { return (*this = glm::normalize(*this)); }

        Vec reflected(const Vec& normal) const { return glm::reflect(*this, normal); }
        Vec& reflect(const Vec& normal) { return (*this = glm::reflect(*this, normal)); }

        Vec2 rotated(const Scalar angle) const requires (Size == 2) { return glm::rotate(*this, angle); }
        Vec2& rotate(const Scalar angle) requires (Size == 2) { return (*this = glm::rotate(*this, angle)); }
        Vec rotatedAround(const Vec& axis, const Scalar angle) const requires (Size >= 3) { return glm::rotate(*this, angle, axis); }
        Vec& rotateAround(const Vec& axis, const Scalar angle) requires (Size >= 3) { return (*this = glm::rotate(*this, angle, axis)); }

        Vec rotatedX(const Scalar angle) const requires (Size >= 3) { return glm::rotateX(*this, angle); }
        Vec& rotateX(const Scalar angle) requires (Size >= 3) { return (*this = glm::rotateX(*this, angle)); }
        Vec rotatedY(const Scalar angle) const requires (Size >= 3) { return glm::rotateY(*this, angle); }
        Vec& rotateY(const Scalar angle) requires (Size >= 3) { return (*this = glm::rotateY(*this, angle)); }
        Vec rotatedZ(const Scalar angle) const requires (Size >= 3) { return glm::rotateZ(*this, angle); }
        Vec& rotateZ(const Scalar angle) requires (Size >= 3) { return (*this = glm::rotateZ(*this, angle)); }

        Scalar dot(const Vec& other) const { return glm::dot(**this, other); }
        Vec cross(const Vec& other) const requires (Size <= 3) { return glm::cross(*this, other); }
        Vec4 cross(const Vec4& B, const Vec4& C) const requires (Size == 4);

        Scalar angleTo(const Vec& other) const { return glm::angle(this->normalized(), other.normalized()); }
        Scalar orientedAngleTo(const Vec2& other) const requires (Size == 2) { return glm::orientedAngle(this->normalized(), other.normalized()); }
        Scalar orientedAngleTo(const Vec3& other, const Vec3& ref) const requires (Size == 3) { return glm::orientedAngle(this->normalized(), other.normalized(), ref.normalized()); }

        operator T&() { return *this; }
        operator const T&() const { return *this; }

        T& operator*() { return *this; }
        const T& operator*() const { return *this; }

        Scalar operator^(const Vec& other) const { return dot(other); }
        Vec operator%(const Vec& other) const requires (Size <= 3) { return cross(other); }

        Vec2 asVec2() const { return *this; }
        Vec3 asVec3(const float z = 0.0f) const requires (Size == 2) { return Vec3(*this, z); }
        Vec3 asVec3() const requires (Size >= 3) { return *this; }
        Vec4 asVec4(const float w = 0.0f, const float z = 0.0f) const requires (Size == 2) { return Vec4(*this, z, w); }
        Vec4 asVec4(const float w = 0.0f) const requires (Size == 3) { return Vec4(*this, w); }
        Vec4 asVec4() const requires (Size >= 4) { return *this; }

        Vec2 fillRandom(const Scalar radius = 1.0f) requires (Size == 2) { return (*this = glm::circularRand(radius)); }
        Vec3 fillRandom(const Scalar radius = 1.0f) requires (Size == 3) { return (*this = glm::sphericalRand(radius)); }
        Vec4 fillRandom(const Scalar radius = 1.0f) requires (Size == 4) { return (*this = Vec4(glm::sphericalRand(radius), (*this)[Size-1])); }

        Scalar* data() { return glm::value_ptr((T&)*this); }
        const Scalar* data() const { return glm::value_ptr((T&)*this); }

        Scalar maxValue() const;
        Scalar minValue() const;

        std::string toPrettyString(const bool printType = true) const;
        std::string toString() const { return glm::to_string(*this); }
        void print() const { MZ_TRACE("{}", toPrettyString()); } 

    };

    //------------------------------------------------------
    //                      Matrix
    //------------------------------------------------------

    export template<MatrixType T>
    class Mat : public T
    {
    public:
        using Scalar = typename T::value_type;
        static constexpr std::size_t Size = T::length();

        using T::T;

        Mat() : T(0.0f) {}
        Mat(const T& mat) : T(mat) {}
        Mat(T&& mat) : T(std::move(mat)) {}

        Mat& fromEulerXYZ(const Vec3& eulerXYZ)
        {
            *this = T(glm::eulerAngleXYZ(eulerXYZ.x, eulerXYZ.y, eulerXYZ.z));
            return *this;
        }

        Mat& fromEulerZYX(const Vec3& eulerZYX)
        {
            *this = T(glm::eulerAngleZYX(eulerZYX[0], eulerZYX[1], eulerZYX[2]));
            return *this;
        }

        Vec3 toEulerXYZ() const
        {
            Vec3 eulerXYZ;
            glm::extractEulerAngleXYZ(asMat4(), eulerXYZ.x, eulerXYZ.y, eulerXYZ.z);
            return eulerXYZ;
        }

        Vec3 toEulerZYX() const
        {
            Vec3 eulerZYX;
            glm::extractEulerAngleZYX(asMat4(), eulerZYX[0], eulerZYX[1], eulerZYX[2]);
            return eulerZYX;
        }

        Scalar maxValue() const 
        {
            Scalar maxVal = std::numeric_limits<Scalar>::min();
            for (std::size_t i = 0; i < Size; ++i) maxVal = glm::max(maxVal, m_vecs[i].maxValue());
            return maxVal;
        }

        Scalar minValue() const 
        {
            Scalar minVal = std::numeric_limits<Scalar>::max();
            for (std::size_t i = 0; i < Size; ++i) minVal = glm::min(minVal, m_vecs[i].minValue());
            return minVal;
        }

        Mat& fillRandom()
        {
            for (std::size_t i = 0; i < Size; ++i)
                m_vecs[i].fillRandom();
            return *this;
        }

        Mat& rescale()
        {
            xAxis().normalize();
            yAxis().normalize();
            zAxis().normalize();
            return *this;
        }

        Vec<glm::vec<Size, Scalar>>& xAxis() { return m_vecs[0]; }
        Vec<glm::vec<Size, Scalar>>& yAxis() { return m_vecs[1]; }
        Vec<glm::vec<Size, Scalar>>& zAxis() { return m_vecs[2]; }
        Vec<glm::vec<Size, Scalar>>& translation() { return m_vecs[Size-1]; }

        const Vec<glm::vec<Size, Scalar>>& xAxis() const { return m_vecs[0]; }
        const Vec<glm::vec<Size, Scalar>>& yAxis() const { return m_vecs[1]; }
        const Vec<glm::vec<Size, Scalar>>& zAxis() const { return m_vecs[2]; }
        const Vec<glm::vec<Size, Scalar>>& translation() const { return m_vecs[Size-1]; }

        operator T&() { return *this; }
        operator const T&() const { return *this; }

        Mat<glm::mat3> asMat3() const { return *this; }
        Mat<glm::mat4> asMat4() const { return *this; }

        Scalar* data() 
        { 
            T& mat = *this;
            return glm::value_ptr(mat); 
        }

        const Scalar* data() const 
        { 
            T& mat = *this;
            return glm::value_ptr(mat); 
        }

        std::string toString() const { return glm::to_string(*this); }
        std::string toPrettyString(const bool printMatType = true, const bool printVecType = false) const 
        { 
            std::stringstream ss;
            if (printMatType) ss << "mat" << Size << "x" << Size << ' ';
            ss << "(\n";
            for (std::size_t i = 0; i < Size; ++i) {
                ss << '\t';
                ss << m_vecs[i].toPrettyString(printVecType);
                ss << (i == Size-1 ? "\n" : ",\n");
            }
            ss << ')';
            return ss.str();
        }
        void print() const { MZ_TRACE("{}", toPrettyString()); }

    private:
        Vec<glm::vec<Size, Scalar>>* const m_vecs = reinterpret_cast<Vec<glm::vec<Size, Scalar>>* const>(data());

    };

}