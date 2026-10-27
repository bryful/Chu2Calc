#include "Motion.h"
#include <cmath>
#include <stdexcept>

namespace chu2
{
namespace
{
Motion checked(Motion result)
{
    if (!std::isfinite(result.acceleration) || !std::isfinite(result.time)
        || !std::isfinite(result.speed) || !std::isfinite(result.distance))
    {
        throw std::overflow_error("Motion overflow");
    }
    return result;
}
}
Motion fromAccelerationTime(double acceleration, double time)
{
    if (!std::isfinite(acceleration) || !std::isfinite(time) || acceleration < 0 || time < 0)
    {
        throw std::invalid_argument("Acceleration and time must be nonnegative");
    }
    const double speed = acceleration * time;
    return checked({acceleration, time, speed, (speed * 0.5) * time});
}
Motion fromSpeedDistance(double speed, double distance)
{
    if (!std::isfinite(speed) || !std::isfinite(distance) || speed <= 0 || distance <= 0)
    {
        throw std::invalid_argument("Speed and distance must be positive");
    }
    const double time = 2 * (distance / speed);
    if (time == 0) throw std::overflow_error("Motion underflow");
    return checked({speed / time, time, speed, distance});
}
Motion freeFallFromTime(double time, double gravity)
{
    if (!std::isfinite(gravity) || gravity <= 0)
        throw std::invalid_argument("Positive gravity required");
    return fromAccelerationTime(gravity, time);
}
Motion freeFallFromDistance(double distance, double gravity)
{
    if (!std::isfinite(distance) || !std::isfinite(gravity) || distance < 0 || gravity <= 0)
        throw std::invalid_argument("Nonnegative distance and positive gravity required");
    if (distance == 0) return {gravity, 0, 0, 0};
    const double speed = std::sqrt(gravity) * std::sqrt(distance) * std::sqrt(2.0);
    const double time = speed / gravity;
    if (speed == 0 || time == 0) throw std::overflow_error("Free fall underflow");
    return checked({gravity, time, speed, distance});
}
}
