module;
#include "mz/core/core.h"
#include "mz/events/events.h"

#include <GLFW/glfw3.h> // todo: mousebuttons / keys
export module mz.graphics.scene;

import std;

import mz.core.logging;

import mz.events;
import mz.events.window;
import mz.events.key;
import mz.events.mouse;

import mz.graphics.window;

import mz.graphics.renderer;
import mz.graphics.renderer.camera;

import mz.graphics.scene.entity;
import mz.graphics.scene.components;
import mz.graphics.scene.component_utils;

import mz.math.geometry;

import mz.util;
import mz.util.time;

namespace mz {

    template<typename Callback, typename Component>
    struct ConstructTrampoline {
        inline static std::decay_t<Callback> callback;

        static void handler(entt::registry& registry, entt::entity handle) {
            Entity entity(&registry, handle);
            callback(entity, entity.getComponent<Component>());
        }
    };

    template<typename... Components, typename Callback>
    void setComponentsConstructCallback(entt::registry& registry, Callback&& callback) {
        ( ..., [&] {
            using tramp = ConstructTrampoline<Callback, Components>;
            tramp::callback = std::forward<Callback>(callback);
            registry.template on_construct<Components>()
            .template connect<&tramp::handler>();
        }() );
    }

    template<typename... Components, typename Callback>
	void setComponentsConstructCallback(ComponentGroup<Components...>, entt::registry& registry, Callback&& callback)
	{
		setComponentsConstructCallback<Components...>(registry, std::forward<Callback>(callback));
	}

    export class Scene
    {
    public:
        using SceneEntityIterator = std::map<UUID, Entity>::iterator;

        Scene(const Vec2& size, const std::shared_ptr<IRenderer>& renderer) 
            : m_size{ size }, m_renderer{ renderer }
        {
            setComponentsConstructCallback(RendererComponents{}, m_registry, [](Entity entity, auto component) 
            {
                entity.addComponentIfNotExists<TransformComponent>(); 
            });

            setComponentsConstructCallback<CameraComponent>(m_registry, [](Entity entity, auto component) 
            {
                entity.addComponentIfNotExists<TransformComponent>(); 
            });

            auto camera = createEntity("SceneCamera");
            camera.addComponent<CameraComponent>();
        }
        ~Scene() = default;

        void update(const Timestep dt, IInput *input)
        {
            m_renderer->clear(Vec4(0.0f));

            {
                auto view = m_registry.view<TransformComponent, CameraComponent>();
                for (auto [entity, transformComponent, cameraComponent] : view.each()) {
                    if (cameraComponent.primary) {
                        // create camera if not exists
                        if (!m_camera) {
                            switch (cameraComponent.cameraType) {
                                case CameraType::Orthographic:  m_camera = std::make_unique<OrthoCamera>(-1.0f, 1.0f, -1.0f, 1.0f, 0.001f, 100.0f); break;
                                case CameraType::Perspective:   m_camera = std::make_unique<PerspectiveCamera>(degToRad(60.0f), m_size.x / m_size.y, 0.001f, 100.0f); break;
                            }
                        }
                        // set correct camera type
                        else {
                            if (cameraComponent.cameraType == CameraType::Orthographic && !m_camera->is<OrthoCamera>())
                                m_camera = std::make_unique<OrthoCamera>(-1.0f, 1.0f, -1.0f, 1.0f, 0.001f, 100.0f);
                            else if (cameraComponent.cameraType == CameraType::Perspective && !m_camera->is<PerspectiveCamera>())
                                m_camera = std::make_unique<PerspectiveCamera>(degToRad(60.0f), m_size.x / m_size.y, 0.001f, 100.0f);
                        }

                        // create cameracontroller if not exists
                        if (!m_cameraController && cameraComponent.controllerType) {
                            switch (*cameraComponent.controllerType) {
                                case CameraControllerType::Orbit:   m_cameraController = std::make_unique<OrbitCameraController>(m_camera.get()); break;
                                case CameraControllerType::Free:    m_cameraController = std::make_unique<FreeCameraController>(m_camera.get()); break;
                            }
                        }
                        // set correct cameracontroller type
                        else if (cameraComponent.controllerType) {
                            if (*cameraComponent.controllerType == CameraControllerType::Orbit && !m_cameraController->is<OrbitCameraController>())
                                m_cameraController = std::make_unique<OrbitCameraController>(m_camera.get());
                            else if (*cameraComponent.controllerType == CameraControllerType::Free && !m_cameraController->is<FreeCameraController>())
                                m_cameraController = std::make_unique<FreeCameraController>(m_camera.get());
                        }

                        // update camera transform
                        if (m_cameraController) {
                            m_cameraController->update(dt, input);
                            setTransform(transformComponent, m_camera->getTransform());
                        }
                        else {
                            m_camera->setPosition(transformComponent.translation);
                            m_camera->setRotation(transformComponent.rotation);
                        }

                        break;
                    }
                }
            }

            if (m_camera) {

                {
                    auto view = m_registry.view<TransformComponent, LineRendererComponent>();
                    for (auto [entity, transformComponent, lineRendererComponent] : view.each()) {
                        m_renderer->drawLine(m_camera.get(), transformComponent, lineRendererComponent.color);
                    }
                }

            }
        }

        Entity createEntity(const std::string& tag, const bool addToMap = true)
        {
            Entity entity(&m_registry);

            auto& idComponent = entity.addComponent<IdComponent>(entity);
            entity.addComponent<TagComponent>(tag.empty() ? "Entity" : tag);
            
            if (addToMap)
                m_entities.emplace(idComponent.id, entity);

            return entity;
        }

        void destroyEntity(const Entity entity)
        {
            m_entities.erase(entity.getId());
            m_registry.destroy(entity);
        }

        SceneEntityIterator destroyEntity(const SceneEntityIterator it)
        {
            m_registry.destroy(it->second);
            return m_entities.erase(it);
        }

        void onEvent(IEvent* e)
        {
            EventDispatcher dispatcher(e);
    
            // Window-Event
            if (e->getCategoryFlags() & static_cast<std::uint8_t>(EventCategory::Window)) {
                dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(onWindowResized));
            }
            // Mouse-Event
            else if (e->getCategoryFlags() & static_cast<std::uint8_t>(EventCategory::Mouse)) {
                dispatcher.dispatch<MouseLeaveEvent>(BIND_EVENT_FUNCTION(onMouseLeave));
                dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT_FUNCTION(onMouseMoved));
                dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(onMouseButtonPressed));
                dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FUNCTION(onMouseButtonReleased));    
                dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(onMouseScrolled));
                dispatcher.dispatch<MouseDroppedEvent>(BIND_EVENT_FUNCTION(onMouseDropped));
            }
        }

        bool onMouseLeave(MouseLeaveEvent* e)
        {
            if (m_cameraController) {
                m_cameraController->startDraggingTrans();
                m_cameraController->startDraggingRot();
            }

            return true;
        }

        bool onMouseMoved(MouseMovedEvent* e)
        {
            return true;
        }

        bool onMouseButtonPressed(MouseButtonPressedEvent* e)
        {
            if (m_cameraController) {
                switch(e->getMouseButton()) {
                    case GLFW_MOUSE_BUTTON_LEFT:
                    {
                        m_cameraController->startDraggingTrans();
                        break;
                    }
                    case GLFW_MOUSE_BUTTON_RIGHT:
                    {
                        m_cameraController->startDraggingRot();
                        break;
                    }
                }
            }

            return true;
        }

        bool onMouseButtonReleased(MouseButtonReleasedEvent* e)
        {
            if (m_cameraController) {
                switch(e->getMouseButton()) {
                    case GLFW_MOUSE_BUTTON_LEFT:
                    {
                        m_cameraController->stopDraggingTrans();
                        break;
                    }
                    case GLFW_MOUSE_BUTTON_RIGHT:
                    {
                        m_cameraController->stopDraggingRot();
                        break;
                    }
                }
            }

            return true;
        }

        bool onMouseScrolled(MouseScrolledEvent* e)
        {
            // todo: cameractrl zoom
            return true;
        }

        bool onMouseDropped(MouseDroppedEvent* e)
        {
            // todo
            return true;
        }

        bool onWindowResized(WindowResizeEvent* e)
        {
            m_size = e->getSize();
            m_renderer->setViewport(m_size);

            const float aspect = e->getWidth() / e->getHeight();
            if (m_camera) {
                if (m_camera->is<PerspectiveCamera>())
                    m_camera->asPtrUnchecked<PerspectiveCamera>()->setProjection(60.0f, aspect, 0.001f, 1000.0f);
            }
            
            return true;
        }


    private:
        entt::registry m_registry;
        std::map<UUID, Entity> m_entities;

        Vec2 m_size;
        std::shared_ptr<IRenderer> m_renderer;

        std::unique_ptr<ICamera> m_camera;
        std::unique_ptr<ICameraController> m_cameraController;
    };

}