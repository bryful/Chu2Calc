#pragma once
#include "Motion.h"
#include <optional>

namespace chu2
{
struct DragFall
{
    Motion motion;
    std::optional<double> terminalSpeed;
};
// 二乗抵抗F=ρCdAv²/2、初速0。SI単位。
DragFall dragFall(bool fromDistance, double input, double mass, double gravity,
    double density, double dragCoefficient, double area);
}
