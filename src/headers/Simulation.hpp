#pragma once

#include <array>
#include <memory>
#include <vector>

namespace RadarSim {

struct EntitySnapshot {
    unsigned int viewId;
    std::array<float, 3> position;
    std::array<float, 3> orientationEulerDeg;
};

class Simulation {
public:
    Simulation();
    ~Simulation();
    void Tick(double deltaTimeSeconds);
    const std::vector<EntitySnapshot>& GetSnapshots() const;

private:
    void BuildSnapshots();

    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace RadarSim
