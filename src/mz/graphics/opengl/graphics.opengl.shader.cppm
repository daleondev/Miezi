module;
#include "mz/core/core.h"

#include <glad/gl.h>
#include <cassert>
export module mz.graphics.opengl.shader;

import std;
import glm;

import mz.core.logging;
import mz.graphics.shader;
import mz.util;

namespace mz {

    export class GlShader : public ShaderBase
    {
    public:
        GlShader(const std::string& name) : ShaderBase(name) {}
        ~GlShader() = default;

        std::expected<void, ShaderError> link() override
        {
            glLinkProgram(m_program);

            GLint linked = 0;
            glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
            if (linked == GL_FALSE) {
                GLint length = 0;
                glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);

                std::vector<GLchar> msg(length);
                glGetProgramInfoLog(m_program, length, &length, msg.data());

                glDeleteProgram(m_program);

                glDeleteShader(m_fragmentShader);
                glDeleteShader(m_vertexShader);

                MZ_ERROR("Shader linking failed: {}", msg.data());
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
            }

            if (!compile(shaderType, shaderSource.c_str()))
                return std::unexpected(ShaderError::CompileError);

            return {};
        }

        void bind() const override
        {
            glUseProgram(m_program);
        }

        void release() const override
        {
            glUseProgram(0);
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

                std::vector<GLchar> msg(length);
                glGetShaderInfoLog(shader, length, &length, msg.data());

                glDeleteShader(shader);

                MZ_ERROR("Shader compilation failed: {}", msg.data());
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
            assert(!exists(name) && "Shader already loaded");

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
            assert(!exists(name) && "Shader already loaded");

            std::shared_ptr<ShaderBase> shader = std::make_shared<GlShader>(name);
            shader->addFromFile(ShaderType::VertexShader, vertFile);
            shader->addFromFile(ShaderType::FragmentShader, fragFile);
            shader->link();

            add(shader);
            return shader;
        }
    };

}