module;
#include <cassert>
export module mz.graphics.shader;

import std;

import mz.graphics;

namespace mz { 

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

    export class ShaderBase : public IGraphicsObject
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
            assert(s_shaders.find(name) == s_shaders.end() && "shader already exists");
            s_shaders[name] = shader;
        }
        
        std::shared_ptr<ShaderBase> get(const std::string& name)
        {
            assert(s_shaders.find(name) != s_shaders.end() && "shader not found");
            return s_shaders[name];
        }

        bool exists(const std::string& name) { return s_shaders.find(name) != s_shaders.end(); }
        void clear() { s_shaders.clear(); }

    protected:
        std::map<std::string, std::shared_ptr<ShaderBase>> s_shaders;

    };

}