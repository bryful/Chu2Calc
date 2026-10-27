#pragma once
#include <span>
#include <string>
#include <string_view>

namespace chu2
{
enum class Category { Speed, Distance, Mass, Acceleration, Time, Energy, Force, Angle, Power, Area };
struct Unit
{
    std::wstring_view name;
    double factor;
    bool usesSound = false;
};
std::span<const Unit> units(Category category);
double convert(double value, const Unit& from, const Unit& to, double soundSpeed);
bool parseNumber(std::wstring_view text, double& result);
std::wstring formatNumber(double value);
}
