#include "Scene.h"

import std;

import mz.graphics.renderer;
import mz.graphics.renderer.camera;

import mz.graphics.scene.entity;

namespace mz {
    Scene::Scene(const std::shared_ptr<IRenderer>& renderer)
        : m_renderer{ renderer }
    {
        // m_registry.on_construct<MeshRendererComponent>().connect<[](entt::registry& registry, Entity entity) -> void {       
        //     // entity.addComponentIfNotExists<PropertiesComponent>();
        //     // auto& transform = entity.addComponentIfNotExists<TransformComponent>();
        //     // auto& triangulation = entity.addComponentIfNotExists<TriangulationComponent>();       
        //     // auto& boundingBox = entity.addComponentIfNotExists<BoundingBoxComponent>();

        //     // updateTriangulation(triangulation, transform);
        //     // updateBoundingBox(boundingBox, triangulation.limits);
        // }>();
    }

    void Scene::update(const Timestep dt, ICamera* camera)
    {
        // todo framebuffer

        // draw lines
        {
            // auto group = m_registry.group<LineRendererComponent>(entt::get_t<TransformComponent>());
            // for (auto [entity, line, transform] : group.each()) {
            //     // detectTransformChange(transform);

            //     m_renderer->drawLine(camera, transform, line.color);
            // }

            auto view = m_registry.view<LineRendererComponent, TransformComponent>();

            // for (auto entity : ) {
            //     auto &lineRenderer = m_registry.get<LineRendererComponent>(entity);
            //     auto &transform = m_registry.get<TransformComponent>(entity);

                
            // }
        }
    }

    Entity Scene::createEntity(const std::string& tag, const bool addToMap)
    {
        Entity entity(&m_registry);

        auto& idComponent = entity.addComponent<IdComponent>(entity);
        entity.addComponent<TagComponent>(tag.empty() ? "Entity" : tag);
        
        if (addToMap)
            m_entities.emplace(idComponent.id, entity);

        return entity;
    }

    void Scene::destroyEntity(const Entity entity)
    {
        m_entities.erase(entity.getId());
        m_registry.destroy(entity);
    }

    Scene::SceneEntityIterator Scene::destroyEntity(const SceneEntityIterator it)
    {
        m_registry.destroy(it->second);
        return m_entities.erase(it);
    }

}