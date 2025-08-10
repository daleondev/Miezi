module;
#include "mz/core/core.h"

#include <glad/gl.h>
export module mz.graphics.renderer.opengl.buffers;

import std;

import mz.core.types;
import mz.core.logging;

import mz.graphics.renderer.buffers;

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

        void allocate(const IIterable<float>* vertices) override
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
            glBufferData(GL_ARRAY_BUFFER, vertices->size()*sizeof(GLfloat), vertices->data(), GL_STATIC_DRAW);
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

        void allocate(const IIterable<std::uint32_t>* indices) override
        {
            m_count = indices->size();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size()*sizeof(GLuint), indices->data(), GL_STATIC_DRAW);
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

    

}