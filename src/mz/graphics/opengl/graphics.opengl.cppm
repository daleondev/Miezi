module;
// #include <GL/gl.h>
export module mz.graphics.opengl;

import mz.graphics;

namespace mz { 

    export class GlGraphicsContext : public IGraphicsContext
    {
    public:
        virtual ~GlGraphicsContext() = default;
    };

}