module;
#include "mz/core/core.h"
export module mz.graphics.renderer.resources;

import std;

import mz.core.logging;
import mz.core.types;

import mz.graphics.renderer.buffers;

import mz.math.geometry;

namespace mz { 

    export class IRenderResource : public ICastable
    {
    public:
        virtual ~IRenderResource() = default;

        virtual void bind() const = 0;
        virtual void release() const = 0;
    };

    //------------------------------------------------------
    //                      VertexArray
    //------------------------------------------------------

    export class VertexArrayBase : public IRenderResource
    {
    public:
        using VertexBufferPtr = std::shared_ptr<VertexBufferBase>;

        virtual ~VertexArrayBase() = default;

        virtual void addVertexBuffer(const std::shared_ptr<VertexBufferBase>& vertexBuffer) = 0;
        virtual void setIndexBuffer(const std::shared_ptr<IndexBufferBase>& indexBuffer) = 0;

        inline const Vector<VertexBufferPtr>& getVertexBuffers() const { return m_vertexBuffers; }
        inline const std::shared_ptr<IndexBufferBase>& getIndexBuffer() const { return m_indexBuffer; }

    protected:
        Vector<VertexBufferPtr> m_vertexBuffers;
        std::shared_ptr<IndexBufferBase> m_indexBuffer;

    };

    //------------------------------------------------------
    //                      Shader
    //------------------------------------------------------

    export enum class ShaderError
    {
        InvalidFile,
        InvalidType,
        CompileError,
        LinkError,
        UniformNotFound
    };

    export enum class ShaderType
    {
        VertexShader,
        FragmentShader
    };

    export class IShader : public IRenderResource
    {
    public:
        virtual ~IShader() = default;

        virtual std::expected<void, ShaderError> link() = 0;

        virtual std::expected<void, ShaderError> add(const ShaderType type, const std::string& source) = 0;
        virtual std::expected<void, ShaderError> addFromFile(const ShaderType type, const std::filesystem::path& filePath) = 0;

        virtual std::expected<void, ShaderError> uploadInt(const std::string& name, const int i) const = 0;
        virtual std::expected<void, ShaderError> uploadFloat(const std::string& name, const float f) const = 0;
 
        virtual std::expected<void, ShaderError> uploadVec2(const std::string& name, const Vec2& vec) const = 0;
        virtual std::expected<void, ShaderError> uploadVec3(const std::string& name, const Vec3& vec) const = 0;
        virtual std::expected<void, ShaderError> uploadVec4(const std::string& name, const Vec4& vec) const = 0;
 
        virtual std::expected<void, ShaderError> uploadMat3(const std::string& name, const Mat3& mat) const = 0;
        virtual std::expected<void, ShaderError> uploadMat4(const std::string& name, const Mat4& mat) const = 0;
    };

    export class ShaderBase : public IShader
    {
    public:
        ShaderBase(const std::string& name) : m_name{ name } { }
        virtual ~ShaderBase() = default;

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

    export enum class ImageFormat
    {
        Rgb8 = 0,
        Rgba8,
        Y8,
        Y10BPack
    };

    export class ITexture : public IRenderResource
    {
    public:
        virtual ~ITexture() = default;

        virtual std::uint32_t getWidth() const = 0;
        virtual std::uint32_t getHeight() const = 0;

        virtual void setData(const ContigData<std::uint8_t>& data, const ImageFormat format) = 0;
    };

    export class TextureBase : public ITexture
    {
    public:
        TextureBase(const std::uint32_t width, const std::uint32_t height) : m_width{ width }, m_height{ height } {}
        virtual ~TextureBase() = default;

        std::uint32_t getWidth() const override { return m_width; }
        std::uint32_t getHeight() const override { return m_height; }

    protected:   
        std::uint32_t m_width;
        std::uint32_t m_height;

    };

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