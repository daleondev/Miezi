export module mz.graphics.scene.component_utils;

import std;

import mz.graphics.scene.components;
import mz.math.geometry;

namespace mz {

    //------------------------------------------------------
    //                    Transform
    //------------------------------------------------------

    export void setTransform(TransformComponent& component, const Mat4& transform)
    {
        component.scale.x = transform.xAxis().length();
        component.scale.y = transform.yAxis().length();
        component.scale.z = transform.zAxis().length();

        component.rotation = transform.rescaled().toEulerXYZ();

        component.translation = transform.translation();
    }

    export Mat4 getTransform(const TransformComponent& component)
    {
        return component;
    }

    void setTransformChangedCallback(TransformComponent& component, const std::function<void(const Mat4&)>& callback) 
    { 
        component.detectChangeData.changedCallback = callback; 
    }

    void detectTransformChange(TransformComponent& component) 
    { 
        auto& data = component.detectChangeData;
        data.translationListener = component.translation;
        data.rotationListener = component.rotation;
        data.scaleListener = component.scale;
        if (data.changedCallback && (data.translationListener().edge() || data.rotationListener().edge() || data.scaleListener().edge())) 
            (*data.changedCallback)(getTransform(component));
    }

}