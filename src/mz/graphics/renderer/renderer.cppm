export module mz.graphics.renderer;

import std;
import glm;

import mz.core.behaviours;

import mz.graphics.renderer.resources;
import mz.graphics.renderer.data;

namespace mz { 

    export class IGraphicsContext : public ICastable
    {
    public:
        virtual ~IGraphicsContext() = default;

        virtual void makeCurrent() = 0;
    };

    export class RenderBase : public ICastable
    {
    public:
        static std::shared_ptr<RenderBase> create(IGraphicsContext* context);

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

        virtual void clear(const glm::vec4& color) = 0;
        // virtual void drawPoint(/*OrbitCamera& camera, */const glm::mat4& transform, const glm::vec4& color, const float lineWidth = 1.0f, const bool smooth = true);
        // virtual void drawLine(/*OrbitCamera& camera, */const glm::mat4& transform, const glm::vec4& color, const float borderWidth = 1.0f, const glm::vec4& borderColor = glm::vec4{0.0f, 0.0f, 0.0f, 1.0f});
        // virtual void drawRect(/*OrbitCamera& camera, */const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const float borderWidth = 1.0f, const glm::vec4& borderColor = glm::vec4{0.0f, 0.0f, 0.0f, 1.0f});
        // virtual void drawRect(/*OrbitCamera& camera, */const glm::vec3& position, const glm::vec4& color, const float size);
        // virtual void drawCircle(/*OrbitCamera& camera, */const glm::vec3& position, const glm::vec4& color, const float size);
        // virtual void drawCircle(/*OrbitCamera& camera, */const glm::vec3& position, const glm::vec4& color, const float size);
        // virtual void drawBox(/*OrbitCamera& camera, */const glm::vec3& position, const glm::vec4& color, const float size);
        // virtual void drawBox(/*OrbitCamera& camera, */const glm::vec3& position, const glm::vec4& color, const float size);

    protected:
        RenderBase(IGraphicsContext* context, std::unique_ptr<ShaderStoreBase>&& shaderStore) 
            : m_context{ context }, m_shaderStore{ std::move(shaderStore) } 
        {
            m_context->makeCurrent();
        }

        IGraphicsContext* m_context;
        std::unique_ptr<ShaderStoreBase> m_shaderStore;

        GraphicsData m_pointData;
        GraphicsData m_lineData;
        GraphicsData m_rectData;
        GraphicsData m_circData;
        GraphicsData m_planeData;
        GraphicsData m_boxData;
        GraphicsData m_sphereData;

        std::map<std::string, GraphicsData> m_meshData;
        // std::map<std::uint32_t, std::tuple<GraphicsData, TextureBase>> m_pcData;

    };

}