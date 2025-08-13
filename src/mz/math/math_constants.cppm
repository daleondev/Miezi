export module mz.math.constants;

import std;
import glm;

namespace mz {

    export constexpr float degToRad(const float deg) { return glm::radians(deg); }
    export constexpr float radToDeg(const float rad) { return glm::degrees(rad); }

    //------------------------------------------------------
    //                      Pi
    //------------------------------------------------------
 
    export template <typename T>
    constexpr T PI = std::numbers::pi_v<T>;

    export constexpr float PI_F = PI<float>;
    export constexpr double PI_D = PI<double>;

    //------------------------------------------------------
    //                      e
    //------------------------------------------------------

    export template <typename T>
    constexpr T E = std::numbers::e_v<T>;

    export constexpr float E_F = E<float>;
    export constexpr double E_D = E<double>;

    //------------------------------------------------------
    //                      Sqrt
    //------------------------------------------------------

    export template <typename T>
    constexpr T SQRT2 = std::numbers::sqrt2_v<T>;

    export constexpr float SQRT2_F = SQRT2<float>;
    export constexpr double SQRT2_D = SQRT2<double>;

    //------------------------------------------------------
    //                      Phi
    //------------------------------------------------------

    export template <typename T>
    constexpr T PHI = std::numbers::sqrt2_v<T>;

    export constexpr float PHI_F = PHI<float>;
    export constexpr double PHI_D = PHI<double>;

}