#include "DragFall.h"
#include <cmath>
#include <numbers>
#include <stdexcept>

namespace chu2
{
DragFall dragFall(bool fromDistance, double input, double mass, double gravity,
    double density, double dragCoefficient, double area)
{
    for (double value : {input, mass, gravity, density, dragCoefficient, area})
        if (!std::isfinite(value)) throw std::invalid_argument("Finite inputs required");
    if (input < 0 || mass <= 0 || gravity <= 0 || density < 0 || dragCoefficient < 0 || area < 0)
        throw std::invalid_argument("Invalid drag fall input");
    if (density == 0 || dragCoefficient == 0 || area == 0)
        return {fromDistance ? freeFallFromDistance(input, gravity) : freeFallFromTime(input, gravity), std::nullopt};
    const double q = ((0.5 * density) * dragCoefficient) * (area / mass);
    const double terminal = std::sqrt(gravity / q);
    if (!std::isfinite(q) || q <= 0 || !std::isfinite(terminal) || terminal <= 0)
        throw std::overflow_error("Drag parameters outside representable range");
    const double tau = terminal / gravity;
    double time{}, speed{}, distance{};
    if (fromDistance)
    {
        const double z = q * input;
        const double ratio = std::sqrt(-std::expm1(-2 * z));
        time = tau * (z + std::log1p(ratio));
        speed = terminal * ratio;
        distance = input;
    }
    else
    {
        const double x = input / tau;
        double logCosh;
        if (x < 0.001)
        {
            const double s = std::sinh(x / 2);
            logCosh = std::log1p(2 * s * s);
        }
        else logCosh = x + std::log1p(std::exp(-2 * x)) - std::numbers::ln2;
        time = input;
        speed = terminal * std::tanh(x);
        distance = logCosh / q;
    }
    for (double value : {tau, time, speed, distance})
        if (!std::isfinite(value) || value < 0) throw std::overflow_error("Drag result outside representable range");
    if (tau == 0 || (input > 0 && (speed == 0 || time == 0 || distance == 0)))
        throw std::overflow_error("Drag result underflow");
    return {{gravity, time, speed, distance}, terminal};
}
}
