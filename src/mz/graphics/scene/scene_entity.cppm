module;
#include "Entity.h"
export module mz.graphics.scene.entity;

namespace mz {

    export using mz::Entity;
    export const mz::Entity EntityNull{entt::null, nullptr};

}

namespace entt {

    export using entt::registry;

}