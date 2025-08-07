module;
#include "mz/core/core.h"

#include <glad/gl.h>
export module mz.graphics.opengl.buffer;

import std;

import mz.core.logging;
import mz.graphics.buffer;

namespace mz { 

    export class GlVertexBuffer : public VertexBufferBase
    {
    public:
        GlVertexBuffer()
        {
            glGenBuffers(1, &m_buffer);
        }

        ~GlVertexBuffer()
        {
            glDeleteBuffers(1, &m_buffer);
        }

        void allocate(const std::vector<float>& vertices) override
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
            glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
        }

        void bind() const override
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
        }

        void release() const override
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

    private:
        GLuint m_buffer;

    };

    export class GlIndexBuffer : public IndexBufferBase
    {
    public:
        GlIndexBuffer()
        {
            glGenBuffers(1, &m_buffer);
        }

        ~GlIndexBuffer()
        {
            glDeleteBuffers(1, &m_buffer);
        }

        void allocate(const std::vector<std::uint32_t>& indices) override
        {
            m_count = indices.size();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        }

        void bind() const override
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
        }

        void release() const override
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

    private:
        GLuint m_buffer;

    };

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

}