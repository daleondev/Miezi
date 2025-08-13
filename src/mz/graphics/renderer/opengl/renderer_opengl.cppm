module;
#include "mz/core/core.h"

#include <glad/gl.h>
export module mz.graphics.renderer.opengl;

import std;

import mz.core.logging;

import mz.graphics.window;

import mz.graphics.renderer;
import mz.graphics.renderer.data;
import mz.graphics.renderer.resources;
import mz.graphics.renderer.opengl.resources;
import mz.graphics.renderer.opengl.debug;

import mz.math.geometry;

namespace mz { 

    export class GlGraphicsContext : public IGraphicsContext
    {
    public:
        GlGraphicsContext(IWindow* window) : m_window{ window }, m_initialized{ false } {}
        virtual ~GlGraphicsContext() = default;

        void init(GLADloadfunc loadFunc)
        {
            if (m_initialized)
                return;

            const int version = gladLoadGL(loadFunc);
            MZ_ASSERT(version != 0, "Failed to initialize GLAD");
            MZ_INFO("OpenGL:");
            MZ_INFO("\tOpenGL Vendor: {}", (char*)glGetString(GL_VENDOR));
            MZ_INFO("\tOpenGL Renderer: {}", (char*)glGetString(GL_RENDERER));
            MZ_INFO("\tOpenGL Version: {}", (char*)glGetString(GL_VERSION));
            MZ_INFO("\tGLSL Version: {}", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
            MZ_INFO("\tGlad Version: {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
            
            setupDebugging();

            m_initialized = true;
        }

    protected:
        IWindow* m_window;

    private:
        bool m_initialized;

    };

    export class GlRenderer : public RenderBase
    {
    public:
        GlRenderer(IGraphicsContext* context) 
            : RenderBase(context, std::make_unique<GlShaderStore>()) 
        { 
            MZ_ASSERT(context->is<GlGraphicsContext>(), "Invalid graphics context type");

            glEnable(GL_DEPTH_TEST);
            glClearDepthf(1.0f);
            glDepthFunc(GL_LESS);
            
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_PROGRAM_POINT_SIZE);
            glEnable(GL_MULTISAMPLE);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

            //------------------------------------------------------
            //                      Point
            //------------------------------------------------------

            const auto pointShader = m_shaderStore->loadFromFiles("assets/shaders/Point");
            MZ_ASSERT(pointShader, "Failed to load Point shader");

            m_pointData.shader = *pointShader;
            m_pointData.vertexArray = std::make_shared<GlVertexArray>();

            //------------------------------------------------------
            //                      Line
            //------------------------------------------------------

            const auto lineShader = m_shaderStore->loadFromFiles("assets/shaders/Line");
            MZ_ASSERT(lineShader, "Failed to load Line shader");

            m_lineData.shader = *lineShader;

            m_lineData.vertexArray = std::make_shared<GlVertexArray>();
            m_lineData.vertexBuffer = std::make_shared<GlVertexBuffer>();
            m_lineData.vertexBuffer->allocate(LineData::vertices.reinterpret<float>());
            m_lineData.vertexBuffer->setLayout(LineData::layout);
            m_lineData.vertexArray->addVertexBuffer(m_lineData.vertexBuffer);

            //------------------------------------------------------
            //                      Rect
            //------------------------------------------------------

            const auto primitve2dShader = m_shaderStore->loadFromFiles("assets/shaders/Primitive2D");
            MZ_ASSERT(primitve2dShader, "Failed to load Primitive2D shader");

            m_lineData.shader = *primitve2dShader;

            m_rectData.vertexArray = std::make_shared<GlVertexArray>();
            m_rectData.vertexBuffer = std::make_shared<GlVertexBuffer>();
            m_rectData.vertexBuffer->allocate(RectData::vertices.reinterpret<float>());
            m_rectData.vertexBuffer->setLayout(RectData::layout);
            m_rectData.vertexArray->addVertexBuffer(m_rectData.vertexBuffer);

            m_rectData.indexBuffer = std::make_shared<GlIndexBuffer>();
            m_rectData.indexBuffer->allocate(RectData::indices.reinterpret<std::uint32_t>());
            m_rectData.vertexArray->setIndexBuffer(m_rectData.indexBuffer);

            //------------------------------------------------------
            //                      Circle
            //------------------------------------------------------

            m_circData.shader = *primitve2dShader;

            const auto [circleVertices, circleIndices] = CircleData::generateCircle();
            m_circData.vertexArray = std::make_shared<GlVertexArray>();
            m_circData.vertexBuffer = std::make_shared<GlVertexBuffer>();
            m_circData.vertexBuffer->allocate(circleVertices.reinterpret<float>());
            m_circData.vertexBuffer->setLayout(CircleData::layout);
            m_circData.vertexArray->addVertexBuffer(m_circData.vertexBuffer);

            m_circData.indexBuffer = std::make_shared<GlIndexBuffer>();
            m_circData.indexBuffer->allocate(circleIndices.reinterpret<std::uint32_t>());
            m_circData.vertexArray->setIndexBuffer(m_circData.indexBuffer);

            //------------------------------------------------------
            //                      Plane
            //------------------------------------------------------

            const auto primitve3dShader = m_shaderStore->loadFromFiles("assets/shaders/Primitive3D");
            MZ_ASSERT(primitve3dShader, "Failed to load Primitive3D shader");

            m_planeData.shader = *primitve3dShader;

            m_planeData.vertexArray = std::make_shared<GlVertexArray>();
            m_planeData.vertexBuffer = std::make_shared<GlVertexBuffer>();
            m_planeData.vertexBuffer->allocate(PlaneData::vertices.reinterpret<float>());
            m_planeData.vertexBuffer->setLayout(PlaneData::layout);
            m_planeData.vertexArray->addVertexBuffer(m_planeData.vertexBuffer);

            m_planeData.indexBuffer = std::make_shared<GlIndexBuffer>();
            m_planeData.indexBuffer->allocate(PlaneData::indices.reinterpret<std::uint32_t>());
            m_planeData.vertexArray->setIndexBuffer(m_planeData.indexBuffer);

            //------------------------------------------------------
            //                      Box
            //------------------------------------------------------

            m_boxData.shader = *primitve3dShader;

            m_boxData.vertexArray = std::make_shared<GlVertexArray>();
            m_boxData.vertexBuffer = std::make_shared<GlVertexBuffer>();
            m_boxData.vertexBuffer->allocate(BoxData::vertices.reinterpret<float>());
            m_boxData.vertexBuffer->setLayout(BoxData::layout);
            m_boxData.vertexArray->addVertexBuffer(m_boxData.vertexBuffer);

            m_boxData.indexBuffer = std::make_shared<GlIndexBuffer>();
            m_boxData.indexBuffer->allocate(BoxData::indices.reinterpret<std::uint32_t>());
            m_boxData.vertexArray->setIndexBuffer(m_boxData.indexBuffer);

            //------------------------------------------------------
            //                      Sphere
            //------------------------------------------------------

            m_sphereData.shader = *primitve3dShader;

            const auto [sphereVertices, sphereIndices] = SphereData::generateSphere();
            m_sphereData.vertexArray = std::make_shared<GlVertexArray>();
            m_sphereData.vertexBuffer = std::make_shared<GlVertexBuffer>();
            m_sphereData.vertexBuffer->allocate(sphereVertices.reinterpret<float>());
            m_sphereData.vertexBuffer->setLayout(SphereData::layout);
            m_sphereData.vertexArray->addVertexBuffer(m_sphereData.vertexBuffer);

            m_sphereData.indexBuffer = std::make_shared<GlIndexBuffer>();
            m_sphereData.indexBuffer->allocate(sphereIndices.reinterpret<std::uint32_t>());
            m_sphereData.vertexArray->setIndexBuffer(m_sphereData.indexBuffer);
        }
        ~GlRenderer() = default;

        void clear(const Vec4& color) override
        {
            m_context->makeCurrent();

            glClearColor(color.r, color.g, color.b, color.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void drawPoint(ICamera* camera, const Vec3& position, const Vec4& color, const float size = 1.0f) const override
        {
            m_pointData.shader->bind();
            m_pointData.shader->uploadVec3("u_pos", position);
            m_pointData.shader->uploadFloat("u_pointSize", size);
            m_pointData.shader->uploadVec4("u_color", color);

            m_pointData.vertexArray->bind();
            glDrawArrays(GL_POINTS, 0, 1);
        }

        void drawBox(ICamera* camera, const Mat4& transform, const Vec4& color) const override
        {
            const auto viewProjection = camera->getViewProjection();
            const auto normalMatrix = transform.invertedTransposed();

            m_boxData.shader->bind();
            m_boxData.shader->uploadMat4("u_viewProjection", viewProjection);
            m_boxData.shader->uploadMat4("u_model", transform);
            m_boxData.shader->uploadMat3("u_normalMatrix", normalMatrix);

            m_boxData.shader->uploadInt("u_useTexture", 0);
            m_boxData.shader->uploadVec4("u_flatColor", color);

            m_sphereData.shader->uploadVec3("u_lightPos", Vec3{10.0f, 10.0f, 20.0f});
            m_boxData.shader->uploadVec4("u_lightColor", {1.0f, 1.0f, 1.0f, 1.0f});
            m_boxData.shader->uploadVec4("u_ambientColor", {0.6f, 0.6f, 0.6f, 1.0f});
            m_boxData.shader->uploadVec3("u_viewPos", camera->getPosition());
            drawIndexed(m_boxData.shader, m_boxData.vertexArray);
        }

    private:
        void drawIndexed(const std::shared_ptr<ShaderBase>& shader, const std::shared_ptr<VertexArrayBase>& vertexArray) const
        {
            shader->bind();
            vertexArray->bind();
            glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, 0);
        }
    };

}