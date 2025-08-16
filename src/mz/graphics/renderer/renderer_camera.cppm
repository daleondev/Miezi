module;
#include "mz/core/core.h"

#include <GLFW/glfw3.h>
export module mz.graphics.renderer.camera;

import std;
import glm;

import mz.core.logging;
import mz.core.types;

import mz.graphics.window;
import mz.math.geometry;
import mz.util.time;

namespace mz {

    //------------------------------------------------------
    //                      Camera
    //------------------------------------------------------

    export enum class CameraType
    {
        Orthographic,
        Perspective
    };

    export class ICamera : public ICastable
    {
    public:
		virtual ~ICamera() = default;
        
        virtual const Vec3& getPosition() const = 0;
        virtual void setPosition(const Vec3& position) = 0;

        virtual const glm::quat& getRotation() const = 0;
        virtual void setRotation(const glm::quat& eulerXYZ) = 0;

        virtual const Vec3 getRotationEulerXYZ() const = 0;
        virtual void setRotationEulerXYZ(const Vec3& eulerXYZ) = 0;

        virtual const Mat4& getTransform() const = 0;

        virtual const Mat4& getView() const = 0;
        virtual const Mat4& getProjection() const = 0;
        virtual const Mat4& getViewProjection() const = 0;

        virtual CameraType getType() const = 0;
    };

    export class CameraBase : public ICamera
    {
    public:
        CameraBase(const Mat4& projectionMatrix)
            : m_viewMatrix {1.0f}, m_projectionMatrix{ projectionMatrix }, m_viewProjectionMatrix{ m_projectionMatrix * m_viewMatrix },
            m_position{ 0.0f }, m_rotation{ Mat3(1.0f) }, m_transform{ 1.0f } {}
        virtual ~CameraBase() = default;

        const Vec3& getPosition() const override { return m_position; }
		void setPosition(const Vec3& position) override { m_position = position; recalculateViewMatrix(); }

        const glm::quat& getRotation() const override { return m_rotation; }
		void setRotation(const glm::quat& rotation) override { m_rotation = rotation; recalculateViewMatrix(); }

        const Vec3 getRotationEulerXYZ() const override { return Mat3::fromQuat(m_rotation).toEulerXYZ(); }
		void setRotationEulerXYZ(const Vec3& eulerXYZ) override { m_rotation = Mat3::createEulerXYZ(eulerXYZ).toQuat(); recalculateViewMatrix(); }

        const Mat4& getTransform() const override { return m_transform; }

        const Mat4& getView() const override { return m_viewMatrix; }
        const Mat4& getProjection() const override { return m_projectionMatrix; }
        const Mat4& getViewProjection() const override { return m_viewProjectionMatrix; }

    protected:
        void recalculateViewMatrix()
        {
            m_transform = Mat4(1.0f);
            m_transform.translate(m_position).rotate(m_rotation);

            m_viewMatrix = m_transform.inverted();
            m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        }

        Mat4 m_viewMatrix;
		Mat4 m_projectionMatrix;
		Mat4 m_viewProjectionMatrix;

        Vec3 m_position;
		glm::quat m_rotation;
        Mat4 m_transform;
    };

    export class OrthoCamera : public CameraBase
    {
    public:
        OrthoCamera(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar)
            : CameraBase(Mat4::createOrtho(left, right, bottom, top, zNear, zFar)) {}

        void setProjection(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar)
        { 
            m_projectionMatrix = Mat4::createOrtho(left, right, bottom, top, zNear, zFar); 
            m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        }

        CameraType getType() const override { return CameraType::Orthographic; }
    };

    export class PerspectiveCamera : public CameraBase
    {
    public:
        PerspectiveCamera(const float fovY, const float aspect, const float zNear, const float zFar)
            : CameraBase(Mat4::createPerspective(fovY, aspect, zNear, zFar)) {}

        void setProjection(const float fovY, const float aspect, const float zNear, const float zFar)
        { 
            m_projectionMatrix = Mat4::createPerspective(fovY, aspect, zNear, zFar);
            m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        }

        CameraType getType() const override { return CameraType::Perspective; }
    };

    //------------------------------------------------------
    //                  Camera Controller
    //------------------------------------------------------

    export enum class CameraControllerType
    {
        Orbit,
        Free
    };

    export class ICameraController : public ICastable
    {
    public:
        virtual ~ICameraController() = default;

        virtual void update(const Timestep dt, IInput* input) = 0;

        virtual void startDraggingRot() = 0;
        virtual void stopDraggingRot() = 0;

        virtual void startDraggingTrans() = 0;
        virtual void stopDraggingTrans() = 0;

        virtual ICamera* getCamera() const = 0;
        virtual void setCamera(ICamera* camera) = 0;

        virtual CameraControllerType getType() const = 0;
    };

    export class CameraControllerBase : public ICameraController
    {
    public:
        CameraControllerBase(ICamera* camera) : m_camera{ camera } {}
        virtual ~CameraControllerBase() = default;

        void startDraggingRot() override { m_draggingRot = true; }
        void stopDraggingRot() override { m_draggingRot = false; }

        void startDraggingTrans() override { m_draggingTrans = true; }
        void stopDraggingTrans() override { m_draggingTrans = false; }

        ICamera* getCamera() const override { return m_camera; }
        void setCamera(ICamera* camera) override { m_camera = camera; }

    protected:
        ICamera* m_camera;

        bool m_draggingRot = false;
        bool m_draggingTrans = false;
    };

    export class OrbitCameraController : public CameraControllerBase
    {
    public:
        OrbitCameraController(ICamera* camera, const Vec3& target = {0,0,0}, const float distance = 4.0f) 
        : CameraControllerBase(camera), m_target{ target }, m_distance{ distance }, m_yaw{ 0.0f }, m_pitch{ 0.0f } {}

        void update(const Timestep dt, IInput* input) override
        {
            MZ_UNUSED(dt);

            if (m_draggingRot) {
                const auto mouseDelta = -input->getMouseDelta();
                m_yaw   += mouseDelta.x * m_rotSens;
                m_pitch += mouseDelta.y * m_rotSens;
                m_pitch = glm::clamp(m_pitch, -PI_F/2.0f + 0.1f, PI_F/2.0f - 0.1f);
            }

            if (m_draggingTrans) {
                const auto mouseDelta = input->getMouseDelta();
                const auto& transform = m_camera->getTransform();
                m_target += m_transSens * transform.xAxis() * -mouseDelta.x;
                m_target += m_transSens * transform.yAxis() * mouseDelta.y;
            }

            const glm::quat yawRot   = glm::angleAxis(m_yaw, Vec3::UnitY());
            const glm::quat pitchRot = glm::angleAxis(m_pitch, Vec3::UnitX());
            const glm::quat orientation = yawRot * pitchRot;
            m_camera->setRotation(orientation);

            const Vec3 offset = orientation * Vec3(0.0f, 0.0f, m_distance);
            m_camera->setPosition(m_target + offset);
        }

        CameraControllerType getType() const override { return CameraControllerType::Orbit; }

    private:
        Vec3 m_target;
        float m_distance;
        float m_yaw;
        float m_pitch;
        float m_transSens = 0.01f;
        float m_rotSens = 0.002f;
    };

    export class FreeCameraController : public CameraControllerBase
    {
    public:
        FreeCameraController(ICamera* camera) 
            : CameraControllerBase(camera), m_yaw{ 0.0f }, m_pitch{ 0.0f } {}

        void update(const Timestep dt, IInput* input) override
        {
            if (m_draggingRot) {
                Vec2 mouseDelta = input->getMouseDelta();
                m_yaw   += mouseDelta.x * m_mouseSensitivity;
                m_pitch += mouseDelta.y * m_mouseSensitivity;

                // Clamp pitch to avoid looking upside-down
                m_pitch = glm::clamp(m_pitch, -PI_F/2.0f + 0.1f, PI_F/2.0f - 0.1f);
            }

            // Create orientation quaternion
            glm::quat yawRot   = glm::angleAxis(m_yaw, Vec3::UnitY());
            glm::quat pitchRot = glm::angleAxis(m_pitch, Vec3::UnitX());
            glm::quat orientation = yawRot * pitchRot;
            m_camera->setRotation(orientation);

            // Movement
            Vec3 forward = orientation * -Vec3::UnitZ();
            Vec3 right   = orientation * Vec3::UnitX();

            Vec3 pos = getCamera()->getPosition();
            if (input->isKeyPressed(GLFW_KEY_W)) pos += forward * m_moveSpeed * (float)dt;
            if (input->isKeyPressed(GLFW_KEY_S)) pos -= forward * m_moveSpeed * (float)dt;
            if (input->isKeyPressed(GLFW_KEY_A)) pos -= right * m_moveSpeed * (float)dt;
            if (input->isKeyPressed(GLFW_KEY_D)) pos += right * m_moveSpeed * (float)dt;
            m_camera->setPosition(pos);  
        }

        CameraControllerType getType() const override { return CameraControllerType::Free; }

    private:
        float m_yaw;
        float m_pitch;
        float m_moveSpeed = 5.0f;
        float m_mouseSensitivity = 0.002f;
    };

}