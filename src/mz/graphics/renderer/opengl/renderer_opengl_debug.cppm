module;
#include "mz/core/core.h"

#include <glad/gl.h>
#include <GL/glu.h>
export module mz.graphics.renderer.opengl.debug;

import std;

import mz.core.types;
import mz.core.logging;

namespace mz {

    export void setupDebugging()
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback([](GLuint source, GLuint type, GLuint id, GLuint severity,
                                GLsizei length, const GLchar* message, const void* userParam) {
            MZ_UNUSED(source);
            MZ_UNUSED(userParam);

            switch (severity)
            {
                case GL_DEBUG_SEVERITY_HIGH: 
                    MZ_ERROR_LOC("GL Debug", "severity = 0x{:X}, type = 0x{:X}, id = 0x{:X}\n\tmsg = '{}'", severity, type, id, std::string(message, length));
                    break;
                case GL_DEBUG_SEVERITY_MEDIUM:
                    MZ_WARN_LOC("GL Debug", "severity = 0x{:X}, type = 0x{:X}, id = 0x{:X}\n\tmsg = '{}'", severity, type, id, std::string(message, length));
                    break;
                case GL_DEBUG_SEVERITY_LOW:
                    MZ_INFO_LOC("GL Debug", "severity = 0x{:X}, type = 0x{:X}, id = 0x{:X}\n\tmsg = '{}'", severity, type, id, std::string(message, length));
                    break;
                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    MZ_TRACE_LOC("GL Debug", "severity = 0x{:X}, type = 0x{:X}, id = 0x{:X}\n\tmsg = '{}'", severity, type, id, std::string(message, length));
                    break;
            }
        }, nullptr);
    }

    std::string getShaderInfoLog(GLuint shader) 
    {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        if (length <= 1) return {};

        std::string log(length, '\0');
        GLsizei written = 0;
        glGetShaderInfoLog(shader, length, &written, log.data());
        if (written > 0) log.resize(written);
        else log.clear();
        return log;
    }

    std::string getProgramInfoLog(GLuint program) 
    {
        GLint length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        if (length <= 1) return {};

        std::string log(length, '\0');
        GLsizei written = 0;
        glGetProgramInfoLog(program, length, &written, log.data());
        if (written > 0) log.resize(written);
        else log.clear();
        return log;
    }

    DynamicArray<GLuint> getAttachedShaders(GLuint program) 
    {
        GLint count = 0;
        glGetProgramiv(program, GL_ATTACHED_SHADERS, &count);
        if (count <= 0) return {};

        DynamicArray<GLuint> shaders((std::size_t)count);
        GLint fetched = 0;
        glGetAttachedShaders(program, count, &fetched, shaders.data());
        shaders.resize(fetched);
        return shaders;
    }

    std::string shaderTypeString(GLenum type)
    {
        switch (type)
        {
            case GL_VERTEX_SHADER:
                return "Vertex";
            case GL_FRAGMENT_SHADER:
                return "Fragment";
        }
        return "<undefined>";
    }

    export void shaderDiagnostics(GLuint program, const std::string& name) 
    {
        GLint linked = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);

        std::string log = getProgramInfoLog(program);
        MZ_TRACE("Program {}: id={}, linked={}, log='{}'", name, program, (linked == GL_TRUE), log);

        auto shaders = getAttachedShaders(program);
        MZ_TRACE("Attached shaders ({}):", shaders.size());
        for (GLuint s : shaders) {
            GLint type = 0;
            glGetShaderiv(s, GL_SHADER_TYPE, &type);

            GLint compiled = GL_FALSE;
            glGetShaderiv(s, GL_COMPILE_STATUS, &compiled);

            log = getShaderInfoLog(s);
            MZ_TRACE("\tShader {}: type = {}, compiled = {}, log = '{}'", s, shaderTypeString(type), (compiled == GL_TRUE), log);
        }

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            MZ_ERROR("glGetError() returned 0x{:X} - {}", err, std::string(reinterpret_cast<const char*>(gluErrorString(err))));
    }

}