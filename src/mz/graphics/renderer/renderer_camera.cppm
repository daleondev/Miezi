export module mz.graphics.renderer.camera;

import std;
import glm;

import mz.core.types;
import mz.math.geometry;

namespace mz {

    //------------------------------------------------------
    //                      Camera
    //------------------------------------------------------

    export class ICamera : ICastable
    {
    public:
		virtual ~ICamera() = default;
        
        virtual const Vec3& getPosition() const = 0;
        virtual void setPosition(const Vec3& position) = 0;

        virtual const Vec3& getRotation() const = 0;
        virtual void setRotation(const Vec3& rotation) = 0;

        virtual const Mat4& getView() const = 0;
        virtual const Mat4& getProjection() const = 0;
        virtual const Mat4& getViewProjection() const = 0;

        virtual void setProjection(const float, const float, const float, const float) = 0;
    };

    export class CameraBase : public ICamera
    {
    public:
        CameraBase(const Mat4& projectionMatrix)
            : m_viewMatrix {1.0f}, m_projectionMatrix{ projectionMatrix }, m_viewProjectionMatrix{ m_projectionMatrix * m_viewMatrix },
            m_position{ 0.0f }, m_rotation{ 0.0f } {}
        virtual ~CameraBase() = default;

        const Vec3& getPosition() const override { return m_position; }
		void setPosition(const Vec3& position) override { m_position = position; recalculateViewMatrix(); }

        const Vec3& getRotation() const override { return m_rotation; }
		void setRotation(const Vec3& rotation) override { m_rotation = rotation; recalculateViewMatrix(); }

        const Mat4& getView() const override { return m_viewMatrix; }
        const Mat4& getProjection() const override { return m_projectionMatrix; }
        const Mat4& getViewProjection() const override { return m_viewProjectionMatrix; }

    protected:
        void recalculateViewMatrix()
        {
            auto transform = Mat4(1.0f);
            transform.translate(m_position);
            transform = transform * Mat4::createEulerXYZ(m_rotation);

            m_viewMatrix = transform.inverted();
            m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        }

        Mat4 m_viewMatrix;
		Mat4 m_projectionMatrix;
		Mat4 m_viewProjectionMatrix;

        Vec3 m_position;
		Vec3 m_rotation;
    };

    export class OrthoCamera : public CameraBase
    {
    public:
        OrthoCamera(const float left, const float right, const float bottom, const float top)
            : CameraBase(Mat4::createOrtho(left, right, bottom, top, -1.0f, 1.0f)) {}

        void setProjection(float left, float right, float bottom, float top) override
        { 
            m_projectionMatrix = Mat4::createOrtho(left, right, bottom, top, -1.0f, 1.0f); 
            m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        }
    };

    export class PerspectiveCamera : public CameraBase
    {
    public:
        PerspectiveCamera(const float fovY, const float aspect, const float zNear, const float zFar)
            : CameraBase(Mat4::createPerspective(fovY, aspect, zNear, zFar)) {}

        void setProjection(const float fovY, const float aspect, const float zNear, const float zFar) override
        { 
            m_projectionMatrix = Mat4::createPerspective(fovY, aspect, zNear, zFar);
            m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        }
    };

    //------------------------------------------------------
    //                  Camera Controller
    //------------------------------------------------------

    class IInput
    {

    };

    export class ICameraController : public ICastable
    {
    public:
        virtual ~ICameraController() = default;

        virtual void update(float deltaTime, IInput* input) = 0;

        virtual std::shared_ptr<ICamera> getCamera() const = 0;
        virtual void setCamera(const std::shared_ptr<ICamera>& camera) = 0;
    };

    export class CameraControllerBase : public ICameraController
    {
    public:
        CameraControllerBase(const std::shared_ptr<ICamera>& camera) : m_camera{ camera } {}
        virtual ~CameraControllerBase() = default;

        virtual std::shared_ptr<ICamera> getCamera() const { return m_camera; }
        virtual void setCamera(const std::shared_ptr<ICamera>& camera) { m_camera = camera; }

    protected:
        std::shared_ptr<ICamera> m_camera;
    };

    export class OrbitCameraController : public CameraControllerBase
    {
    public:
        OrbitCameraController(const std::shared_ptr<ICamera>& camera, const Vec3& target = {0,0,0}, const float distance = 10.0f) 
        : CameraControllerBase(camera), m_target{ target }, m_distance{ distance }, m_yaw{ 0.0f }, m_pitch{ 0.0f } {}

        void update(float deltaTime, IInput* input) override
        {
            // // Example: Get mouse delta from input system
            // Vec2 mouseDelta = input->getMouseDelta();
            // m_yaw   += mouseDelta.x * m_sensitivity;
            // m_pitch += mouseDelta.y * m_sensitivity;

            // // Clamp pitch to avoid flipping
            // m_pitch = glm::clamp(m_pitch, -PI_F/2.0f + 0.1f, PI_F/2.0f - 0.1f);

            // // Create quaternion from yaw & pitch
            // glm::quat yawRot   = Quat::fromAxisAngle({0, 1, 0}, m_yaw);
            // glm::quat pitchRot = Quat::fromAxisAngle({1, 0, 0}, m_pitch);
            // glm::quat orientation = yawRot * pitchRot;

            // // Set camera rotation
            // m_camera->setRotation(orientation);

            // // Calculate position based on target & orientation
            // Vec3 offset = orientation * Vec3(0, 0, m_distance);
            // m_camera->setPosition(m_target - offset);
        }

    private:
        Vec3 m_target;
        float m_distance;
        float m_yaw;
        float m_pitch;
        float m_sensitivity = 0.002f;
    };

    export class FreeCameraController : public CameraControllerBase
    {
    public:
        FreeCameraController(const std::shared_ptr<ICamera>& camera) 
            : CameraControllerBase(camera), m_yaw{ 0.0f }, m_pitch{ 0.0f } {}

        void update(float deltaTime, IInput* input) override
        {
            // Vec2 mouseDelta = Input::getMouseDelta();
            // m_yaw   += mouseDelta.x * m_mouseSensitivity;
            // m_pitch += mouseDelta.y * m_mouseSensitivity;

            // // Clamp pitch to avoid looking upside-down
            // m_pitch = glm::clamp(m_pitch, -glm::half_pi<float>() + 0.1f, glm::half_pi<float>() - 0.1f);

            // // Create orientation quaternion
            // Quat yawRot   = Quat::fromAxisAngle({0, 1, 0}, m_yaw);
            // Quat pitchRot = Quat::fromAxisAngle({1, 0, 0}, m_pitch);
            // Quat orientation = yawRot * pitchRot;
            // m_camera->setRotation(orientation);

            // // Movement
            // Vec3 forward = orientation * Vec3(0, 0, -1);
            // Vec3 right   = orientation * Vec3(1, 0, 0);

            // Vec3 pos = getCamera()->getPosition();
            // if (Input::isKeyPressed(KEY_W)) pos += forward * m_moveSpeed * deltaTime;
            // if (Input::isKeyPressed(KEY_S)) pos -= forward * m_moveSpeed * deltaTime;
            // if (Input::isKeyPressed(KEY_A)) pos -= right * m_moveSpeed * deltaTime;
            // if (Input::isKeyPressed(KEY_D)) pos += right * m_moveSpeed * deltaTime;
            // m_camera->setPosition(pos);
        }

    private:
        float m_yaw;
        float m_pitch;
        float m_moveSpeed = 5.0f;
        float m_mouseSensitivity = 0.002f;
    };

}