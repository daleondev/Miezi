#pragma once

// imgui
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// glfw
#include <GLFW/glfw3.h>

// // glm
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/matrix_inverse.hpp>
// #include <glm/gtx/euler_angles.hpp>
// #include <glm/gtx/string_cast.hpp>
// #include <glm/gtx/quaternion.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtc/epsilon.hpp>
// #include <glm/gtc/random.hpp>
// #include <glm/glm.hpp>

#if defined(WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__NT__)
    #define NOMINMAX
    #include <Windows.h>
#endif