module;
#include "mz/core/core.h"

#include <glad/gl.h>
export module mz.graphics.opengl;

import std;

import mz.core.logging;
import mz.graphics;

namespace mz { 

    export class GlGraphicsContext : public IGraphicsContext
    {
    public:
        GlGraphicsContext() : m_initialized{ false } {}
        virtual ~GlGraphicsContext() = default;

        void init(GLADloadfunc loadFunc)
        {
            if (!m_initialized) {
                int version = gladLoadGL(loadFunc);
                MZ_ASSERT(version != 0, "Failed to initialize GLAD");
                MZ_INFO("OpenGL:");
                MZ_INFO("\tOpenGL Vendor: {}", (char*)glGetString(GL_VENDOR));
                MZ_INFO("\tOpenGL Renderer: {}", (char*)glGetString(GL_RENDERER));
                MZ_INFO("\tOpenGL Version: {}", (char*)glGetString(GL_VERSION));
                MZ_INFO("\tGLSL Version: {}", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
                MZ_INFO("\tGlad Version: {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
                m_initialized = true;
            }
        }

    private:
        bool m_initialized;

    };

}