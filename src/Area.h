#pragma once
#include "Units.h"

namespace chu2
{
double circleArea(double radius);
double rectangleArea(double width, double height);
std::span<const Unit> areaReferences();
}
