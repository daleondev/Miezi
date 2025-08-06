export module mz.graphics;

import mz.core.behaviours;

namespace mz { 

    export class IGraphicsContext : public ICastable
    {
    public:
        virtual ~IGraphicsContext() = default;

        virtual void makeCurrent() = 0;
        virtual void swapBuffers() = 0;
    };

    export class IGraphicsObject : public ICastable
    {
    public:
        virtual ~IGraphicsObject() = default;

        virtual void bind() const = 0;
        virtual void release() const = 0;
    };

}