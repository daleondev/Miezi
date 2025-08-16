#pragma once
#include "entt/entity/fwd.hpp"
#include "mz/core/core.h"

#include <entt/entt.hpp>

import std;

import mz.core.logging;
import mz.graphics.scene.components;
import mz.util;

namespace mz{

    class Entity
    {
    public:
        Entity() = delete;
        Entity(entt::registry* registry) : m_registry{ registry }, m_handle{ registry ? registry->create() : entt::null } {}
        Entity(entt::registry* registry, const entt::entity handle) : m_registry{ registry }, m_handle{ handle } {}
        Entity(const Entity&) = default;
        ~Entity() = default;

        template<typename T, typename... Args>
        T& addComponent(Args&&... args)
        {
            MZ_ASSERT(!hasComponent<T>(), "Entity already has component!");
            return m_registry->emplace<T>(m_handle, std::forward<Args>(args)...);
        }

        template<typename T, typename... Args>
        T& addOrReplaceComponent(Args&&... args)
        {
            return m_registry->emplace_or_replace<T>(m_handle, std::forward<Args>(args)...);
        }

        template<typename T, typename... Args>
        T& addComponentIfNotExists(Args&&... args)
        {
            return hasComponent<T>() ? getComponent<T>() : m_registry->emplace_or_replace<T>(m_handle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& getComponent() const
        {
            MZ_ASSERT(hasComponent<T>(), "Entity does not have component!");
            return m_registry->get<T>(m_handle);
        }

        template<typename T>
        inline bool hasComponent() const { return m_registry->all_of<T>(m_handle); }

        template<typename T>
        void RemoveComponent()
        {
            MZ_ASSERT(hasComponent<T>(), "Entity does not have component!");
            m_registry->remove<T>(m_handle);
        }

        std::uint32_t getId() const
        { 
            return getComponent<IdComponent>(); 
        }

        std::string getTag() const
        { 
            return getComponent<TagComponent>(); 
        }

        operator bool() const { return m_handle != entt::null; }
        operator entt::entity() const { return m_handle; }
        operator std::uint32_t() const { return static_cast<uint32_t>(m_handle); }

        bool operator==(const Entity other) const { return m_handle == other.m_handle; }
        bool operator!=(const Entity other) const { return m_handle != other.m_handle; }
        bool operator<(const Entity other) const { return m_handle < other.m_handle; }
        bool operator>(const Entity other) const { return m_handle > other.m_handle; }
        bool operator<=(const Entity other) const { return m_handle <= other.m_handle; }
        bool operator>=(const Entity other) const { return m_handle >= other.m_handle; }

    private:
        entt::basic_registry<entt::entity>* m_registry;
        entt::entity m_handle;

    };

}

