#pragma once

namespace chu2
{
inline constexpr double BABA_KICK_JOULES = 0.5 * 20 * 15 * 15 * 1.5;
// 質量kg、速さm/sから非相対論の運動エネルギーJを求める。
double kineticEnergy(double massKg, double speed);
double powerFromEnergyTime(double energy, double time);
double timeFromEnergyPower(double energy, double power);
double energyFromPowerTime(double power, double time);
}
