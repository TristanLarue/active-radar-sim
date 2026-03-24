#pragma once

#include <cstdint>

namespace Entities {

using EntityId = std::uint64_t;
using ViewId = std::uint32_t;

enum class EntityKind {
    Aircraft,
    Missile,
    Bomb,
    Unknown
};

class Entity {
public:
    explicit Entity(EntityId id, ViewId viewId, EntityKind kind);
    virtual ~Entity() = default;

    EntityId GetId() const;
    ViewId GetViewId() const;
    EntityKind GetKind() const;

    // Decision-making step (guidance, target selection, mode switches, etc.).
    virtual void Think(double deltaTimeSeconds) = 0;

    // Physics step (state integration, forces/moments application, etc.).
    virtual void Tick(double deltaTimeSeconds) = 0;

private:
    EntityId id_;
    ViewId viewId_;
    EntityKind kind_;
};

class Aircraft : public Entity {
public:
    explicit Aircraft(EntityId id, ViewId viewId);
    ~Aircraft() override = default;

    // Required force models for all aircraft-like entities.
    virtual double CalculateGravityForce(double mass, double gravityAcceleration) const = 0;
    virtual double CalculateLiftForce(
        double airDensity,
        double velocity,
        double wingArea,
        double liftCoefficient
    ) const = 0;
    virtual double CalculateDragForce(
        double airDensity,
        double velocity,
        double referenceArea,
        double dragCoefficient
    ) const = 0;
};

} // namespace Entities
