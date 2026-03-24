#pragma once

namespace Physics {
double CalculateLiftForce(double airDensity, double velocity, double wingArea, double liftCoefficient);
double CalculateDragForce(double airDensity, double velocity, double referenceArea, double dragCoefficient);
double CalculateGravityForce(double mass, double gravityAcceleration);
double CalculateThrustForce(double massFlowRate, double exhaustVelocity, double exitPressure, double ambientPressure, double nozzleExitArea);
} // namespace Physics
