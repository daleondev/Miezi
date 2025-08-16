#pragma once
#include "mz/core/core.h"

#include <entt/entt.hpp>

import std;

import mz.core.logging;

import mz.graphics.renderer;
import mz.graphics.renderer.camera;

import mz.graphics.scene.entity;
import mz.graphics.scene.components;
import mz.graphics.scene.component_utils;

import mz.util;
import mz.util.time;

namespace mz {

    class Scene
    {
    public:
        using SceneEntityIterator = std::map<UUID, Entity>::iterator;

        Scene(const std::shared_ptr<IRenderer>& renderer);
        ~Scene() = default;

        void update(const Timestep dt, ICamera* camera);
        
        Entity createEntity(const std::string& tag, const bool addToMap = true);
        void destroyEntity(const Entity entity);
        SceneEntityIterator destroyEntity(const SceneEntityIterator it);

        Entity createLineEntity(const std::string& tag, const bool addToMap = true);

    private:
        entt::registry m_registry;
        std::map<UUID, Entity> m_entities;

        std::shared_ptr<IRenderer> m_renderer;

    };

}