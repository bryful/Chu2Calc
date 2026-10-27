#include "Units.h"
#include "Motion.h"
#include "Energy.h"
#include "Orbit.h"
#include "BodyPresets.h"
#include "Mechanics.h"
#include "DragFall.h"
#include "Area.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

int main()
{
    int failures = 0;
    auto check = [&](bool condition, const char* name)
    {
        if (!condition) { std::cerr << name << '\n'; ++failures; }
    };
    auto near = [](double a, double b) { return std::abs(a - b) <= std::max(1.0, std::abs(b)) * 1e-12; };
    const auto speed = chu2::units(chu2::Category::Speed);
    const auto distance = chu2::units(chu2::Category::Distance);
    const auto mass = chu2::units(chu2::Category::Mass);
    check(near(chu2::convert(36, speed[5], speed[0], 343), 10), "km/h");
    check(near(chu2::convert(2, speed[7], speed[0], 340), 680), "custom Mach");
    check(near(chu2::convert(1, distance[9], distance[0], 343), 299792458), "light second");
    check(near(chu2::convert(1, mass[3], mass[1], 343), 0.45359237), "pound");
    for (auto category : {chu2::Category::Speed, chu2::Category::Distance, chu2::Category::Mass})
    {
        const auto units = chu2::units(category);
        for (const auto& from : units)
            for (const auto& to : units)
                check(near(chu2::convert(chu2::convert(123.456, from, to, 343), to, from, 343), 123.456), "round trip");
    }
    double value;
    check(chu2::formatNumber(1.0 / 299792458) == L"0.00000000333564095198", "decimal light ratio");
    check(chu2::formatNumber(0.000003) == L"0.000003", "small decimal");
    check(chu2::formatNumber(-1.2e-12) == L"-0.0000000000012", "negative decimal");
    check(chu2::formatNumber(1e20) == L"100000000000000000000", "large decimal");
    check(chu2::formatNumber(-0.0) == L"0", "negative zero");
    check(chu2::parseNumber(L" -1.2e3 ", value) && value == -1200, "scientific input");
    for (const auto* input : {L"", L"abc", L"1abc", L"nan", L"inf", L"1e999", L"1e-999"})
        check(!chu2::parseNumber(input, value), "invalid input");
    try { chu2::convert(1, speed[0], speed[1], 0); check(false, "invalid sound"); }
    catch (const std::invalid_argument&) {}
    try { chu2::convert(1e308, distance[14], distance[0], 343); check(false, "overflow"); }
    catch (const std::overflow_error&) {}
    const auto references = chu2::areaReferences();
    const auto areas = chu2::units(chu2::Category::Area);
    check(near(chu2::convert(46755, areas[0], references[0], 343), 1), "one Tokyo Dome");
    check(near(chu2::convert(1.5488, areas[0], references[1], 343), 1), "one Kanto tatami");
    check(near(chu2::convert(1.82405, areas[0], references[2], 343), 1), "one Kansai tatami");
    check(near(chu2::convert(23377.5, areas[0], references[0], 343), 0.5), "fractional area reference");
    check(near(chu2::circleArea(100), 31415.92653589793), "radius to area");
    check(near(chu2::rectangleArea(100, 50), 5000), "rectangle to area");
    check(chu2::circleArea(0) == 0 && chu2::rectangleArea(0, 10) == 0, "zero area");
    check(near(chu2::convert(1, areas[1], areas[0], 343), 1e6), "square kilometer");
    const auto lengthsForArea = chu2::units(chu2::Category::Distance);
    check(near(chu2::rectangleArea(chu2::convert(1, lengthsForArea[1], lengthsForArea[0], 343), chu2::convert(2, lengthsForArea[1], lengthsForArea[0], 343)), 2e6), "rectangle kilometer sides");
    const auto energyForJoke = chu2::units(chu2::Category::Energy);
    check(chu2::BABA_KICK_JOULES == 3375 && near(chu2::convert(6750, energyForJoke[0], energyForJoke[13], 343), 2), "joke kick energy multiples");
    try { chu2::circleArea(-1); check(false, "negative radius"); }
    catch (const std::invalid_argument&) {}
    try { chu2::rectangleArea(1, -1); check(false, "negative rectangle side"); }
    catch (const std::invalid_argument&) {}
    try { chu2::circleArea(1e308); check(false, "area overflow"); }
    catch (const std::overflow_error&) {}
    check(near(chu2::powerFromEnergyTime(1000, 0.001), 1e6), "short discharge average power");
    check(near(chu2::timeFromEnergyPower(3600000, 1000), 3600), "1kWh charge with 1kW");
    check(near(chu2::energyFromPowerTime(1000, 3600), 3600000), "1kW for 1h");
    check(near(chu2::energyFromPowerTime(chu2::powerFromEnergyTime(1234, 5), 5), 1234), "power energy round trip");
    check(chu2::powerFromEnergyTime(0, 1) == 0 && chu2::timeFromEnergyPower(0, 1) == 0 && chu2::energyFromPowerTime(1, 0) == 0, "zero energy power boundaries");
    const auto watts = chu2::units(chu2::Category::Power);
    check(near(chu2::convert(1, watts[1], watts[0], 343), 1000), "kW to W");
    const auto joules = chu2::units(chu2::Category::Energy);
    check(near(chu2::convert(1, joules[11], joules[0], 343), 3600), "Wh to J");
    try { chu2::powerFromEnergyTime(1, 0); check(false, "zero discharge time"); }
    catch (const std::invalid_argument&) {}
    try { chu2::timeFromEnergyPower(1, 0); check(false, "zero charging power"); }
    catch (const std::invalid_argument&) {}
    try { chu2::energyFromPowerTime(-1, 1); check(false, "negative power"); }
    catch (const std::invalid_argument&) {}
    try { chu2::energyFromPowerTime(1e308, 1e308); check(false, "power energy overflow"); }
    catch (const std::overflow_error&) {}
    try { chu2::powerFromEnergyTime(1e-300, 1e300); check(false, "power underflow"); }
    catch (const std::overflow_error&) {}
    const auto escape = chu2::escapeFromAltitude(6371000, 0, chu2::EARTH_MU);
    check(escape.speed > 11180 && escape.speed < 11190, "Earth surface escape speed");
    check(near(escape.speed / escape.circularSpeed, std::sqrt(2.0)), "escape circular speed ratio");
    const auto highEscape = chu2::escapeFromAltitude(6371000, 6371000, chu2::EARTH_MU);
    check(near(highEscape.speed / escape.speed, 1 / std::sqrt(2.0)), "escape radius scaling");
    check(near(highEscape.gravity / escape.gravity, 0.25), "escape local gravity scaling");
    const auto moonEscape = chu2::escapeFromAltitude(1737400, 0, 4902.800118e9);
    check(moonEscape.speed > 2370 && moonEscape.speed < 2380, "Moon escape speed");
    try { chu2::escapeFromAltitude(1, -1, 1); check(false, "negative escape altitude"); }
    catch (const std::invalid_argument&) {}
    try { chu2::escapeFromAltitude(0, 1, 1); check(false, "zero escape radius"); }
    catch (const std::invalid_argument&) {}
    try { chu2::escapeFromAltitude(1, 1, 0); check(false, "zero escape mu"); }
    catch (const std::invalid_argument&) {}
    try { chu2::escapeFromAltitude(1e308, 1e308, 1); check(false, "escape overflow"); }
    catch (const std::overflow_error&) {}
    const auto drag = chu2::dragFall(false, 10, 1, 9.80665, 1.225, 0.47, 0.01);
    const auto vacuum = chu2::freeFallFromTime(10, 9.80665);
    check(drag.motion.speed < vacuum.speed && drag.motion.distance < vacuum.distance, "drag reduces speed and distance");
    check(drag.terminalSpeed && drag.motion.speed < *drag.terminalSpeed, "drag below terminal speed");
    const auto dragInverse = chu2::dragFall(true, drag.motion.distance, 1, 9.80665, 1.225, 0.47, 0.01);
    check(near(dragInverse.motion.time, 10) && near(dragInverse.motion.speed, drag.motion.speed), "drag time distance round trip");
    const auto knownDrag = chu2::dragFall(false, 1, 1, 1, 2, 1, 1);
    check(near(knownDrag.motion.speed, std::tanh(1.0)) && near(knownDrag.motion.distance, std::log(std::cosh(1.0))), "analytic drag example");
    const auto longDrag = chu2::dragFall(false, 10000, 1, 1, 2, 1, 1);
    check(near(longDrag.motion.speed, 1) && near(longDrag.motion.distance, 10000 - std::log(2.0)), "stable long drag fall");
    const auto noDrag = chu2::dragFall(false, 10, 1, 9.80665, 0, 1, 1);
    check(!noDrag.terminalSpeed && near(noDrag.motion.distance, vacuum.distance), "zero density vacuum");
    const auto zeroDrag = chu2::dragFall(true, 0, 1, 1, 2, 1, 1);
    check(zeroDrag.motion.time == 0 && zeroDrag.terminalSpeed.has_value(), "zero distance has terminal parameter");
    const auto tinyDrag = chu2::dragFall(false, 0.00001, 1, 1, 2, 1, 1);
    check(near(tinyDrag.motion.distance / 5e-11, 1 - 1e-10 / 6), "small time stable drag");
    try { chu2::dragFall(false, 1, 0, 1, 1, 1, 1); check(false, "zero drag mass"); }
    catch (const std::invalid_argument&) {}
    try { chu2::dragFall(false, 1, 1, 1, -1, 1, 1); check(false, "negative density"); }
    catch (const std::invalid_argument&) {}
    const auto fall = chu2::freeFallFromTime(2, 10);
    check(near(fall.speed, 20) && near(fall.distance, 20), "free fall time known values");
    const auto inverseFall = chu2::freeFallFromDistance(20, 10);
    check(near(inverseFall.time, 2) && near(inverseFall.speed, 20), "free fall distance inverse");
    const auto moonFall = chu2::freeFallFromTime(2, 1.62);
    check(near(moonFall.speed, 3.24) && near(moonFall.distance, 3.24), "custom gravity free fall");
    check(chu2::freeFallFromTime(0, 10).distance == 0 && chu2::freeFallFromDistance(0, 10).time == 0, "zero free fall");
    try { chu2::freeFallFromTime(-1, 10); check(false, "negative fall time"); }
    catch (const std::invalid_argument&) {}
    try { chu2::freeFallFromDistance(-1, 10); check(false, "negative fall distance"); }
    catch (const std::invalid_argument&) {}
    try { chu2::freeFallFromTime(1, 0); check(false, "zero fall gravity"); }
    catch (const std::invalid_argument&) {}
    check(near(chu2::freeFallFromDistance(1e308, 1e308).speed / 1e308, std::sqrt(2.0)), "large finite free fall");
    try { chu2::freeFallFromDistance(1.7e308, 1.7e308); check(false, "fall overflow"); }
    catch (const std::overflow_error&) {}
    const auto gravityRotation = chu2::rotationFromAcceleration(100, 9.80665);
    check(near(gravityRotation.period, 20.064092925890403), "1G rotation period");
    const auto gravityRoundTrip = chu2::rotationFromPeriod(100, gravityRotation.period);
    check(near(gravityRoundTrip.acceleration, 9.80665), "rotation inverse round trip");
    const auto jump = chu2::jumpFromHeight(5, 0.5, 10);
    check(near(jump.speed, 10) && near(jump.acceleration, 100) && near(jump.supportAcceleration, 110), "jump height inverse");
    check(near(jump.pushTime, 0.1) && near(jump.peakTime, 1) && near(jump.flightTime, 2), "jump times");
    const auto jumpForward = chu2::projectile(jump.speed, 90, 10);
    check(near(jumpForward.height, 5), "jump forward inverse round trip");
    check(near(chu2::jumpFromHeight(5, 1, 10).acceleration, jump.acceleration / 2), "jump push distance scaling");
    try { chu2::rotationFromAcceleration(100, 0); check(false, "zero target gravity"); }
    catch (const std::invalid_argument&) {}
    try { chu2::rotationFromAcceleration(-1, 1); check(false, "negative rotation radius"); }
    catch (const std::invalid_argument&) {}
    try { chu2::jumpFromHeight(5, 0, 10); check(false, "zero push distance"); }
    catch (const std::invalid_argument&) {}
    try { chu2::jumpFromHeight(0, 1, 10); check(false, "zero jump height"); }
    catch (const std::invalid_argument&) {}
    try { chu2::jumpFromHeight(1, 1, 0); check(false, "zero jump gravity"); }
    catch (const std::invalid_argument&) {}
    try { chu2::jumpFromHeight(1e308, 1e-300, 1e308); check(false, "jump overflow"); }
    catch (const std::overflow_error&) {}
    const auto impact = chu2::impactFromDistance(1, 10, 0.1);
    check(near(impact.energy, 50) && near(impact.force, 500) && near(impact.deceleration, 500) && near(impact.time, 0.02), "impact known example");
    check(near(chu2::impactFromDistance(1, 10, 0.05).force, 2 * impact.force), "impact distance scaling");
    check(chu2::impactFromDistance(1, 0, 1).force == 0, "stationary impact");
    const auto rotation = chu2::rotationFromPeriod(100, 20);
    check(near(rotation.speed, 31.41592653589793) && near(rotation.acceleration, 9.869604401089358) && near(rotation.rpm, 3), "rotation known example");
    check(near(chu2::rotationFromPeriod(100, 10).acceleration, 4 * rotation.acceleration), "rotation period scaling");
    const auto throw30 = chu2::projectile(10, 30, 10);
    check(near(throw30.height, 1.25) && near(throw30.flightTime, 1) && near(throw30.range, 8.660254037844386), "projectile known example");
    const auto vertical = chu2::projectile(10, 90, 10);
    check(vertical.range == 0 && near(vertical.height, 5) && near(vertical.flightTime, 2), "vertical jump");
    check(chu2::projectile(10, 0, 10).range == 0 && chu2::projectile(0, 45, 10).height == 0, "projectile boundary angles and rest");
    check(near(chu2::projectile(10, 45, 5).range, 2 * chu2::projectile(10, 45, 10).range), "projectile gravity scaling");
    const auto angles = chu2::units(chu2::Category::Angle);
    check(near(chu2::convert(3.141592653589793, angles[1], angles[0], 343), 180), "radian input");
    try { chu2::impactFromDistance(1, 10, 0); check(false, "zero stopping distance"); }
    catch (const std::invalid_argument&) {}
    try { chu2::impactFromDistance(0, 10, 1); check(false, "zero impact mass"); }
    catch (const std::invalid_argument&) {}
    try { chu2::rotationFromPeriod(100, 0); check(false, "zero rotation period"); }
    catch (const std::invalid_argument&) {}
    try { chu2::projectile(10, 91, 10); check(false, "invalid launch angle"); }
    catch (const std::invalid_argument&) {}
    try { chu2::projectile(10, 45, 0); check(false, "zero gravity"); }
    catch (const std::invalid_argument&) {}
    try { chu2::impactFromDistance(1e308, 1e308, 1); check(false, "impact overflow"); }
    catch (const std::overflow_error&) {}
    try { chu2::rotationFromPeriod(1e308, 1e-300); check(false, "rotation overflow"); }
    catch (const std::overflow_error&) {}
    try { chu2::projectile(1e308, 45, 1e-300); check(false, "projectile overflow"); }
    catch (const std::overflow_error&) {}
    const auto presets = chu2::bodyPresets();
    check(presets.size() == 17, "available body presets");
    for (const auto& body : presets)
    {
        check(body.radiusKm > 0 && body.mu > 0 && !body.name.empty(), "usable body preset");
        const auto orbit = chu2::orbitFromAltitude(body.radiusKm * 1000, 400000, body.mu);
        const auto reverse = chu2::orbitFromSpeed(body.radiusKm * 1000, orbit.speed, body.mu);
        check(near(reverse.altitude, 400000), "preset orbit round trip in SI");
    }
    const auto& moon = presets[1];
    const auto moonOrbit = chu2::orbitFromAltitude(moon.radiusKm * 1000, 100000, moon.mu);
    check(moonOrbit.speed > 1600 && moonOrbit.speed < 1700, "Moon preset physical scale");
    const auto& juno = presets[11];
    const auto junoOrbit = chu2::orbitFromAltitude(juno.radiusKm * 1000, 0, juno.mu);
    check(junoOrbit.speed > 100 && junoOrbit.speed < 130, "Juno estimated physical scale");
    const auto toyOrbit = chu2::orbitFromAltitude(5, 5, 1000);
    check(near(toyOrbit.speed, 10) && near(toyOrbit.period, 6.283185307179586), "analytic circular orbit");
    const auto inverseOrbit = chu2::orbitFromSpeed(5, 10, 1000);
    check(near(inverseOrbit.altitude, 5) && near(inverseOrbit.radius, 10), "inverse circular orbit");
    const auto lowOrbit = chu2::orbitFromAltitude(6371000, 400000, chu2::EARTH_MU);
    check(lowOrbit.speed > 7672 && lowOrbit.speed < 7674 && lowOrbit.period > 5540 && lowOrbit.period < 5550, "400km Earth orbit");
    const auto reverseOrbit = chu2::orbitFromSpeed(6371000, lowOrbit.speed, chu2::EARTH_MU);
    check(near(reverseOrbit.altitude, 400000) && near(reverseOrbit.period, lowOrbit.period), "orbit round trip");
    const auto surface = chu2::orbitFromAltitude(6371000, 0, chu2::EARTH_MU);
    check(chu2::orbitFromSpeed(6371000, surface.speed, chu2::EARTH_MU).altitude == 0, "surface orbit roundoff");
    try { chu2::orbitFromAltitude(0, 1, 1); check(false, "zero body radius"); }
    catch (const std::invalid_argument&) {}
    try { chu2::orbitFromAltitude(1, -1, 1); check(false, "negative altitude"); }
    catch (const std::invalid_argument&) {}
    try { chu2::orbitFromAltitude(1, 1, -1); check(false, "negative mu"); }
    catch (const std::invalid_argument&) {}
    try { chu2::orbitFromSpeed(1, 0, 1); check(false, "zero orbit speed"); }
    catch (const std::invalid_argument&) {}
    try { chu2::orbitFromSpeed(5, 100, 1000); check(false, "inside body orbit"); }
    catch (const std::domain_error&) {}
    try { chu2::orbitFromAltitude(1e308, 1e308, 1); check(false, "orbit overflow"); }
    catch (const std::overflow_error&) {}
    const auto energyUnits = chu2::units(chu2::Category::Energy);
    check(near(chu2::kineticEnergy(1, 10), 50), "1000g at 10m/s");
    check(near(chu2::kineticEnergy(chu2::convert(1000, mass[0], mass[1], 343), 10), 50), "grams input");
    const double carEnergy = chu2::kineticEnergy(1000, chu2::convert(60, speed[5], speed[0], 343));
    check(near(carEnergy, 138888.8888888889), "car at 60km/h");
    check(near(chu2::convert(4184, energyUnits[0], energyUnits[6], 343), 1), "one gram TNT");
    check(near(chu2::convert(4.184e9, energyUnits[0], energyUnits[8], 343), 1), "one tonne TNT");
    check(near(chu2::convert(4.184e15, energyUnits[0], energyUnits[10], 343), 1), "one megatonne TNT");
    check(chu2::kineticEnergy(1, 0) == 0 && chu2::kineticEnergy(0, 1e308) == 0, "zero energy");
    check(near(chu2::kineticEnergy(1e-300, 1e200), 5e99), "finite energy despite large speed squared");
    check(near(chu2::kineticEnergy(1e300, 1e-200) / 5e-101, 1), "finite energy despite tiny speed squared");
    for (double invalid : {-1.0, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::quiet_NaN()})
    {
        try { chu2::kineticEnergy(invalid, 10); check(false, "invalid energy mass"); }
        catch (const std::invalid_argument&) {}
        try { chu2::kineticEnergy(1, invalid); check(false, "invalid energy speed"); }
        catch (const std::invalid_argument&) {}
    }
    try { chu2::kineticEnergy(1e308, 1e308); check(false, "energy overflow"); }
    catch (const std::overflow_error&) {}
    try { chu2::kineticEnergy(1e-300, 1e-200); check(false, "energy underflow"); }
    catch (const std::overflow_error&) {}
    const auto motion = chu2::fromAccelerationTime(9.80665, 10);
    check(near(motion.speed, 98.0665) && near(motion.distance, 490.3325), "1G for 10 seconds");
    const auto reverse = chu2::fromSpeedDistance(100, 500);
    check(near(reverse.acceleration, 10) && near(reverse.time, 10), "inverse motion");
    const auto roundTrip = chu2::fromSpeedDistance(motion.speed, motion.distance);
    check(near(roundTrip.acceleration, 9.80665) && near(roundTrip.time, 10), "motion round trip");
    check(chu2::fromAccelerationTime(0, 10).distance == 0, "zero acceleration");
    check(chu2::fromAccelerationTime(10, 0).speed == 0, "zero time");
    try { chu2::fromAccelerationTime(-1, 1); check(false, "negative acceleration"); }
    catch (const std::invalid_argument&) {}
    try { chu2::fromSpeedDistance(0, 1); check(false, "zero inverse speed"); }
    catch (const std::invalid_argument&) {}
    try { chu2::fromSpeedDistance(1, -1); check(false, "negative inverse distance"); }
    catch (const std::invalid_argument&) {}
    try { chu2::fromAccelerationTime(1e308, 1e308); check(false, "motion overflow"); }
    catch (const std::overflow_error&) {}
    return failures ? 1 : 0;
}
