#include "Area.h"
#include <cmath>
#include <numbers>
#include <stdexcept>

namespace chu2
{
namespace
{
constexpr Unit REFERENCES[] = {
    {L"東京ドーム / 個分", 46755},
    {L"畳・関東（江戸間）/ 畳分", 1.76 * 0.88},
    {L"畳・関西（京間）/ 畳分", 1.91 * 0.955}
};
void validate(double value)
{
    if (!std::isfinite(value) || value < 0)
        throw std::invalid_argument("Nonnegative finite size required");
}
double checked(double result, bool positiveExpected)
{
    if (!std::isfinite(result) || (positiveExpected && result == 0))
        throw std::overflow_error("Area outside representable range");
    return result;
}
}
double circleArea(double radius)
{
    validate(radius);
    return checked((radius * radius) * std::numbers::pi, radius > 0);
}
double rectangleArea(double width, double height)
{
    validate(width);
    validate(height);
    return checked(width * height, width > 0 && height > 0);
}
std::span<const Unit> areaReferences()
{
    return REFERENCES;
}
}
