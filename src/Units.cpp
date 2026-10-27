#include "Units.h"
#include "Energy.h"
#include <cerrno>
#include <cmath>
#include <cwchar>
#include <cwctype>
#include <iomanip>
#include <limits>
#include <locale>
#include <sstream>
#include <stdexcept>

namespace chu2
{
namespace
{
constexpr double LIGHT = 299792458.0;
constexpr Unit SPEED[] = {
    {L"m/s", 1}, {L"km/s", 1000}, {L"m/min", 1.0/60},
    {L"km/min", 1000.0/60}, {L"m/h", 1.0/3600}, {L"km/h", 1.0/3.6},
    {L"万km/h", 10000.0/3.6}, {L"マッハ", 1, true},
    {L"ノット", 1852.0/3600}, {L"光速比 (c)", LIGHT},
    {L"mph", 1609.344/3600}, {L"ft/s", 0.3048}};
constexpr Unit DISTANCE[] = {
    {L"m", 1}, {L"km", 1000}, {L"万km", 1e7}, {L"音秒", 1, true},
    {L"海里", 1852}, {L"feet", 0.3048}, {L"inch", 0.0254},
    {L"yard", 0.9144}, {L"mile", 1609.344}, {L"光秒", LIGHT},
    {L"光分", LIGHT*60}, {L"光時", LIGHT*3600}, {L"光日", LIGHT*86400},
    {L"光年 (365.25日)", LIGHT*31557600}, {L"AU", 149597870700.0},
    {L"パーセク", 149597870700.0*648000/3.14159265358979323846},
    {L"mm", 0.001}, {L"cm", 0.01}};
constexpr Unit MASS[] = {
    {L"g", 0.001}, {L"kg", 1}, {L"t", 1000}, {L"ポンド (lb)", 0.45359237},
    {L"mg", 1e-6}, {L"μg", 1e-9}, {L"オンス (oz)", 0.028349523125}};
constexpr Unit ACCELERATION[] = {
    {L"m/s²", 1}, {L"G (標準重力)", 9.80665}, {L"km/s²", 1000}, {L"km/h/s", 1.0/3.6}};
constexpr Unit TIME[] = {
    {L"秒", 1}, {L"分", 60}, {L"時間", 3600}, {L"日", 86400}};
constexpr Unit ENERGY[] = {
    {L"J（ジュール）", 1}, {L"kJ", 1000}, {L"MJ", 1e6}, {L"GJ", 1e9},
    {L"kcal（熱化学）", 4184}, {L"kWh", 3600000},
    {L"TNT換算 / g", 4184}, {L"TNT換算 / kg", 4184000},
    {L"TNT換算 / t", 4.184e9}, {L"TNT換算 / kt", 4.184e12}, {L"TNT換算 / Mt", 4.184e15},
    {L"Wh", 3600}, {L"MWh", 3.6e9}, {L"ジャイアント馬場キック", BABA_KICK_JOULES}};
constexpr Unit AREA[] = {{L"m²", 1}, {L"km²", 1e6}, {L"ha", 10000}, {L"a（アール）", 100}, {L"cm²", 0.0001}};
constexpr Unit POWER[] = {{L"W", 1}, {L"kW", 1000}, {L"MW", 1e6}, {L"GW", 1e9}, {L"TW", 1e12}, {L"mW", 0.001}};
constexpr Unit FORCE[] = {{L"N", 1}, {L"kN", 1000}, {L"kgf", 9.80665}, {L"tf", 9806.65}};
constexpr Unit ANGLE[] = {{L"度 (°)", 1}, {L"rad", 180 / 3.14159265358979323846}};
}
std::span<const Unit> units(Category category)
{
    switch (category)
    {
    case Category::Speed: return SPEED;
    case Category::Distance: return DISTANCE;
    case Category::Mass: return MASS;
    case Category::Acceleration: return ACCELERATION;
    case Category::Time: return TIME;
    case Category::Energy: return ENERGY;
    case Category::Force: return FORCE;
    case Category::Angle: return ANGLE;
    case Category::Power: return POWER;
    case Category::Area: return AREA;
    }
    return {};
}
double convert(double value, const Unit& from, const Unit& to, double soundSpeed)
{
    if (!std::isfinite(value) || !std::isfinite(soundSpeed) || soundSpeed <= 0)
    {
        throw std::invalid_argument("Invalid conversion input");
    }
    const double fromFactor = from.factor * (from.usesSound ? soundSpeed : 1);
    const double toFactor = to.factor * (to.usesSound ? soundSpeed : 1);
    const double result = value * (fromFactor / toFactor);
    if (!std::isfinite(result))
    {
        throw std::overflow_error("Conversion overflow");
    }
    return result;
}
bool parseNumber(std::wstring_view text, double& result)
{
    const std::wstring input(text);
    wchar_t* end = nullptr;
    errno = 0;
    result = std::wcstod(input.c_str(), &end);
    if (end == input.c_str() || errno == ERANGE || !std::isfinite(result)) return false;
    while (*end && std::iswspace(*end)) ++end;
    return *end == 0;
}
std::wstring formatNumber(double value)
{
    std::wostringstream stream;
    stream.imbue(std::locale::classic());
    stream << std::setprecision(12) << value;
    std::wstring text = stream.str();
    const auto exponentAt = text.find_first_of(L"eE");
    if (exponentAt == std::wstring::npos) return text == L"-0" ? L"0" : text;

    // 有効数字を保ちつつ指数を展開し、文章へそのまま貼れる表記にする。
    const int exponent = std::stoi(text.substr(exponentAt + 1));
    std::wstring digits = text.substr(0, exponentAt);
    const bool negative = digits.front() == L'-';
    if (negative) digits.erase(0, 1);
    const auto dot = digits.find(L'.');
    const int point = (dot == std::wstring::npos ? static_cast<int>(digits.size()) : static_cast<int>(dot)) + exponent;
    if (dot != std::wstring::npos) digits.erase(dot, 1);
    if (point <= 0) digits = L"0." + std::wstring(static_cast<size_t>(-point), L'0') + digits;
    else if (point >= static_cast<int>(digits.size())) digits.append(static_cast<size_t>(point) - digits.size(), L'0');
    else digits.insert(static_cast<size_t>(point), 1, L'.');
    return negative ? L"-" + digits : digits;
}
}
