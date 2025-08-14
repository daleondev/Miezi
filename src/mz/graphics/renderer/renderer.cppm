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

        virtual void clear(const Vec4& color) = 0;
        virtual void drawPoint(ICamera* camera, const Vec3& position, const Vec4& color, const float size = 1.0f) const = 0;
        virtual void drawLine(ICamera* camera, const Mat4& transform, const Vec4& color, const float lineWidth = 1.0f, const bool smooth = false) const = 0;
        virtual void drawBox(ICamera* camera, const Mat4& transform, const Vec4& color) const = 0;
        // virtual void drawPoint(/*OrbitCamera& camera, */const glm::mat4& transform, const Vec4& color, const float lineWidth = 1.0f, const bool smooth = true);
        // virtual void drawLine(/*OrbitCamera& camera, */const glm::mat4& transform, const Vec4& color, const float borderWidth = 1.0f, const Vec4& borderColor = Vec4{0.0f, 0.0f, 0.0f, 1.0f});
        // virtual void drawRect(/*OrbitCamera& camera, */const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const float borderWidth = 1.0f, const Vec4& borderColor = Vec4{0.0f, 0.0f, 0.0f, 1.0f});
        // virtual void drawRect(/*OrbitCamera& camera, */const Vec3& position, const Vec4& color, const float size);
        // virtual void drawCircle(/*OrbitCamera& camera, */const Vec3& position, const Vec4& color, const float size);
        // virtual void drawCircle(/*OrbitCamera& camera, */const Vec3& position, const Vec4& color, const float size);
        // virtual void drawBox(/*OrbitCamera& camera, */const Vec3& position, const Vec4& color, const float size);
        // virtual void drawBox(/*OrbitCamera& camera, */const Vec3& position, const Vec4& color, const float size);
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

    protected:
        RenderBase(IGraphicsContext* context, std::unique_ptr<ShaderStoreBase>&& shaderStore)
            : m_context{ context }, m_shaderStore{ std::move(shaderStore) } 
        {
            m_context->makeCurrent();
        }

        IGraphicsContext* m_context;
        std::unique_ptr<ShaderStoreBase> m_shaderStore;

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