#include "Mechanics.h"
#include "Energy.h"
#include <cmath>
#include <initializer_list>
#include <numbers>
#include <stdexcept>

namespace chu2
{
namespace
{
void checkFinite(std::initializer_list<double> values)
{
    for (double value : values)
        if (!std::isfinite(value)) throw std::overflow_error("Mechanics outside representable range");
}
}
Impact impactFromDistance(double massKg, double speed, double distance)
{
    checkFinite({massKg, speed, distance});
    if (massKg <= 0 || speed < 0 || distance <= 0)
        throw std::invalid_argument("Positive mass and distance, nonnegative speed required");
    const double energy = kineticEnergy(massKg, speed);
    const double force = energy / distance;
    const double acceleration = force / massKg;
    const double time = speed == 0 ? 0 : 2 * (distance / speed);
    checkFinite({energy, force, acceleration, time});
    if (speed > 0 && (force == 0 || acceleration == 0 || time == 0))
        throw std::overflow_error("Impact underflow");
    return {energy, force, acceleration, time};
}
Rotation rotationFromPeriod(double radius, double period)
{
    checkFinite({radius, period});
    if (radius <= 0 || period <= 0)
        throw std::invalid_argument("Positive radius and period required");
    const double omega = (2 * std::numbers::pi) / period;
    const double speed = radius * omega;
    const double acceleration = speed * omega;
    const double rpm = 60 / period;
    checkFinite({acceleration, speed, rpm});
    if (acceleration == 0 || speed == 0 || rpm == 0) throw std::overflow_error("Rotation underflow");
    return {acceleration, speed, rpm, period};
}
Rotation rotationFromAcceleration(double radius, double acceleration)
{
    checkFinite({radius, acceleration});
    if (radius <= 0 || acceleration <= 0)
        throw std::invalid_argument("Positive radius and acceleration required");
    const double speed = std::sqrt(radius) * std::sqrt(acceleration);
    const double period = (std::sqrt(radius) / std::sqrt(acceleration)) * (2 * std::numbers::pi);
    const double rpm = 60 / period;
    checkFinite({speed, period, rpm});
    if (speed <= 0 || period <= 0 || rpm <= 0) throw std::overflow_error("Rotation underflow");
    return {acceleration, speed, rpm, period};
}
Jump jumpFromHeight(double height, double pushDistance, double gravity)
{
    checkFinite({height, pushDistance, gravity});
    if (height <= 0 || pushDistance <= 0 || gravity <= 0)
        throw std::invalid_argument("Positive height, push distance and gravity required");
    const double speed = std::sqrt(gravity) * std::sqrt(height) * std::sqrt(2.0);
    const double acceleration = gravity * (height / pushDistance);
    const double supportAcceleration = acceleration + gravity;
    const double pushTime = 2 * (pushDistance / speed);
    const double peakTime = speed / gravity;
    const double flightTime = 2 * peakTime;
    checkFinite({speed, acceleration, supportAcceleration, pushTime, peakTime, flightTime});
    if (speed == 0 || acceleration == 0 || pushTime == 0 || peakTime == 0 || flightTime == 0)
        throw std::overflow_error("Jump underflow");
    return {speed, acceleration, supportAcceleration, pushTime, peakTime, flightTime};
}
Projectile projectile(double speed, double angleDegrees, double gravity)
{
    checkFinite({speed, angleDegrees, gravity});
    if (speed < 0 || angleDegrees < 0 || angleDegrees > 90 || gravity <= 0)
        throw std::invalid_argument("Nonnegative speed, angle 0..90, positive gravity required");
    const double angle = angleDegrees * (std::numbers::pi / 180);
    const double vx = angleDegrees == 90 ? 0 : speed * std::cos(angle);
    const double vy = angleDegrees == 0 ? 0 : speed * std::sin(angle);
    const double peakTime = vy / gravity;
    const double flightTime = 2 * peakTime;
    const double height = (vy * 0.5) * peakTime;
    const double range = vx * flightTime;
    checkFinite({height, range, flightTime, peakTime, vx, vy});
    return {height, range, flightTime, peakTime, vx, vy};
}
}
