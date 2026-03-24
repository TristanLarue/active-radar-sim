#include "headers/Physics.hpp"

#include <cmath>

namespace Physics {

double CalculateLiftForce(double airDensity, double velocity, double wingArea, double liftCoefficient) {
    return 0.5 * airDensity * std::pow(velocity, 2.0) * wingArea * liftCoefficient;
}

double CalculateDragForce(double airDensity, double velocity, double referenceArea, double dragCoefficient) {
    return 0.5 * airDensity * std::pow(velocity, 2.0) * referenceArea * dragCoefficient;
}

double CalculateGravityForce(double mass, double gravityAcceleration) {
    return mass * gravityAcceleration;
}

double CalculateThrustForce(
    double massFlowRate,
    double exhaustVelocity,
    double exitPressure,
    double ambientPressure,
    double nozzleExitArea
) {
    return (massFlowRate * exhaustVelocity) + ((exitPressure - ambientPressure) * nozzleExitArea);
}

} // namespace Physics
