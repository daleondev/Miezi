module;
#include "mz/core/core.h"

// #include <entt/entt.hpp>
export module mz.graphics.scene.entity;

import std;

import mz.core.logging;
export import mz.util;

namespace mz {

    // class Entity
    // {
    // public:
    //     Entity() = delete;
    //     Entity(entt::entity handle, entt::registry* registry) : m_handle{handle}, m_registry{ registry }  {}
    //     Entity(const Entity&) = default;
    //     ~Entity() = default;

    //     template<typename T, typename... Args>
    //     T& addComponent(Args&&... args)
    //     {
    //         MZ_ASSERT(!hasComponent<T>(), "Entity already has component!");
    //         return m_registry->emplace<T>(m_handle, std::forward<Args>(args)...);
    //     }

    //     template<typename T, typename... Args>
    //     T& addOrReplaceComponent(Args&&... args)
    //     {
    //         return m_registry->emplace_or_replace<T>(m_handle, std::forward<Args>(args)...);
    //     }

    //     template<typename T, typename... Args>
    //     T& addComponentIfNotExists(Args&&... args)
    //     {
    //         return hasComponent<T>() ? getComponent<T>() : m_registry->emplace_or_replace<T>(m_handle, std::forward<Args>(args)...);
    //     }

    //     template<typename T>
    //     T& getComponent() const
    //     {
    //         MZ_ASSERT(hasComponent<T>(), "Entity does not have component!");
    //         return m_registry->get<T>(m_handle);
    //     }

    //     template<typename T>
    //     inline bool hasComponent() const { return m_registry->all_of<T>(m_handle); }

    //     template<typename T>
    //     void RemoveComponent()
    //     {
    //         MZ_ASSERT(hasComponent<T>(), "Entity does not have component!");
    //         m_registry->remove<T>(m_handle);
    //     }

    //     UUID getId() const;
    //     std::string getTag() const;

    //     operator bool() const { return m_handle != entt::null; }
    //     operator entt::entity() const { return m_handle; }
    //     operator std::uint32_t() const { return static_cast<uint32_t>(m_handle); }

    //     bool operator==(const Entity other) const { return m_handle == other.m_handle; }
    //     bool operator!=(const Entity other) const { return m_handle != other.m_handle; }
    //     bool operator<(const Entity other) const { return m_handle < other.m_handle; }
    //     bool operator>(const Entity other) const { return m_handle > other.m_handle; }
    //     bool operator<=(const Entity other) const { return m_handle <= other.m_handle; }
    //     bool operator>=(const Entity other) const { return m_handle >= other.m_handle; }

    // private:
    //     entt::entity m_handle;
    //     entt::registry* m_registry;

    // };

    // export const Entity EntityNull{static_cast<entt::entity>(entt::null), nullptr};

}