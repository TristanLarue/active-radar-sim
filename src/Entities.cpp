#include "headers/Entities.hpp"

namespace Entities {

Entity::Entity(EntityId id, ViewId viewId, EntityKind kind) : id_(id), viewId_(viewId), kind_(kind) {}

EntityId Entity::GetId() const {
    return id_;
}

ViewId Entity::GetViewId() const {
    return viewId_;
}

EntityKind Entity::GetKind() const {
    return kind_;
}

Aircraft::Aircraft(EntityId id, ViewId viewId) : Entity(id, viewId, EntityKind::Aircraft) {}

} // namespace Entities
