module;
#include "mz/core/core.h"
export module mz.graphics.plotting.plot;

import std;

import mz.core.logging;
import mz.core.types;

import mz.graphics.scene;
import mz.graphics.scene.components;
import mz.graphics.scene.component_utils;

import mz.math.geometry;

import mz.util;
import mz.util.thread;
import mz.util.time;

namespace mz {

    export enum class MarkerStyle
    {
        None,
        Dot,
        Triangle,
        Rect,
        Circle,
        Plus,
        X,
        Star
    };

    export enum class LineStyle
    {
        None,
        Solid,
        Dashed,
        Dotted,
    }; 

    export struct PlotData
    {
        Vector<Vec3> positions;

        float markerSize;
        float lineWidth;
        Vec4 color;

        Vec4 fillColor = Vec4(0.0f);

        MarkerStyle markerStyle = MarkerStyle::Dot;
        LineStyle lineStyle = LineStyle::Solid;
    };

    export class Plot
    {
    public:
        ~Plot()
        {
            clearSceneData();
        }

        PlotData getData() const 
        { 
            std::lock_guard<std::mutex> lock(m_mutex); 
            return m_data; 
        }
        void setData(const PlotData& data) 
        { 
            std::lock_guard<std::mutex> lock(m_mutex); 
            m_data = data; 
            updateSceneData();
        }

    private:
    public:
        Plot(Scene* scene, const PlotData& data)
            : m_id{uuid()}, m_scene{ scene }, m_data{ data }, m_mutex{}
        {
            updateSceneData();
            MZ_INFO("Created Plot");
        }

        void setScene(Scene* scene) { m_scene = scene; }

        struct PlotIdComponent
        {
            UUID id;
            PlotIdComponent(const UUID id) : id{ id } {}
            operator UUID() { return id; }
        };

        void clearSceneData()
        {
            if (!m_scene)
                return;

            // todo: check performance
            if constexpr (false) {
                for (auto [entity, component] : m_scene->getEntitiesWithComponent<PlotIdComponent>()) {
                    if (component.id == m_id)
                        m_scene->destroyEntity(entity);
                }
            }  
            else {
                for (const auto& entity : m_markerEntities) {
                    m_scene->destroyEntity(entity.id_0);
                    m_scene->destroyEntity(entity.id_1);
                    m_scene->destroyEntity(entity.id_2);
                    m_scene->destroyEntity(entity.id_3);
                }
                for (auto id : m_lineEntities)
                    m_scene->destroyEntity(id);
            }

            m_markerEntities.clear();
            m_lineEntities.clear();
        }

        void updateSceneData()
        {
            clearSceneData();

            m_markerEntities = DynamicArray<MarkerEntityDefinition>(m_data.markerStyle == MarkerStyle::None ? 0 : m_data.positions.size());
            m_lineEntities = DynamicArray<UUID>(m_data.lineStyle == LineStyle::None ? 0 : m_data.positions.size()-1);

            for (std::size_t i = 0; i < m_data.positions.size(); ++i) {

                if (!m_markerEntities.empty()) {
                    auto entity = m_scene->createEntity(std::format("marker{}_{}_0", m_id, i));
                    entity.addComponent<PlotIdComponent>(m_id);
                    m_markerEntities[i].id_0 = entity.getId();

                    switch (m_data.markerStyle)
                    {
                        case MarkerStyle::Dot:
                        {
                            entity.addComponent<PointRendererComponent>(m_data.fillColor, m_data.markerSize);
                            break;
                        }
                        case MarkerStyle::Rect:
                        {
                            entity.addComponent<RectRendererComponent>(m_data.fillColor, m_data.lineWidth, m_data.color);
                            break;
                        }
                        case MarkerStyle::Circle:
                        {
                            entity.addComponent<CircleRendererComponent>(m_data.fillColor, m_data.lineWidth, m_data.color);
                            break;
                        }
                        case MarkerStyle::Plus: 
                        case MarkerStyle::X:
                        {
                            entity.addComponent<LineRendererComponent>(m_data.color, m_data.lineWidth);

                            auto entity2 = m_scene->createEntity(std::format("marker{}_{}_1", m_id, i));
                            entity2.addComponent<PlotIdComponent>(m_id);
                            entity2.addComponent<LineRendererComponent>(m_data.color, m_data.lineWidth);
                            m_markerEntities[i].id_1 = entity2.getId();
                            break;
                        }
                        case MarkerStyle::Star:
                        {
                            entity.addComponent<LineRendererComponent>(m_data.color, m_data.lineWidth);

                            auto entity2 = m_scene->createEntity(std::format("marker{}_{}_1", m_id, i));
                            entity2.addComponent<PlotIdComponent>(m_id);
                            entity2.addComponent<LineRendererComponent>(m_data.color, m_data.lineWidth);
                            m_markerEntities[i].id_1 = entity2.getId();

                            auto entity3 = m_scene->createEntity(std::format("marker{}_{}_2", m_id, i));
                            entity3.addComponent<PlotIdComponent>(m_id);
                            entity3.addComponent<LineRendererComponent>(m_data.color, m_data.lineWidth);
                            m_markerEntities[i].id_2 = entity3.getId();

                            auto entity4 = m_scene->createEntity(std::format("marker{}_{}_3", m_id, i));
                            entity4.addComponent<PlotIdComponent>(m_id);
                            entity4.addComponent<LineRendererComponent>(m_data.color, m_data.lineWidth);
                            m_markerEntities[i].id_3 = entity4.getId();
                            break;
                        }
                        default:
                            MZ_ASSERT(false, "Invalid Markerstyle");
                    }
                }

                if (i < 1) continue;

                if (!m_lineEntities.empty()) {
                    auto entity = m_scene->createEntity(std::format("line{}_{}", m_id, i-1));
                    entity.addComponent<PlotIdComponent>(m_id);
                    m_lineEntities[i-1] = entity.getId();

                    switch (m_data.lineStyle)
                    {
                        case LineStyle::Solid:
                        {
                            entity.addComponent<LineRendererComponent>(m_data.color, m_data.lineWidth);
                            break;        
                        }
                        default:
                            MZ_ASSERT(false, "Invalid Linestyle");
                    }
                }
            }

            update(false);
        }

        void update(const bool sync = true)
        {
            std::unique_lock<std::mutex> lock(m_mutex, std::defer_lock);
            if (sync) lock.lock();

            auto axesSize = m_scene->getSize();
            float aspect = axesSize.x / axesSize.y;
            float pixelSize = m_data.markerSize*MARKER_PIXEL_SIZE;

            auto t = Mat4(1.0f).scaled({pixelSize/axesSize.x, aspect*(pixelSize/axesSize.x), 1.0f});
            for (std::size_t i = 0; i < m_data.positions.size(); ++i) {
                t.translation() = m_data.positions[i].asVec4(1.0f);

                if (!m_markerEntities.empty()) {
                    switch (m_data.markerStyle)
                    {
                        case MarkerStyle::Dot:
                        case MarkerStyle::Rect:
                        case MarkerStyle::Circle:
                        {
                            auto& transform = m_scene->getEntity(m_markerEntities[i].id_0).getComponent<TransformComponent>();
                            setTransform(transform, t);

                            break;  
                        }
                        case MarkerStyle::Plus:
                        case MarkerStyle::X:
                        {
                            const glm::vec3 a = {t.translation() - 0.5f*t.xAxis()};
                            const glm::vec3 b = {t.translation() + 0.5f*t.xAxis()};
                            const glm::vec3 c = {t.translation() - 0.5f*t.yAxis()};
                            const glm::vec3 d = {t.translation() + 0.5f*t.yAxis()};

                            auto& transform = m_scene->getEntity(m_markerEntities[i].id_0).getComponent<TransformComponent>();
                            setTransform(transform, calcLineTransform(a, b));

                            auto& transform2 = m_scene->getEntity(m_markerEntities[i].id_1).getComponent<TransformComponent>();
                            setTransform(transform2, calcLineTransform(c, d));

                            break;
                        }
                        case MarkerStyle::Star:
                        {
                            const glm::vec3 a = {t.translation() - 0.5f*t.xAxis()};
                            const glm::vec3 b = {t.translation() + 0.5f*t.xAxis()};
                            const glm::vec3 c = {t.translation() - 0.5f*t.yAxis()};
                            const glm::vec3 d = {t.translation() + 0.5f*t.yAxis()};
                            const glm::vec3 e = {t.translation() - 0.3535f*t.xAxis() + 0.3535f*t.yAxis()};
                            const glm::vec3 f = {t.translation() + 0.3535f*t.xAxis() - 0.3535f*t.yAxis()};
                            const glm::vec3 g = {t.translation() - 0.3535f*t.xAxis() - 0.3535f*t.yAxis()};
                            const glm::vec3 h = {t.translation() + 0.3535f*t.xAxis() + 0.3535f*t.yAxis()};

                            auto& transform = m_scene->getEntity(m_markerEntities[i].id_0).getComponent<TransformComponent>();
                            setTransform(transform, calcLineTransform(a, b));

                            auto& transform2 = m_scene->getEntity(m_markerEntities[i].id_1).getComponent<TransformComponent>();
                            setTransform(transform2, calcLineTransform(c, d));

                            auto& transform3 = m_scene->getEntity(m_markerEntities[i].id_2).getComponent<TransformComponent>();
                            setTransform(transform3, calcLineTransform(e, f));

                            auto& transform4 = m_scene->getEntity(m_markerEntities[i].id_3).getComponent<TransformComponent>();
                            setTransform(transform4, calcLineTransform(g, h));

                            break;
                        }               
                        default:
                            MZ_ASSERT(false, "Invalid Linestyle");
                    }
                }

                if (i < 1) continue;

                if (!m_lineEntities.empty()) {
                    switch (m_data.lineStyle)
                    {
                        case LineStyle::Solid:
                        {
                            auto& transform = m_scene->getEntity(m_lineEntities[i-1]).getComponent<TransformComponent>();
                            setTransform(transform, calcLineTransform(m_data.positions[i-1], m_data.positions[i]));
                            break; 
                        }
                        default:
                            MZ_ASSERT(false, "Invalid Linestyle");
                    }
                }
            }
        }

        UUID m_id;

        Scene* m_scene;
        PlotData m_data;

        struct MarkerEntityDefinition
        {
            UUID id_0;
            UUID id_1;
            UUID id_2;
            UUID id_3;
            MarkerEntityDefinition() { id_0 = id_1 = id_2 = id_3 = UUID_NULL; }
        };
        DynamicArray<MarkerEntityDefinition> m_markerEntities;
        DynamicArray<UUID> m_lineEntities;
        
        mutable std::mutex m_mutex;

        inline static constexpr float MARKER_PIXEL_SIZE = 8.4;

        friend class Axes;
        friend class Figure;
        friend class PlottingEngine;
    };
}