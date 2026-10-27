#pragma once

namespace chu2
{
struct Impact
{
    double energy, force, deceleration, time;
};
struct Rotation
{
    double acceleration, speed, rpm, period;
};
struct Jump
{
    double speed, acceleration, supportAcceleration, pushTime, peakTime, flightTime;
};
struct Projectile
{
    double height, range, flightTime, peakTime, horizontalSpeed, verticalSpeed;
};
Impact impactFromDistance(double massKg, double speed, double distance);
Rotation rotationFromPeriod(double radius, double period);
Rotation rotationFromAcceleration(double radius, double acceleration);
Jump jumpFromHeight(double height, double pushDistance, double gravity);
Projectile projectile(double speed, double angleDegrees, double gravity);
}
