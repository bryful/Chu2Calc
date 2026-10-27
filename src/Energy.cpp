#include "Energy.h"
#include <cmath>
#include <stdexcept>

namespace chu2
{
namespace
{
void validate(double first, double second, bool secondPositive)
{
    if (!std::isfinite(first) || !std::isfinite(second) || first < 0
        || (secondPositive ? second <= 0 : second < 0))
        throw std::invalid_argument("Invalid energy or power input");
}
double checkedResult(double value, bool positiveExpected)
{
    if (!std::isfinite(value) || (positiveExpected && value == 0))
        throw std::overflow_error("Energy or power outside representable range");
    return value;
}
}
double powerFromEnergyTime(double energy, double time)
{
    validate(energy, time, true);
    return checkedResult(energy / time, energy > 0);
}
double timeFromEnergyPower(double energy, double power)
{
    validate(energy, power, true);
    return checkedResult(energy / power, energy > 0);
}
double energyFromPowerTime(double power, double time)
{
    validate(power, time, false);
    return checkedResult(power * time, power > 0 && time > 0);
}
double kineticEnergy(double massKg, double speed)
{
    if (!std::isfinite(massKg) || !std::isfinite(speed) || massKg < 0 || speed < 0)
    {
        throw std::invalid_argument("Mass and speed must be nonnegative");
    }
    if (massKg == 0 || speed == 0) return 0;
    // 中間の二乗による不要なオーバーフローを避ける。
    int massExponent{}, speedExponent{};
    const double massFraction = std::frexp(massKg, &massExponent);
    const double speedFraction = std::frexp(speed, &speedExponent);
    const double result = std::ldexp(0.5 * massFraction * speedFraction * speedFraction,
        massExponent + 2 * speedExponent);
    if (!std::isfinite(result) || result == 0)
    {
        throw std::overflow_error("Energy outside representable range");
    }
    return result;
}
}
