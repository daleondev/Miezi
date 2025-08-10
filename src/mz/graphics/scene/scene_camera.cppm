export module mz.graphics.scene.camera;

import std;

import mz.math.geometry;

namespace mz {

    export class SceneCamera
    {
    public:
        enum class ProjectionType
        { 
            Perspective = 0, 
            Orthographic = 1 
        };

        SceneCamera() 
            : m_projectionType{ ProjectionType::Orthographic }, m_aspectRatio{ 0.0f },
            m_perspectiveFovY{ degToRad(45.0f) }, m_perspectiveZNear{ 0.01f }, m_PerspectiveZFar{ 1000.0f },
            m_OrthographicSize{ degToRad(10.0f) }, m_OrthographicZNear{ -1.0f }, m_OrthographicZFar{ 1.0f }
        {  

        }
		virtual ~SceneCamera() = default;

        void setPerspective(float fovY, float zNear, float zFar);
		void setOrthographic(float size, float zNear, float zFar);

        void setViewportSize(const Vec2& size);

        float getPerspectiveFovY() const { return m_perspectiveFovY; }
		void setPerspectiveFovY(const float fovY) { m_perspectiveFovY = fovY; RecalculateProjection(); }
		float getPerspectiveZNear() const { return m_perspectiveZNear; }
		void setPerspectiveZNear(const float zNear) { m_perspectiveZNear = zNear; RecalculateProjection(); }
		float getPerspectiveZFar() const { return m_PerspectiveZFar; }
		void setPerspectiveZFar(const float zFar) { m_PerspectiveZFar = zFar; RecalculateProjection(); }

        float getOrthographicSize() const { return m_OrthographicSize; }
		void setOrthographicSize(const float size) { m_OrthographicSize = size; RecalculateProjection(); }
		float getOrthographicZNear() const { return m_OrthographicZNear; }
		void setOrthographicZNear(const float zNear) { m_OrthographicZNear = zNear; RecalculateProjection(); }
		float getOrthographicZFar() const { return m_OrthographicZFar; }
		void setOrthographicZFar(const float zFar) { m_OrthographicZFar = zFar; RecalculateProjection(); }

        ProjectionType getProjectionType() const { return m_projectionType; }
		void setProjectionType(ProjectionType type) { m_projectionType = type; RecalculateProjection(); }

    private:
        void RecalculateProjection() {}

        ProjectionType m_projectionType;

        float m_aspectRatio;

        float m_perspectiveFovY;
		float m_perspectiveZNear;
        float m_PerspectiveZFar;

		float m_OrthographicSize;
		float m_OrthographicZNear;
        float m_OrthographicZFar;
    };

}