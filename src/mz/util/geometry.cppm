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

        bool isFinite() const { return !glm::any(glm::isinf(**this)); }
        bool hasNaN() const { return glm::any(glm::isnan(**this)); }

        Mat3 skewSymmetricCross_alt() const requires (Size == 3);
        Mat3 rotationTo_alt(const Vec3& vec) const requires (Size == 3);
        Mat3 rotationTo(const Vec3& vec) const requires (Size == 3);

        Scalar length() const { return glm::length(**this); }
        Scalar dist(const Vec& other) const { return glm::distance(**this, other); }

        Vec3 projected(const Mat4 model, const Mat4 proj, const Vec4 viewPort) const requires (Size == 3);
        Vec3& project(const Mat4 model, const Mat4 proj, const Vec4 viewPort) requires (Size == 3);

        Vec normalized() const { return glm::normalize(**this); }
        Vec& normalize() { return (*this = normalized()); }
        bool isNormalized() const { return glm::epsilonEqual<float>(length(), 1.0f, glm::epsilon<float>()); }

        Vec reflected(const Vec& normal) const { return glm::reflect(**this, *normal); }
        Vec& reflect(const Vec& normal) { return (*this = reflected(normal)); }

        Vec2 rotated(const Scalar angle) const requires (Size == 2) { return glm::rotate(**this, angle); }
        Vec2& rotate(const Scalar angle) requires (Size == 2) { return (*this = rotated(angle)); }
        Vec rotatedAround(const Vec& axis, const Scalar angle) const requires (Size >= 3) { return glm::rotate(**this, angle, *axis); }
        Vec& rotateAround(const Vec& axis, const Scalar angle) requires (Size >= 3) { return (*this = rotatedAround(axis, angle)); }

        Vec rotatedX(const Scalar angle) const requires (Size >= 3) { return glm::rotateX(**this, angle); }
        Vec& rotateX(const Scalar angle) requires (Size >= 3) { return (*this = rotatedX(angle)); }
        Vec rotatedY(const Scalar angle) const requires (Size >= 3) { return glm::rotateY(**this, angle); }
        Vec& rotateY(const Scalar angle) requires (Size >= 3) { return (*this = rotatedY(angle)); }
        Vec rotatedZ(const Scalar angle) const requires (Size >= 3) { return glm::rotateZ(**this, angle); }
        Vec& rotateZ(const Scalar angle) requires (Size >= 3) { return (*this = rotatedZ(angle)); }

        Scalar dot(const Vec& other) const { return glm::dot(**this, *other); }
        Vec cross(const Vec& other) const requires (Size <= 3) { return glm::cross(**this, *other); }
        Vec4 cross(const Vec4& B, const Vec4& C) const requires (Size == 4);

        Scalar angleTo(const Vec& other) const { return glm::angle(*this->normalized(), *other.normalized()); }
        Scalar orientedAngleTo(const Vec2& other) const requires (Size == 2) { return glm::orientedAngle(*this->normalized(), *other.normalized()); }
        Scalar orientedAngleTo(const Vec3& other, const Vec3& ref) const requires (Size == 3) { return glm::orientedAngle(*this->normalized(), *other.normalized(), *ref.normalized()); }

        operator T&() { return *this; }
        operator const T&() const { return *this; }

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

        Scalar maxValue() const;
        Scalar minValue() const;

        Scalar* data() { return glm::value_ptr(**this); }
        const Scalar* data() const { return glm::value_ptr(**this); }

        std::string toPrettyString(const bool printType = true) const;
        std::string toString() const { return glm::to_string(**this); }
        void print() const { MZ_TRACE("{}", toPrettyString()); } 

    private:
        T& operator*() { return *this; }
        const T& operator*() const { return *this; }

        template<MatrixType>
        friend class Mat;

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

        Vec<glm::vec<Size, Scalar>>& xAxis() { return m_vecs[0]; }
        Vec<glm::vec<Size, Scalar>>& yAxis() { return m_vecs[1]; }
        Vec<glm::vec<Size, Scalar>>& zAxis() { return m_vecs[2]; }
        Vec<glm::vec<Size, Scalar>>& translation() { return m_vecs[Size-1]; }

        const Vec<glm::vec<Size, Scalar>>& xAxis() const { return m_vecs[0]; }
        const Vec<glm::vec<Size, Scalar>>& yAxis() const { return m_vecs[1]; }
        const Vec<glm::vec<Size, Scalar>>& zAxis() const { return m_vecs[2]; }
        const Vec<glm::vec<Size, Scalar>>& translation() const { return m_vecs[Size-1]; }

        bool isAffine() const 
        { 
            Scalar expected = 1;
            for (std::size_t i = Size; i > 0; i--) {
                if ((*this)[i-1][Size-1] != expected) return false;
                expected = 0;
            }
            return true;
        }

        bool isFinite() const 
        { 
            for (std::size_t i = 0; i < Size; ++i) 
                if (!m_vecs[i].isFinite())
                    return false;
            return true;
        }

        bool hasNaN() const 
        { 
            for (std::size_t i = 0; i < Size; ++i) 
                if (!m_vecs[i].hasNaN())
                    return false;
            return true;
        }

        Scalar determinant() const { return glm::determinant(**this); }

        Mat inverted() const { return isAffine() ? glm::affineInverse(**this) : glm::inverse(**this); }
        Mat& invert() { return (*this = inverted()); }

        Mat transposed() const { return glm::transpose(**this); }
        Mat& transpose() { return (*this = transposed()); }
        
        Mat invertedTransposed() const { return glm::inverseTranspose(**this); }
        Mat& inverseTranspose() { return (*this = invertedTransposed()); }

        Mat rescaled() const
        {
            Mat mat = *this;
            mat.xAxis().normalize();
            mat.yAxis().normalize();
            mat.zAxis().normalize();
            return mat;
        }
        Mat& rescale()
        {
            xAxis().normalize();
            yAxis().normalize();
            zAxis().normalize();
            return *this;
        }

        Mat rotated(const Vec3& axis, const Scalar angle) const { return T(glm::rotate(**this, angle, *axis)); }
        Mat& rotate(const Vec3& axis, const Scalar angle) { return (*this = rotated(axis, angle)); }

        Mat scaled(const Vec3& scale) const { return T(glm::scale(**this, *scale)); }
        Mat& scale(const Vec3& scale) { return (*this = scaled(scale)); }

        Mat translated(const Vec3& vec) const { return T(glm::translate(**this, *vec)); }
        Mat& translate(const Vec3& vec) { return (*this = translated(vec)); }

        static Mat createLookAt(const Vec3& eye, const Vec3& center, const Vec3& up) { return T(glm::lookAt(*eye, *center, *up)); }
        Mat& lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) { return (*this = createLookAt(eye, center, up)); }

        static Mat createAngleAxis(const Vec3& axis, const Scalar angle) { return T(glm::axisAngleMatrix(*axis, angle)); }
        Mat& fromAngleAxis(const Vec3& axis, const Scalar angle) { return (*this = createAngleAxis(axis, angle)); }

        static Mat createEulerXYZ(const Vec3& eulerXYZ) { return T(glm::eulerAngleXYZ(eulerXYZ.x, eulerXYZ.y, eulerXYZ.z)); }
        Mat& fromEulerXYZ(const Vec3& eulerXYZ) { return (*this = createEulerXYZ(eulerXYZ.x, eulerXYZ.y, eulerXYZ.z)); }

        static Mat createEulerZYX(const Vec3& eulerZYX) { return T(glm::eulerAngleZYX(eulerZYX[0], eulerZYX[1], eulerZYX[2])); }
        Mat& fromEulerZYX(const Vec3& eulerZYX) { return (*this = createEulerZYX(eulerZYX[0], eulerZYX[1], eulerZYX[2])); }

        std::tuple<Vec3, Scalar> toAngleAxis() const
        {
            Vec3 axis; Scalar angle;
            glm::axisAngle(*asMat4(), axis, angle);
            return std::make_tuple(axis, angle);
        }

        Vec3 toEulerXYZ() const
        {
            Vec3 eulerXYZ;
            glm::extractEulerAngleXYZ(*asMat4(), eulerXYZ.x, eulerXYZ.y, eulerXYZ.z);
            return eulerXYZ;
        }
        
        Vec3 toEulerZYX() const
        {
            Vec3 eulerZYX;
            glm::extractEulerAngleZYX(*asMat4(), eulerZYX[0], eulerZYX[1], eulerZYX[2]);
            return eulerZYX;
        }

        Mat componentMult(const Mat& other) const { return glm::matrixCompMult(**this, *other); }
        Mat outerProduct(const Mat& other) const { return glm::outerProduct(**this, *other); }

        operator T&() { return *this; }
        operator const T&() const { return *this; }

        Mat3 asMat3() const { return *this; }
        Mat4 asMat4() const { return *this; }

        Mat& fillRandom()
        {
            for (std::size_t i = 0; i < Size; ++i)
                m_vecs[i].fillRandom();
            return *this;
        }

        Scalar maxValue() const 
        {
            Scalar maxVal = std::numeric_limits<Scalar>::min();
            for (std::size_t i = 0; i < Size; ++i) 
                maxVal = glm::max(maxVal, m_vecs[i].maxValue());
            return maxVal;
        }

        Scalar minValue() const 
        {
            Scalar minVal = std::numeric_limits<Scalar>::max();
            for (std::size_t i = 0; i < Size; ++i) 
                minVal = glm::min(minVal, m_vecs[i].minValue());
            return minVal;
        }

        Scalar* data() { return glm::value_ptr(**this); }
        const Scalar* data() const { return glm::value_ptr(**this); }

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

        T& operator*() { return *this; }
        const T& operator*() const { return *this; }

    };

}