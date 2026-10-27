#pragma once

namespace chu2
{
inline constexpr double EARTH_MU = 3.986004418e14;
struct Orbit
{
    double radius;
    double altitude;
    double speed;
    double period;
};
struct Escape
{
    double speed;
    double circularSpeed;
    double gravity;
};
Escape escapeFromAltitude(double bodyRadius, double altitude, double mu);
// 長さm、速度m/s、重力パラメータm³/s²。球対称天体の円軌道。
Orbit orbitFromAltitude(double bodyRadius, double altitude, double mu);
Orbit orbitFromSpeed(double bodyRadius, double speed, double mu);
}
