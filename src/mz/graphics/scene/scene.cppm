export module mz.graphics.scene;

import std;

import mz.graphics.renderer;
import mz.graphics.scene.entity;
import mz.graphics.scene.components;

import mz.util;

namespace mz {

    template<typename... Components>
    void setComponentsConstructCallback(entt::registry& registry);

    template<typename... Components>
	void setComponentsConstructCallback(ComponentGroup<Components...>, entt::registry& registry)
	{
		setComponentsConstructCallback<Components...>(registry);
	}

    export class Scene
    {
    public:
        using SceneEntityIterator = std::map<UUID, Entity>::iterator;

        static void onRendererComponentConstruct(Entity entity, auto component)
        {
            entity.addComponentIfNotExists<TransformComponent>();
        }

        Scene(const std::shared_ptr<IRenderer>& renderer)
            : m_renderer{ renderer }
        {
            setComponentsConstructCallback(RendererComponents{}, m_registry);

            // m_registry.on_construct<LineRendererComponent>().connect<[](entt::registry& registry, entt::entity handle) -> void {
            //     Entity entity(&registry, handle);

            // }>();
        }
        ~Scene() = default;

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

    private:
        entt::registry m_registry;
        std::map<UUID, Entity> m_entities;

        std::shared_ptr<IRenderer> m_renderer;
    };

    template<typename... Components>
    void setComponentsConstructCallback(entt::registry& registry) 
    {
        (..., registry.on_construct<Components>().template connect<[&](entt::registry& registry, entt::entity handle) { 
            Scene::onRendererComponentConstruct(Entity(&registry, handle), registry.get<Components>(handle));
        }>());
    }

}