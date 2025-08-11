export module mz.graphics.renderer.camera;

import std;
import glm;

import mz.core.types;

import mz.math.geometry;

namespace mz {

    export class ICamera : ICastable
    {
    public:
		virtual ~ICamera() = default;

        virtual const Mat4& getView() const = 0;
        virtual const Mat4& getProjection() const = 0;
        virtual const Mat4& getViewProjection() const = 0;
        
    };

    export class OrthoCamera : public ICamera
    {
    public:
        OrthoCamera(const float left, const float right, const float bottom, const float top)
            :  m_viewMatrix {1.0f}, m_projectionMatrix{ Mat4::createOrtho(left, right, bottom, top, -1.0f, 1.0f) },
            m_viewProjectionMatrix{ m_projectionMatrix * m_viewMatrix }, m_position{ 0.0f }, m_rotation{ 0.0f } {}

        void setProjection(float left, float right, float bottom, float top) 
        { 
            m_projectionMatrix = Mat4::createOrtho(left, right, bottom, top, -1.0f, 1.0f); 
            m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        }

        const Vec3& getPosition() const { return m_position; }
		void setPosition(const Vec3& position) { m_position = position; recalculateViewMatrix(); }

        const Vec3& getRotation() const { return m_rotation; }
		void setRotation(const Vec3& rotation) { m_rotation = rotation; recalculateViewMatrix(); }

        const Mat4& getView() const override { return m_viewMatrix; }
        const Mat4& getProjection() const override { return m_projectionMatrix; }
        const Mat4& getViewProjection() const override { return m_viewProjectionMatrix; }

    private:
        void recalculateViewMatrix()
        {
            auto transform = Mat4(1.0f);
            transform.translate(m_position);
            transform = transform * Mat4::createEulerXYZ(m_rotation);

            m_viewMatrix = glm::inverse(transform);
            m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        }

        Mat4 m_viewMatrix;
		Mat4 m_projectionMatrix;
		Mat4 m_viewProjectionMatrix;

        Vec3 m_position;
		Vec3 m_rotation;
    };

}