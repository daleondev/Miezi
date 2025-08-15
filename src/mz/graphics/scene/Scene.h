#pragma once
#include "mz/core/core.h"

#include <entt/entt.hpp>

import std;

import mz.core.logging;

import mz.graphics.scene.entity;
import mz.graphics.scene.components;
import mz.graphics.scene.component_utils;

import mz.util;

namespace mz {

    class Scene
    {
    public:
        using SceneEntityIterator = std::map<UUID, Entity>::iterator;

        Scene();
        ~Scene() = default;

        void update(const Timestep dt);
        
        Entity createEntity(const std::string& tag, const bool addToMap = true);
        void destroyEntity(const Entity entity);
        SceneEntityIterator destroyEntity(const SceneEntityIterator it);

    private:
        entt::registry m_registry;
        std::map<UUID, Entity> m_entities;
    };

}