export module mz.graphics.renderer;

import std;

import mz.core.types;

import mz.graphics.renderer.resources;
import mz.graphics.renderer.data;

import mz.math.geometry;

namespace mz { 

    export class IRenderContext : public ICastable
    {
    public:
        virtual ~IRenderContext() = default;

        virtual void makeCurrent() = 0;
    };

    export class IRenderer : public ICastable
    {
    public:
        virtual ~IRenderer() = default;

        virtual void clear(const Vec4& color) = 0;
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
        static std::shared_ptr<RenderBase> create(IRenderContext* context);

        virtual ~RenderBase()
        {
            m_pointData.clear();
            m_lineData.clear();
            m_rectData.clear();
            m_circData.clear();
            m_planeData.clear();
            m_boxData.clear();
            m_sphereData.clear();
            m_meshData.clear();
        
            m_shaderStore->clear();
        }

    protected:
        RenderBase(IRenderContext* context, std::unique_ptr<ShaderStoreBase>&& shaderStore)
            : m_context{ context }, m_shaderStore{ std::move(shaderStore) } 
        {
            m_context->makeCurrent();
        }

        IRenderContext* m_context;
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