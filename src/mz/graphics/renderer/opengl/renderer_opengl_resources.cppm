module;
#include "mz/core/core.h"

#include <glad/gl.h>
export module mz.graphics.renderer.opengl.resources;

import std;

import mz.core.logging;

import mz.graphics.renderer.resources;
import mz.graphics.renderer.buffers;
import mz.graphics.renderer.opengl.debug;
export import mz.graphics.renderer.opengl.buffers;

import mz.util;
import mz.math.geometry;

namespace mz {

    static GLenum ShaderDataTypeToOpenGLBaseType(const ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Bool:       return GL_BOOL;
            case ShaderDataType::Float:      return GL_FLOAT;
            case ShaderDataType::Float2:     return GL_FLOAT;
            case ShaderDataType::Float3:     return GL_FLOAT;
            case ShaderDataType::Float4:     return GL_FLOAT;
            case ShaderDataType::Float2x2:   return GL_FLOAT;
            case ShaderDataType::Float3x3:   return GL_FLOAT;
            case ShaderDataType::Float4x4:   return GL_FLOAT;
            case ShaderDataType::Int:        return GL_INT;
            case ShaderDataType::Int2:       return GL_INT;
            case ShaderDataType::Int3:       return GL_INT;
            case ShaderDataType::Int4:       return GL_INT;
            case ShaderDataType::Int2x2:     return GL_INT;
            case ShaderDataType::Int3x3:     return GL_INT;
            case ShaderDataType::Int4x4:     return GL_INT;
            case ShaderDataType::None:       return GL_NONE;
        }

        MZ_ASSERT(false, "Unknown shader data type");
        return 0;
    }

    //------------------------------------------------------
    //                      VertexArray
    //------------------------------------------------------

    export class GlVertexArray : public VertexArrayBase
    {
    public:
        GlVertexArray()
        {
            glGenVertexArrays(1, &m_array);
        }

        ~GlVertexArray()
        {
            glDeleteVertexArrays(1, &m_array);
        }

        void addVertexBuffer(const std::shared_ptr<VertexBufferBase>& vertexBuffer) override
        {
            MZ_ASSERT(vertexBuffer->is<GlVertexBuffer>(), "Invalid Vertex Buffer type");

            const auto& layout = vertexBuffer->getLayout();
            MZ_ASSERT(!layout.getElements().empty(), "Vertex buffer has no layout!");

            glBindVertexArray(m_array);
            vertexBuffer->bind();     

            std::size_t index = 0;
            for (const auto& element : layout) {
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(  index++, element.getComponentCount(), ShaderDataTypeToOpenGLBaseType(element.type), 
                                        (GLboolean)element.normalized, layout.getStride(), (const void*)element.offset);
            }

            m_vertexBuffers.push_back(vertexBuffer);
        }

        void setIndexBuffer(const std::shared_ptr<IndexBufferBase>& indexBuffer) override
        {
            MZ_ASSERT(indexBuffer->is<GlIndexBuffer>(), "Invalid Index Buffer type");

            glBindVertexArray(m_array);
            indexBuffer->bind();

            m_indexBuffer = indexBuffer;
        }

        void bind() const override
        {
            glBindVertexArray(m_array);
        }

        void release() const override
        {
            glBindVertexArray(0);
        }

    private:
        GLuint m_array;

    };

    //------------------------------------------------------
    //                      Shader
    //------------------------------------------------------

    export class GlShader : public ShaderBase
    {
    public:
        GlShader(const std::string& name) : ShaderBase(name), m_fragmentShader{ 0 }, m_vertexShader{ 0 }, m_program{ 0 } 
        {
            m_program = glCreateProgram();
        }
        ~GlShader()
        {
            glDeleteProgram(m_program);
        }

        void bind() const override
        {
            glUseProgram(m_program);
        }

        void release() const override
        {
            glUseProgram(0);
        }

        std::expected<void, ShaderError> link() override
        {
            glLinkProgram(m_program);
            
            shaderDiagnostics(m_program, m_name.c_str());

            GLint linked = 0;
            glGetProgramiv(m_program, GL_LINK_STATUS, &linked);

            if (linked == GL_FALSE) {
                glDeleteProgram(m_program);
                glDeleteShader(m_fragmentShader);
                glDeleteShader(m_vertexShader);

                MZ_ERROR("Linking Shader {} failed", m_name);
                return std::unexpected(ShaderError::LinkError);
            }

            glDetachShader(m_program, m_fragmentShader);
            glDetachShader(m_program, m_vertexShader);
            glDeleteShader(m_fragmentShader);
            glDeleteShader(m_vertexShader);

            return {};
        }

        std::expected<void, ShaderError> add(const ShaderType type, const std::string& source) override
        {
            GLenum shaderType;
            switch (type) {
                case ShaderType::VertexShader: shaderType = GL_VERTEX_SHADER; break;
                case ShaderType::FragmentShader: shaderType = GL_FRAGMENT_SHADER; break;
                default: return std::unexpected(ShaderError::InvalidType);
            }

            if (!compile(shaderType, source.c_str()))
                return std::unexpected(ShaderError::CompileError);

            return {};
        }

        std::expected<void, ShaderError> addFromFile(const ShaderType type, const std::filesystem::path& filePath) override
        {
            std::string shaderSource;
            if (!std::filesystem::exists(filePath) || (shaderSource = readFile(filePath)).empty()) {
                MZ_ERROR("Invalid shader file provided: {}", filePath.string());
                return std::unexpected(ShaderError::InvalidFile);
            }

            GLenum shaderType;
            switch (type) {
                case ShaderType::VertexShader: shaderType = GL_VERTEX_SHADER; break;
                case ShaderType::FragmentShader: shaderType = GL_FRAGMENT_SHADER; break;
                default: return std::unexpected(ShaderError::InvalidType);
            }

            if (!compile(shaderType, shaderSource.c_str()))
                return std::unexpected(ShaderError::CompileError);

            return {};
        }

        std::expected<void, ShaderError> uploadInt(const std::string& name, const int i) const override
        {
            GLint location = glGetUniformLocation(m_program, name.c_str());
            if (location == -1) {
                MZ_ERROR("Uniform {} doesnt exist", name);
                return std::unexpected(ShaderError::UniformNotFound);
            }

            glUniform1i(location, i);
            return {};
        }

        std::expected<void, ShaderError> uploadFloat(const std::string& name, const float f) const override
        {
            GLint location = glGetUniformLocation(m_program, name.c_str());
            if (location == -1) {
                MZ_ERROR("Uniform {} doesnt exist", name);
                return std::unexpected(ShaderError::UniformNotFound);
            }

            glUniform1f(location, f);
            return {};
        }
 
        std::expected<void, ShaderError> uploadVec2(const std::string& name, const Vec2& vec) const override
        {
            GLint location = glGetUniformLocation(m_program, name.c_str());
            if (location == -1) {
                MZ_ERROR("Uniform {} doesnt exist", name);
                return std::unexpected(ShaderError::UniformNotFound);
            }

            glUniform2fv(location, 1, vec.data());
            return {};
        }

        std::expected<void, ShaderError> uploadVec3(const std::string& name, const Vec3& vec) const override
        {
            GLint location = glGetUniformLocation(m_program, name.c_str());
            if (location == -1) {
                MZ_ERROR("Uniform {} doesnt exist", name);
                return std::unexpected(ShaderError::UniformNotFound);
            }
            
            glUniform3fv(location, 1, vec.data());
            return {};
        }

        std::expected<void, ShaderError> uploadVec4(const std::string& name, const Vec4& vec) const override
        {
            GLint location = glGetUniformLocation(m_program, name.c_str());
            if (location == -1) {
                MZ_ERROR("Uniform {} doesnt exist", name);
                return std::unexpected(ShaderError::UniformNotFound);
            }

            glUniform4fv(location, 1, vec.data());
            return {};
        }
 
        std::expected<void, ShaderError> uploadMat3(const std::string& name, const Mat3& mat) const override
        {
            GLint location = glGetUniformLocation(m_program, name.c_str());
            if (location == -1) {
                MZ_ERROR("Uniform {} doesnt exist", name);
                return std::unexpected(ShaderError::UniformNotFound);
            }

            glUniformMatrix3fv(location, 1, GL_FALSE, mat.data());
            return {};
        }

        std::expected<void, ShaderError> uploadMat4(const std::string& name, const Mat4& mat) const override
        {
            GLint location = glGetUniformLocation(m_program, name.c_str());
            if (location == -1) {
                MZ_ERROR("Uniform {} doesnt exist", name);
                return std::unexpected(ShaderError::UniformNotFound);
            }

            glUniformMatrix4fv(location, 1, GL_FALSE, mat.data());
            return {};
        }

    private:
        bool compile(const GLenum type, const GLchar* source)
        {
            GLuint shader;

            shader = glCreateShader(type);
            glShaderSource(shader, 1, &source, 0);
            glCompileShader(shader);

            GLint compiled;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (compiled == GL_FALSE) {
                GLint length = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

                std::string msg(length, '\0');
                glGetShaderInfoLog(shader, length, &length, msg.data());

                glDeleteShader(shader);

                MZ_ERROR("Shader compilation failed: {}", msg);
                return false;
            }

            glAttachShader(m_program, shader);

            switch (type) {
                case GL_FRAGMENT_SHADER: m_fragmentShader = shader; break;
                case GL_VERTEX_SHADER: m_vertexShader = shader; break;
            }

            return true;
        }

        GLuint m_fragmentShader;
        GLuint m_vertexShader;
        GLuint m_program;

    };

    export class GlShaderStore : public ShaderStoreBase
    {
    public:
        std::expected<std::shared_ptr<ShaderBase>, ShaderError> loadFromSource(const std::string& name, const std::string& vertSource, const std::string& fragSource) override
        {
            MZ_ASSERT(!exists(name), "Shader already loaded");

            std::shared_ptr<ShaderBase> shader = std::make_shared<GlShader>(name);
            shader->add(ShaderType::VertexShader, vertSource.c_str());
            shader->add(ShaderType::FragmentShader, fragSource.c_str());
            shader->link();

            add(shader);
            return shader;
        }
        
        std::expected<std::shared_ptr<ShaderBase>, ShaderError> loadFromFiles(const std::string& filePath) override
        {
            const std::filesystem::path vertFile = filePath + ".vert";
            if (!std::filesystem::exists(vertFile)) {
                MZ_ERROR("Invalid shader file provided: {}", vertFile.string());
                return std::unexpected(ShaderError::InvalidFile);
            }

            const std::filesystem::path fragFile = filePath + ".frag";
            if (!std::filesystem::exists(fragFile)) {
                MZ_ERROR("Invalid shader file provided: {}", fragFile.string());
                return std::unexpected(ShaderError::InvalidFile);
            }
            
            const std::string name = vertFile.filename().stem().string();
            MZ_ASSERT(!exists(name), "Shader already loaded");

            std::shared_ptr<ShaderBase> shader = std::make_shared<GlShader>(name);

            auto result = shader->addFromFile(ShaderType::VertexShader, vertFile);
            if (!result) 
                return std::unexpected(result.error());

            result = shader->addFromFile(ShaderType::FragmentShader, fragFile);
            if (!result) 
                return std::unexpected(result.error());

            result = shader->link();
            if (!result) 
                return std::unexpected(result.error());

            add(shader);
            return shader;
        }
    };

}