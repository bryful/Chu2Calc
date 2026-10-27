#pragma once

namespace chu2
{
struct Motion
{
    double acceleration;
    double time;
    double speed;
    double distance;
};
// 初速ゼロ・一定加速度・空気抵抗なし。単位はm、s。
Motion fromAccelerationTime(double acceleration, double time);
Motion fromSpeedDistance(double speed, double distance);
Motion freeFallFromTime(double time, double gravity);
Motion freeFallFromDistance(double distance, double gravity);
}
