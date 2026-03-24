#include "headers/Simulation.hpp"

#include "headers/Entities.hpp"
#include "headers/Physics.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace RadarSim {
namespace {

class StaticAircraft final : public Entities::Aircraft {
public:
    StaticAircraft(
        Entities::EntityId id,
        Entities::ViewId viewId,
        std::array<float, 3> position,
        std::array<float, 3> orientationEulerDeg
    )
        : Aircraft(id, viewId), position_(position), orientationEulerDeg_(orientationEulerDeg) {}

    void Think(double) override {}
    void Tick(double) override {}

    double CalculateGravityForce(double mass, double gravityAcceleration) const override {
        return Physics::CalculateGravityForce(mass, gravityAcceleration);
    }

    double CalculateLiftForce(double airDensity, double velocity, double wingArea, double liftCoefficient) const override {
        return Physics::CalculateLiftForce(airDensity, velocity, wingArea, liftCoefficient);
    }

    double CalculateDragForce(double airDensity, double velocity, double referenceArea, double dragCoefficient) const override {
        return Physics::CalculateDragForce(airDensity, velocity, referenceArea, dragCoefficient);
    }

    const std::array<float, 3>& GetPosition() const {
        return position_;
    }

    const std::array<float, 3>& GetOrientationEulerDeg() const {
        return orientationEulerDeg_;
    }

private:
    std::array<float, 3> position_;
    std::array<float, 3> orientationEulerDeg_;
};

} // namespace

struct Simulation::Impl {
    std::vector<std::unique_ptr<StaticAircraft>> entities;
    std::vector<EntitySnapshot> snapshots;
};

Simulation::Simulation() : impl_(std::make_unique<Impl>()) {
    impl_->entities.emplace_back(std::make_unique<StaticAircraft>(
        1u,
        1001u,
        std::array<float, 3>{-4.0f, 0.0f, -14.0f},
        std::array<float, 3>{0.0f, 10.0f, 0.0f}
    ));
    impl_->entities.emplace_back(std::make_unique<StaticAircraft>(
        2u,
        1002u,
        std::array<float, 3>{0.0f, 1.0f, -10.0f},
        std::array<float, 3>{0.0f, -20.0f, 0.0f}
    ));
    impl_->entities.emplace_back(std::make_unique<StaticAircraft>(
        3u,
        1003u,
        std::array<float, 3>{3.5f, -1.0f, -12.0f},
        std::array<float, 3>{5.0f, 35.0f, 0.0f}
    ));

    BuildSnapshots();
}

Simulation::~Simulation() = default;

void Simulation::Tick(double deltaTimeSeconds) {
    for(const std::unique_ptr<StaticAircraft>& entity : impl_->entities) {
        entity->Think(deltaTimeSeconds);
        entity->Tick(deltaTimeSeconds);
    }

    BuildSnapshots();
}

const std::vector<EntitySnapshot>& Simulation::GetSnapshots() const {
    return impl_->snapshots;
}

void Simulation::BuildSnapshots() {
    impl_->snapshots.clear();
    impl_->snapshots.reserve(impl_->entities.size());

    for(const std::unique_ptr<StaticAircraft>& entity : impl_->entities) {
        impl_->snapshots.push_back(EntitySnapshot{
            static_cast<unsigned int>(entity->GetViewId()),
            entity->GetPosition(),
            entity->GetOrientationEulerDeg()
        });
    }
}

} // namespace RadarSim
