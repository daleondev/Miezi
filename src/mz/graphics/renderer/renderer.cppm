export module mz.graphics.renderer;

import std;

import mz.core.types;

export import mz.graphics.renderer.camera;
import mz.graphics.renderer.resources;
import mz.graphics.renderer.data;
import mz.graphics.window;

import mz.math.geometry;

namespace mz { 

    export class IRenderer : public ICastable
    {
    public:
        virtual ~IRenderer() = default;

        virtual void setViewport(const Vec2& viewport) = 0;

        virtual void beginScene(ICamera* camera) = 0;
        virtual void endScene() = 0;

        virtual void clear(const Vec4& color) = 0;
        virtual void drawPoint(const Vec3& position, const Vec4& color, const float size = 1.0f) const = 0;
        virtual void drawLine(const Mat4& transform, const Vec4& color, const float lineWidth = 1.0f, const bool smooth = false) const = 0;
        virtual void drawRect(const Mat4& transform, const Vec4& color) const = 0;
        virtual void drawRect(const Mat4& transform, const std::shared_ptr<ITexture>& texture) const = 0;
        virtual void drawCircle(const Mat4& transform, const Vec4& color) const = 0;
        virtual void drawCircle(const Mat4& transform, const std::shared_ptr<ITexture>& texture) const = 0;
        virtual void drawPlane(const Mat4& transform, const Vec4& color) const = 0;
        virtual void drawPlane(const Mat4& transform, const std::shared_ptr<ITexture>& texture) const = 0;
        virtual void drawBox(const Mat4& transform, const Vec4& color) const = 0;
        virtual void drawBox(const Mat4& transform, const std::shared_ptr<ITexture>& texture) const = 0;
        virtual void drawSphere(const Mat4& transform, const Vec4& color) const = 0;
        virtual void drawSphere(const Mat4& transform, const std::shared_ptr<ITexture>& texture) const = 0;
    };

    export class RenderBase : public IRenderer
    {
    public:
        static std::shared_ptr<RenderBase> create(IGraphicsContext* context);

        virtual ~RenderBase()
        {
            m_pointData.reset();
            m_lineData.reset();
            m_rectData.reset();
            m_circData.reset();
            m_planeData.reset();
            m_boxData.reset();
            m_sphereData.reset();
            
            m_meshData.clear();
        
            m_shaderStore->clear();
        }

        void beginScene(ICamera* camera) override
        {
            m_cameraData.transform = camera->getTransform();
            m_cameraData.view = camera->getView();
            m_cameraData.projection = camera->getProjection();
            m_cameraData.viewProjection = camera->getViewProjection();
        }

        void endScene() override
        {

        }

    protected:
        RenderBase(IGraphicsContext* context, std::unique_ptr<ShaderStoreBase>&& shaderStore)
            : m_context{ context }, m_shaderStore{ std::move(shaderStore) } 
        {
            m_context->makeCurrent();
        }

        IGraphicsContext* m_context;
        std::unique_ptr<ShaderStoreBase> m_shaderStore;

        struct CameraData
        {
            Mat4 transform;
            Mat4 view;
            Mat4 projection;
            Mat4 viewProjection;
        } m_cameraData; 

        RenderData m_pointData;
        RenderData m_lineData;
        RenderData m_rectData;
        RenderData m_circData;
        RenderData m_planeData;
        RenderData m_boxData;
        RenderData m_sphereData;

        std::map<std::string, RenderData> m_meshData;
        // std::map<std::uint32_t, std::tuple<GraphicsData, TextureBase>> m_pcData;

    };

}