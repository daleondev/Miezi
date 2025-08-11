module;
#include "mz/core/core.h"
export module mz.graphics.renderer.data;

import std;

import mz.core.logging;
import mz.core.types;

import mz.graphics.renderer.buffers;
import mz.graphics.renderer.resources;

import mz.math.geometry;

namespace mz { 

    export struct RenderData
    {
        std::shared_ptr<VertexArrayBase> vertexArray;
        std::shared_ptr<VertexBufferBase> vertexBuffer;
        std::shared_ptr<IndexBufferBase> indexBuffer;
        std::shared_ptr<ShaderBase> shader;

        void reset()
        {
            vertexArray.reset();
            vertexBuffer.reset();
            indexBuffer.reset();
            shader.reset();
        }
    };

    //------------------------------------------------------
    //                      Line
    //------------------------------------------------------

    export struct LineData
    {
        struct Vertex
        {
            Vec3 pos;
        };

        inline static const BufferLayout layout = {
            { ShaderDataType::Float3, "a_pos" }
        };

        static constexpr const Array<Vertex, 2> vertices = std::array<Vertex, 2>{
            Vertex{Vec3{0.0f, 0.0f, 0.0f}},
            Vertex{Vec3{0.0f, 0.0f, 1.0f}}
        };
    };

    //------------------------------------------------------
    //                      Rect
    //------------------------------------------------------

    export struct RectData
    {
        struct Vertex
        {
            Vec3 pos;
            Vec2 uv;
        };

        inline static const BufferLayout layout = {
            { ShaderDataType::Float3, "a_pos" },
            { ShaderDataType::Float2, "a_uv" }
        };

        static constexpr const Array<Vertex, 4> vertices = std::array<Vertex, 4>{
            Vertex{Vec3{-0.5f, -0.5f, 0.0f},    Vec2{0.0f, 0.0f}},
            Vertex{Vec3{ 0.5f, -0.5f, 0.0f},    Vec2{1.0f, 0.0f}},
            Vertex{Vec3{ 0.5f,  0.5f, 0.0f},    Vec2{1.0f, 1.0f}},
            Vertex{Vec3{-0.5f,  0.5f, 0.0f},    Vec2{0.0f, 1.0f}}
        };

        static constexpr const Array<std::uint32_t, 6> indices = std::array<std::uint32_t, 6>{
            0, 1, 2, 
            2, 3, 0
        };
    };

    //------------------------------------------------------
    //                      Circle
    //------------------------------------------------------

    static constexpr float CIRCLE_RADIUS = 0.5f;
    static constexpr std::uint16_t CIRCLE_SEGMENTS = 64;

    export struct CircleData
    {
        struct Vertex
        {
            Vec3 pos;
            Vec2 uv;
        };

        inline static const BufferLayout layout = {
            { ShaderDataType::Float3, "a_pos" },
            { ShaderDataType::Float2, "a_uv" }
        };

        static auto generateCircle() {
            constexpr std::size_t vertexCount = CIRCLE_SEGMENTS + 1; // center + perimeter

            Array<Vertex, vertexCount> vertices{};
            Array<Array<std::uint32_t, 3>, CIRCLE_SEGMENTS> indices{};

            // Center vertex
            vertices[0].pos = Vec3(0.0f, 0.0f, 0.0f);
            vertices[0].uv  = Vec2(0.5f, 0.5f);

            for (std::uint16_t i = 0; i < CIRCLE_SEGMENTS; ++i) {
                float theta = 2.0f * std::numbers::pi_v<float> * i / CIRCLE_SEGMENTS;
                float x = std::cos(theta);
                float y = std::sin(theta);

                vertices[i + 1].pos = Vec3(x * CIRCLE_RADIUS, y * CIRCLE_RADIUS, 0.0f);
                vertices[i + 1].uv  = Vec2(0.5f + 0.5f * x, 0.5f + 0.5f * y);
            }

            // Indices: triangle fan
            for (std::uint16_t i = 0; i < CIRCLE_SEGMENTS; ++i) {
                std::uint16_t next = (i + 1) % CIRCLE_SEGMENTS;
                indices[i][0] = 0;
                indices[i][1] = i+1;
                indices[i][2] = next+1;
            }

            return std::make_pair(vertices, indices);
        }
    };

    //------------------------------------------------------
    //                      Plane
    //------------------------------------------------------

    export struct PlaneData
    {
        struct Vertex
        {
            Vec3 pos;
            Vec2 uv;
            Vec3 normal;
        };

        inline static const BufferLayout layout = {
            { ShaderDataType::Float3, "a_pos" },
            { ShaderDataType::Float2, "a_uv" },
            { ShaderDataType::Float3, "a_normal" }
        };

        static constexpr const Array<Vertex, 4> vertices = std::array<Vertex, 4>{
            Vertex{Vec3{-0.5f, -0.5f, 0.0f},    Vec2{0.0f, 0.0f},     Vec3{0.0f, 0.0f, 1.0f}},
            Vertex{Vec3{ 0.5f, -0.5f, 0.0f},    Vec2{1.0f, 0.0f},     Vec3{0.0f, 0.0f, 1.0f}},
            Vertex{Vec3{ 0.5f,  0.5f, 0.0f},    Vec2{1.0f, 1.0f},     Vec3{0.0f, 0.0f, 1.0f}},
            Vertex{Vec3{-0.5f,  0.5f, 0.0f},    Vec2{0.0f, 1.0f},     Vec3{0.0f, 0.0f, 1.0f}}
        };

        static constexpr const Array<std::uint32_t, 6> indices = std::array<std::uint32_t, 6>{
            0, 1, 2, 
            2, 3, 0
        };
    };

    //------------------------------------------------------
    //                      Box
    //------------------------------------------------------

    export struct BoxData
    {
        struct Vertex
        {
            Vec3 pos;
            Vec2 uv;
            Vec3 normal;
        };

        inline static const BufferLayout layout = {
            { ShaderDataType::Float3, "a_pos" },
            { ShaderDataType::Float2, "a_uv" },
            { ShaderDataType::Float3, "a_normal" }
        };

        static constexpr const Array<Vertex, 24> vertices = std::array<Vertex, 24>{
            // Front face (+Z)
            Vertex{Vec3{-0.5f, -0.5f,  0.5f},    Vec2{0.0f, 0.0f},    Vec3{ 0.0f,  0.0f,  1.0f}}, // Bottom-left
            Vertex{Vec3{ 0.5f, -0.5f,  0.5f},    Vec2{1.0f, 0.0f},    Vec3{ 0.0f,  0.0f,  1.0f}}, // Bottom-right
            Vertex{Vec3{ 0.5f,  0.5f,  0.5f},    Vec2{1.0f, 1.0f},    Vec3{ 0.0f,  0.0f,  1.0f}}, // Top-right
            Vertex{Vec3{-0.5f,  0.5f,  0.5f},    Vec2{0.0f, 1.0f},    Vec3{ 0.0f,  0.0f,  1.0f}}, // Top-left

            // Back face (-Z)
            Vertex{Vec3{-0.5f, -0.5f, -0.5f},    Vec2{1.0f, 0.0f},    Vec3{ 0.0f,  0.0f, -1.0f}}, // Bottom-right
            Vertex{Vec3{ 0.5f, -0.5f, -0.5f},    Vec2{0.0f, 0.0f},    Vec3{ 0.0f,  0.0f, -1.0f}}, // Bottom-left
            Vertex{Vec3{ 0.5f,  0.5f, -0.5f},    Vec2{0.0f, 1.0f},    Vec3{ 0.0f,  0.0f, -1.0f}}, // Top-left
            Vertex{Vec3{-0.5f,  0.5f, -0.5f},    Vec2{1.0f, 1.0f},    Vec3{ 0.0f,  0.0f, -1.0f}}, // Top-right

            // Left face (-X)
            Vertex{Vec3{-0.5f, -0.5f, -0.5f},    Vec2{0.0f, 0.0f},    Vec3{-1.0f,  0.0f,  0.0f}}, // Bottom-left
            Vertex{Vec3{-0.5f, -0.5f,  0.5f},    Vec2{1.0f, 0.0f},    Vec3{-1.0f,  0.0f,  0.0f}}, // Bottom-right
            Vertex{Vec3{-0.5f,  0.5f,  0.5f},    Vec2{1.0f, 1.0f},    Vec3{-1.0f,  0.0f,  0.0f}}, // Top-right
            Vertex{Vec3{-0.5f,  0.5f, -0.5f},    Vec2{0.0f, 1.0f},    Vec3{-1.0f,  0.0f,  0.0f}}, // Top-left

            // Right face (+X)
            Vertex{Vec3{ 0.5f, -0.5f,  0.5f},    Vec2{0.0f, 0.0f},    Vec3{ 1.0f,  0.0f,  0.0f}}, // Bottom-left
            Vertex{Vec3{ 0.5f, -0.5f, -0.5f},    Vec2{1.0f, 0.0f},    Vec3{ 1.0f,  0.0f,  0.0f}}, // Bottom-right
            Vertex{Vec3{ 0.5f,  0.5f, -0.5f},    Vec2{1.0f, 1.0f},    Vec3{ 1.0f,  0.0f,  0.0f}}, // Top-right
            Vertex{Vec3{ 0.5f,  0.5f,  0.5f},    Vec2{0.0f, 1.0f},    Vec3{ 1.0f,  0.0f,  0.0f}}, // Top-left

            // Top face (+Y)
            Vertex{Vec3{-0.5f,  0.5f,  0.5f},    Vec2{0.0f, 0.0f},    Vec3{ 0.0f,  1.0f,  0.0f}}, // Bottom-left
            Vertex{Vec3{ 0.5f,  0.5f,  0.5f},    Vec2{1.0f, 0.0f},    Vec3{ 0.0f,  1.0f,  0.0f}}, // Bottom-right
            Vertex{Vec3{ 0.5f,  0.5f, -0.5f},    Vec2{1.0f, 1.0f},    Vec3{ 0.0f,  1.0f,  0.0f}}, // Top-right
            Vertex{Vec3{-0.5f,  0.5f, -0.5f},    Vec2{0.0f, 1.0f},    Vec3{ 0.0f,  1.0f,  0.0f}}, // Top-left

            // Bottom face (-Y)
            Vertex{Vec3{-0.5f, -0.5f, -0.5f},    Vec2{0.0f, 0.0f},    Vec3{ 0.0f, -1.0f,  0.0f}}, // Bottom-left
            Vertex{Vec3{ 0.5f, -0.5f, -0.5f},    Vec2{1.0f, 0.0f},    Vec3{ 0.0f, -1.0f,  0.0f}}, // Bottom-right
            Vertex{Vec3{ 0.5f, -0.5f,  0.5f},    Vec2{1.0f, 1.0f},    Vec3{ 0.0f, -1.0f,  0.0f}}, // Top-right
            Vertex{Vec3{-0.5f, -0.5f,  0.5f},    Vec2{0.0f, 1.0f},    Vec3{ 0.0f, -1.0f,  0.0f}}  // Top-left
        };

        static constexpr Array<std::uint32_t, 36> indices = std::array<std::uint32_t, 36>{
            // Front face
            0,  1,  2,
            2,  3,  0,
            // Back face
            4,  5,  6,
            6,  7,  4,
            // Left face
            8,  9, 10,
            10, 11, 8,
            // Right face
            12, 13, 14,
            14, 15, 12,
            // Top face
            16, 17, 18,
            18, 19, 16,
            // Bottom face
            20, 21, 22,
            22, 23, 20,
        };
    };

    //------------------------------------------------------
    //                      Sphere
    //------------------------------------------------------

    static constexpr float SPHERE_RADIUS = 0.5f;
    static constexpr std::uint16_t SPHERE_RINGS = 16;
    static constexpr std::uint16_t SPHERE_SECTORS = 32;

    export struct SphereData
    {
        struct Vertex
        {
            Vec3 pos;
            Vec2 uv;
            Vec3 normal;
        };

        inline static const BufferLayout layout = {
            { ShaderDataType::Float3, "a_pos" },
            { ShaderDataType::Float2, "a_uv" },
            { ShaderDataType::Float3, "a_normal" }
        };

        static auto generateSphere() {
            constexpr std::size_t vertexCount = SPHERE_RINGS * SPHERE_SECTORS;
            constexpr std::size_t indexCount = (SPHERE_RINGS - 1) * SPHERE_SECTORS * 6;

            Array<Vertex, vertexCount> vertices{};
            Array<Array<std::uint32_t, 3>, indexCount / 3> indices{};

            constexpr float R = 1.0f / static_cast<float>(SPHERE_RINGS - 1);
            constexpr float S = 1.0f / static_cast<float>(SPHERE_SECTORS - 1);

            // Generate vertices
            std::size_t vertexIndex = 0;
            for (std::uint16_t r = 0; r < SPHERE_RINGS; ++r) {
                for (std::uint16_t s = 0; s < SPHERE_SECTORS; ++s) {
                    const float y = std::sin(-std::numbers::pi_v<float> / 2.0f + std::numbers::pi_v<float> * r * R);
                    const float x = std::cos(2.0f * std::numbers::pi_v<float> * s * S) * std::sin(std::numbers::pi_v<float> * r * R);
                    const float z = std::sin(2.0f * std::numbers::pi_v<float> * s * S) * std::sin(std::numbers::pi_v<float> * r * R);

                    const float u = s * S; // [0,1]
                    const float v = r * R; // [0,1]

                    vertices[vertexIndex++] = Vertex{
                        Vec3{x * SPHERE_RADIUS, y * SPHERE_RADIUS, z * SPHERE_RADIUS},
                        Vec2{u, v},
                        Vec3{x, y, z}.normalized()
                    };
                }
            }

            // Generate indices for triangles
            std::size_t indexIndex = 0;
            for (std::uint16_t r = 0; r < SPHERE_RINGS - 1; ++r) {
                for (std::uint16_t s = 0; s < SPHERE_SECTORS; ++s) {
                    std::uint16_t i1 = r * SPHERE_SECTORS + s;
                    std::uint16_t i2 = r * SPHERE_SECTORS + (s + 1) % SPHERE_SECTORS;
                    std::uint16_t i3 = (r + 1) * SPHERE_SECTORS + s;
                    std::uint16_t i4 = (r + 1) * SPHERE_SECTORS + (s + 1) % SPHERE_SECTORS;

                    indices[indexIndex++] = std::array<std::uint32_t, 3>{i1, i2, i4};
                    indices[indexIndex++] = std::array<std::uint32_t, 3>{i1, i4, i3};
                }
            }

            return std::make_pair(vertices, indices);
        }
    };

    //------------------------------------------------------
    //                      Mesh
    //------------------------------------------------------

    export struct MeshData
    {
        struct Vertex
        {
            Vec3 pos;
            Vec4 color;
            Vec3 normal;
        };

        inline static const BufferLayout layout = {
            { ShaderDataType::Float3, "a_pos" },
            { ShaderDataType::Float4, "a_color" },
            { ShaderDataType::Float3, "a_normal" }
        };

        Vector<Vertex> vertices = {};
        Vector<Array<std::uint32_t, 3>> indices = {};
    };

    //------------------------------------------------------
    //                      Pointcloud
    //------------------------------------------------------

    export struct PointcloudData
    {
        struct Vertex
        {
            Vec3 pos;
            Vec2 uv;
        };

        inline static const BufferLayout layout = {
            { ShaderDataType::Float3, "a_pos" },
            { ShaderDataType::Float2, "a_uv" }
        };

        Vector<Vertex> vertices = {};
    };

}