export module mz.graphics.renderer.camera;

import std;

import mz.core.behaviours;

import mz.util.geometry;

namespace mz {

    export class ICamera : ICastable
    {
    public:
		virtual ~ICamera() = default;

        const Mat4& getView() const;
        const Mat4& getProjection() const;
        const Mat4& getViewProjection() const;
    };



}