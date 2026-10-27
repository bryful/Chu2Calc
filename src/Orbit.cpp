#include "Orbit.h"
#include <cmath>
#include <numbers>
#include <limits>
#include <stdexcept>

namespace chu2
{
namespace
{
void validateBody(double bodyRadius, double mu)
{
    if (!std::isfinite(bodyRadius) || !std::isfinite(mu) || bodyRadius <= 0 || mu <= 0)
        throw std::invalid_argument("Body radius and mu must be positive");
}
Orbit complete(double bodyRadius, double radius, double speed)
{
    const double altitude = radius - bodyRadius;
    const double period = (radius / speed) * (2 * std::numbers::pi);
    if (!std::isfinite(radius) || !std::isfinite(speed) || !std::isfinite(period)
        || !std::isfinite(altitude) || radius <= 0 || speed <= 0 || period <= 0)
        throw std::overflow_error("Orbit outside representable range");
    if (altitude < 0) throw std::domain_error("Orbit lies inside body");
    return {radius, altitude, speed, period};
}
}
Orbit orbitFromAltitude(double bodyRadius, double altitude, double mu)
{
    validateBody(bodyRadius, mu);
    if (!std::isfinite(altitude) || altitude < 0)
        throw std::invalid_argument("Altitude must be nonnegative");
    const double radius = bodyRadius + altitude;
    auto result = complete(bodyRadius, radius, std::sqrt(mu) / std::sqrt(radius));
    result.altitude = altitude;
    return result;
}
Orbit orbitFromSpeed(double bodyRadius, double speed, double mu)
{
    validateBody(bodyRadius, mu);
    if (!std::isfinite(speed) || speed <= 0)
        throw std::invalid_argument("Speed must be positive");
    double radius = (mu / speed) / speed;
    if (radius < bodyRadius && bodyRadius - radius <= bodyRadius * (8 * std::numeric_limits<double>::epsilon()))
        radius = bodyRadius;
    return complete(bodyRadius, radius, speed);
}
Escape escapeFromAltitude(double bodyRadius, double altitude, double mu)
{
    validateBody(bodyRadius, mu);
    if (!std::isfinite(altitude) || altitude < 0)
        throw std::invalid_argument("Nonnegative altitude required");
    const double radius = bodyRadius + altitude;
    const double circular = std::sqrt(mu) / std::sqrt(radius);
    const double speed = std::sqrt(2.0) * circular;
    const double gravity = (mu / radius) / radius;
    if (!std::isfinite(radius) || !std::isfinite(speed) || !std::isfinite(gravity)
        || speed <= 0 || circular <= 0 || gravity <= 0)
        throw std::overflow_error("Escape result outside representable range");
    return {speed, circular, gravity};
}
}
