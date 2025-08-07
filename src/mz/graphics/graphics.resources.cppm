module;
#include "mz/core/core.h"
export module mz.graphics.resources;

import std;
import glm;

import mz.core.logging;
import mz.core.behaviours;

import mz.graphics.buffers;

namespace mz { 

    export class IGraphicsResource : public ICastable
    {
    public:
        virtual ~IGraphicsResource() = default;

        virtual void bind() const = 0;
        virtual void release() const = 0;
    };

    //------------------------------------------------------
    //                      VertexArray
    //------------------------------------------------------

    export class VertexArrayBase : public IGraphicsResource
    {
    public:
        virtual ~VertexArrayBase() = default;

        virtual void addVertexBuffer(const std::shared_ptr<VertexBufferBase>& vertexBuffer) = 0;
        virtual void setIndexBuffer(const std::shared_ptr<IndexBufferBase>& indexBuffer) = 0;

        inline const std::vector<std::shared_ptr<VertexBufferBase>>& getVertexBuffers() const { return m_vertexBuffers; }
        inline const std::shared_ptr<IndexBufferBase>& getIndexBuffer() const { return m_indexBuffer; }

    protected:
        std::vector<std::shared_ptr<VertexBufferBase>> m_vertexBuffers;
        std::shared_ptr<IndexBufferBase> m_indexBuffer;

    };

    //------------------------------------------------------
    //                      Shader
    //------------------------------------------------------

    export enum class ShaderError
    {
        InvalidFile,
        CompileError,
        LinkError
    };

    export enum class ShaderType
    {
        VertexShader,
        FragmentShader
    };

    export class ShaderBase : public IGraphicsResource
    {
    public:
        ShaderBase(const std::string& name) : m_name{ name } { }
        virtual ~ShaderBase() = default;

        virtual std::expected<void, ShaderError> link() = 0;

        virtual std::expected<void, ShaderError> add(const ShaderType type, const std::string& source) = 0;
        virtual std::expected<void, ShaderError> addFromFile(const ShaderType type, const std::filesystem::path& filePath) = 0;

        const std::string& getName() const { return m_name; }

    protected:
        std::string m_name;

    };

    export class ShaderStoreBase
    {
    public:
        virtual ~ShaderStoreBase() = default;

        virtual std::expected<std::shared_ptr<ShaderBase>, ShaderError> loadFromSource(const std::string& name, const std::string& vertSource, const std::string& fragSource) = 0;
        virtual std::expected<std::shared_ptr<ShaderBase>, ShaderError> loadFromFiles(const std::string& filePath) = 0;
        
        void add(const std::shared_ptr<ShaderBase>& shader)
        {
            const auto name = shader->getName();
            MZ_ASSERT(m_shaders.find(name) == m_shaders.end(), "shader already exists");
            m_shaders[name] = shader;
        }
        
        std::shared_ptr<ShaderBase> get(const std::string& name)
        {
            MZ_ASSERT(m_shaders.find(name) != m_shaders.end(), "shader not found");
            return m_shaders[name];
        }

        bool exists(const std::string& name) { return m_shaders.find(name) != m_shaders.end(); }
        void clear() { m_shaders.clear(); }

    protected:
        std::map<std::string, std::shared_ptr<ShaderBase>> m_shaders;

    };

    //------------------------------------------------------
    //                      Texture
    //------------------------------------------------------

    // export class TextureBase : public IGraphicsResource
    // {
    // public:
    //     virtual ~TextureBase() = default;
    //     // todo
    // };

    // export class TextureStoreBase
    // {
    // public:
    //     virtual ~TextureStoreBase() = default;

    //     // virtual std::expected<std::shared_ptr<ShaderBase>, ShaderError> loadFromSource(const std::string& name, const std::string& vertSource, const std::string& fragSource) = 0;
    //     // virtual std::expected<std::shared_ptr<ShaderBase>, ShaderError> loadFromFiles(const std::string& filePath) = 0;
        
    //     void add(const std::shared_ptr<TextureBase>& texture)
    //     {
    //         const auto name = texture->getName();
    //         MZ_ASSERT(m_textures.find(name) == m_textures.end(), "texture already exists");
    //         m_textures[name] = texture;
    //     }
        
    //     std::shared_ptr<ShaderBase> get(const std::string& name)
    //     {
    //         MZ_ASSERT(m_textures.find(name) != m_textures.end(), "texture not found");
    //         return m_textures[name];
    //     }

    //     bool exists(const std::string& name) { return m_textures.find(name) != m_textures.end(); }
    //     void clear() { m_textures.clear(); }

    // protected:
    //     std::map<std::string, std::shared_ptr<TextureBase>> m_textures;

    // };

}