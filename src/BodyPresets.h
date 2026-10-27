#pragma once
#include <span>
#include <string_view>

namespace chu2
{
struct BodyPreset
{
    std::wstring_view name;
    double radiusKm;
    double mu;
    std::wstring_view note;
};
std::span<const BodyPreset> bodyPresets();
}
