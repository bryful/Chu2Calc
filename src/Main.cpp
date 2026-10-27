#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <dwmapi.h>
#include <uxtheme.h>
#include <algorithm>
#include <array>
#include <cstring>
#include <exception>
#include <filesystem>
#include <string>
#include <stdexcept>
#include <vector>
#include "Units.h"
#include "Motion.h"
#include "Energy.h"
#include "Orbit.h"
#include "BodyPresets.h"
#include "Tips.h"
#include "Mechanics.h"
#include "DragFall.h"
#include "Resource.h"
#include "Area.h"

namespace
{
constexpr int CATEGORY = 101, VALUE = 102, UNIT = 103, SOUND = 104, COPY = 105, FONT = 106, THEME = 107;
constexpr int SECOND_VALUE = 108, SECOND_UNIT = 109;
constexpr int MU = 110;
constexpr int BODY_PRESET = 111;
constexpr int THIRD_VALUE = 112, THIRD_UNIT = 113;
constexpr int AIR_DENSITY = 114, DRAG_COEFFICIENT = 115, DRAG_AREA = 116;
constexpr int AREA_REFERENCE = 117;
bool isArea(int page) { return page >= 21 && page <= 23; }
bool isOrbit(int page) { return page == 6 || page == 7 || page == 17; }
bool isDragFall(int page) { return page == 15 || page == 16; }
bool hasThirdInput(int page) { return page == 8 || page == 10 || page == 12 || isDragFall(page); }
constexpr const wchar_t* PAGE_NAMES[] = {L"速度変換", L"距離変換", L"質量変換", L"加速度と時間", L"速度と距離", L"運動エネルギー・TNT", L"人工衛星：高度から", L"人工衛星：速度から", L"衝撃力の目安", L"人工重力：周期から", L"跳躍・投射", L"人工重力：Gから", L"跳躍：高さから", L"自由落下：時間から", L"自由落下：距離から", L"抵抗あり：時間から", L"抵抗あり：距離から", L"脱出速度", L"電力：Eと時間から", L"充電時間：Eと電力", L"エネルギー：電力から", L"面積：直接入力", L"面積：半径から", L"面積：縦横から"};
// 画面IDと表示順を分け、対応する順算・逆算を隣に配置する。
constexpr int NAV_ORDER[] = {0, 1, 2, 21, 22, 23, 3, 4, 10, 12, 13, 14, 15, 16, 5, 8, 18, 19, 20, 9, 11, 6, 7, 17};
struct App
{
    HWND window{}, navigation{}, heading{}, valueLabel{}, value{}, unit{}, soundLabel{}, sound{}, results{}, status{}, copy{}, fontButton{};
    HWND themeButton{};
    HWND secondLabel{}, secondValue{}, secondUnit{}, assumption{};
    HWND muLabel{}, mu{};
    HWND presetLabel{}, preset{};
    HWND tipsLabel{}, tips{};
    HWND thirdLabel{}, thirdValue{}, thirdUnit{};
    std::array<HWND, 3> airLabels{}, airInputs{};
    HWND areaReferenceLabel{}, areaReference{};
    HFONT font{};
    HBRUSH background{}, surface{};
    bool dark = true;
    COLORREF backgroundColor() const { return dark ? RGB(24, 26, 30) : RGB(250, 250, 250); }
    COLORREF surfaceColor() const { return dark ? RGB(36, 39, 45) : RGB(255, 255, 255); }
    COLORREF textColor() const { return dark ? RGB(232, 234, 238) : RGB(24, 26, 30); }
    LOGFONTW fontSpec{};
    std::filesystem::path settings;
    int page = 0;
    UINT dpi = 96;
    bool changing = false;
    std::array<std::wstring, 24> values{L"1", L"1", L"1", L"9.80665", L"100", L"1000", L"400", L"7.67", L"1000", L"100", L"10", L"100", L"5", L"10", L"100", L"10", L"100", L"0", L"1000", L"1", L"1", L"46755", L"100", L"100"};
    std::array<int, 24> selectedUnits{0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 1, 0, 0, 0};
    std::array<std::wstring, 21> secondValues{L"10", L"500", L"10", L"6371", L"6371", L"10", L"20", L"45", L"1", L"0.5", L"9.80665", L"9.80665", L"1", L"1", L"6371", L"1", L"1", L"1", L"", L"", L"50"};
    std::array<int, 21> secondSelectedUnits{0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 2, 0, 0, 0};
    std::array<std::wstring, 9> thirdValues{L"0.1", L"", L"9.80665", L"", L"9.80665", L"", L"", L"9.80665", L"9.80665"};
    std::array<int, 9> thirdSelectedUnits{};
    std::vector<std::pair<std::wstring, std::wstring>> resultRows;
    std::vector<HWND> controls;
    int px(int valueInDip) const { return MulDiv(valueInDip, static_cast<int>(dpi), 96); }
};
LRESULT CALLBACK headerProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR id, DWORD_PTR data)
{
    auto& app = *reinterpret_cast<App*>(data);
    if (message == WM_PAINT)
    {
        PAINTSTRUCT paint{};
        HDC dc = BeginPaint(window, &paint);
        RECT bounds{};
        GetClientRect(window, &bounds);
        FillRect(dc, &bounds, app.background);
        HGDIOBJ previous = SelectObject(dc, app.font);
        SetTextColor(dc, app.textColor());
        SetBkMode(dc, TRANSPARENT);
        for (int index = 0; index < Header_GetItemCount(window); ++index)
        {
            wchar_t text[64]{};
            HDITEMW item{};
            item.mask = HDI_TEXT;
            item.pszText = text;
            item.cchTextMax = static_cast<int>(std::size(text));
            Header_GetItem(window, index, &item);
            RECT rect{};
            Header_GetItemRect(window, index, &rect);
            rect.left += app.px(6);
            DrawTextW(dc, text, -1, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
        }
        SelectObject(dc, previous);
        EndPaint(window, &paint);
        return 0;
    }
    if (message == WM_ERASEBKGND) return 1;
    if (message == WM_NCDESTROY) RemoveWindowSubclass(window, headerProc, id);
    return DefSubclassProc(window, message, wParam, lParam);
}
void applyTheme(App& app)
{
    if (app.background) DeleteObject(app.background);
    if (app.surface) DeleteObject(app.surface);
    app.background = CreateSolidBrush(app.backgroundColor());
    app.surface = CreateSolidBrush(app.surfaceColor());
    for (HWND control : app.controls) SetWindowTheme(control, L"", L"");
    SetWindowTheme(ListView_GetHeader(app.results), L"", L"");
    ListView_SetBkColor(app.results, app.surfaceColor());
    ListView_SetTextBkColor(app.results, app.surfaceColor());
    ListView_SetTextColor(app.results, app.textColor());
    ListView_SetExtendedListViewStyle(app.results, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | (app.dark ? 0 : LVS_EX_GRIDLINES));
    const BOOL dark = app.dark;
    DwmSetWindowAttribute(app.window, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));
    SetWindowTextW(app.themeButton, app.dark ? L"ライトモードに切替" : L"ダークモードに切替");
    RedrawWindow(app.window, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
}
std::wstring readText(HWND control)
{
    const int length = GetWindowTextLengthW(control);
    std::wstring result(static_cast<size_t>(length) + 1, L'\0');
    GetWindowTextW(control, result.data(), length + 1);
    result.resize(length);
    return result;
}
void applyFont(App& app)
{
    LOGFONTW spec = app.fontSpec;
    spec.lfHeight = -MulDiv(std::max(8L, app.fontSpec.lfHeight), static_cast<int>(app.dpi), 72);
    HFONT font = CreateFontIndirectW(&spec);
    if (!font) return;
    for (HWND control : app.controls) SendMessageW(control, WM_SETFONT, reinterpret_cast<WPARAM>(font), TRUE);
    if (app.font) DeleteObject(app.font);
    app.font = font;
}
void layout(App& app)
{
    RECT bounds{};
    GetClientRect(app.window, &bounds);
    const int width = MulDiv(bounds.right, 96, static_cast<int>(app.dpi));
    const int height = MulDiv(bounds.bottom, 96, static_cast<int>(app.dpi));
    auto move = [&](HWND control, int x, int y, int w, int h)
    {
        MoveWindow(control, app.px(x), app.px(y), app.px(w), app.px(h), TRUE);
    };
    move(app.navigation, 16, 24, 150, height - 140);
    move(app.themeButton, 16, height - 100, 150, 32);
    move(app.fontButton, 16, height - 56, 150, 32);
    const int x = 190, content = width - x - 24;
    const int orbitOffset = isOrbit(app.page) ? 44 : 0;
    move(app.heading, x, 24, content, 32);
    move(app.presetLabel, x, 72, 110, 24);
    move(app.preset, x + 120, 68, content - 120, 420);
    move(app.valueLabel, x, 72 + orbitOffset, content, 24);
    move(app.value, x, 100 + orbitOffset, content - 190, 30);
    move(app.unit, width - 204, 100 + orbitOffset, 180, 320);
    const bool motion = app.page >= 3;
    move(app.secondLabel, x, 142 + orbitOffset, content, 24);
    move(app.secondValue, x, 170 + orbitOffset, content - 190, 30);
    move(app.secondUnit, width - 204, 170 + orbitOffset, 180, 320);
    move(app.thirdLabel, x, 212, content, 24);
    move(app.thirdValue, x, 240, content - 190, 30);
    move(app.thirdUnit, width - 204, 240, 180, 320);
    move(app.soundLabel, x, motion ? 214 : 146, 240, 24);
    move(app.sound, x + 244, motion ? 210 : 142, 150, 30);
    move(app.muLabel, x, 214 + orbitOffset, 260, 24);
    move(app.mu, x + 264, 210 + orbitOffset, content - 264, 30);
    move(app.areaReferenceLabel, x, 214, 110, 24);
    move(app.areaReference, x + 120, 210, content - 120, 250);
    for (int index = 0; index < 3; ++index)
    {
        const int columnWidth = content / 3;
        move(app.airLabels[index], x + index * columnWidth, 280, columnWidth - 12, 24);
        move(app.airInputs[index], x + index * columnWidth, 308, columnWidth - 12, 30);
    }
    const int thirdOffset = isDragFall(app.page) ? 100 : hasThirdInput(app.page) ? 34 : 0;
    move(app.assumption, x, 250 + orbitOffset + thirdOffset, content, app.page >= 5 ? 52 : 26);
    const int resultsTop = app.page >= 5 ? 310 + orbitOffset + thirdOffset : motion ? 286 : 190;
    const int tipsHeight = std::clamp(height / 5, 110, 180);
    const int tipsTop = height - 24 - tipsHeight;
    move(app.results, x, resultsTop, content, std::max(50, tipsTop - 88 - resultsTop));
    move(app.status, x, tipsTop - 72, content, 24);
    move(app.copy, x, tipsTop - 42, 240, 30);
    move(app.tipsLabel, x, tipsTop, content, 24);
    move(app.tips, x, tipsTop + 28, content, tipsHeight - 28);
    ListView_SetColumnWidth(app.results, 0, app.px(185));
    ListView_SetColumnWidth(app.results, 1, app.px(std::max(160, content - 210)));
}
chu2::Category firstCategory(int page)
{
    if (page == 3) return chu2::Category::Acceleration;
    if (page == 4) return chu2::Category::Speed;
    if (page == 5) return chu2::Category::Mass;
    if (page == 6) return chu2::Category::Distance;
    if (page == 7) return chu2::Category::Speed;
    if (page == 8) return chu2::Category::Mass;
    if (page == 9) return chu2::Category::Distance;
    if (page == 10) return chu2::Category::Speed;
    if (page == 11 || page == 12) return chu2::Category::Distance;
    if (page == 13) return chu2::Category::Time;
    if (page == 14) return chu2::Category::Distance;
    if (page == 15) return chu2::Category::Time;
    if (page == 16) return chu2::Category::Distance;
    if (page == 17) return chu2::Category::Distance;
    if (page == 18 || page == 19) return chu2::Category::Energy;
    if (page == 20) return chu2::Category::Power;
    if (page == 21) return chu2::Category::Area;
    if (page == 22 || page == 23) return chu2::Category::Distance;
    return static_cast<chu2::Category>(page);
}
chu2::Category secondCategory(int page)
{
    if (isArea(page)) return chu2::Category::Distance;
    if (page == 18 || page == 20) return chu2::Category::Time;
    if (page == 19) return chu2::Category::Power;
    if (page == 17) return chu2::Category::Distance;
    if (page == 3 || page == 9) return chu2::Category::Time;
    if (page == 5 || page == 8) return chu2::Category::Speed;
    if (page == 10) return chu2::Category::Angle;
    if (isDragFall(page)) return chu2::Category::Mass;
    if (page == 11 || page >= 13) return chu2::Category::Acceleration;
    return chu2::Category::Distance;
}
chu2::Category thirdCategory(int page)
{
    return page == 8 ? chu2::Category::Distance : chu2::Category::Acceleration;
}
void calculate(App& app)
{
    if (app.changing) return;
    const auto units = chu2::units(firstCategory(app.page));
    double value = 0, sound = 0;
    const int selectedItem = static_cast<int>(SendMessageW(app.unit, CB_GETCURSEL, 0, 0));
    const int selected = selectedItem < 0 ? -1 : static_cast<int>(SendMessageW(app.unit, CB_GETITEMDATA, selectedItem, 0));
    sound = 343;
    const bool soundValid = app.page >= 6 || (chu2::parseNumber(readText(app.sound), sound) && sound > 0);
    const bool valid = chu2::parseNumber(readText(app.value), value)
        && soundValid
        && selected >= 0 && static_cast<size_t>(selected) < units.size();
    bool overflow = false;
    std::wstring error;
    bool invalidArguments = false;
    app.resultRows.clear();
    auto append = [&](double amount, chu2::Category category, std::wstring_view prefix)
    {
        const auto targets = chu2::units(category);
        for (const auto& target : targets)
        {
            std::wstring text;
            try { text = chu2::formatNumber(chu2::convert(amount, targets[0], target, sound)); }
            catch (const std::exception&) { text = L"範囲外"; overflow = true; }
            app.resultRows.emplace_back(std::wstring(prefix) + std::wstring(target.name), text);
        }
    };
    if (app.page < 3)
    {
        for (const auto& target : units)
        {
            std::wstring text = L"—";
            if (valid)
            {
                try { text = chu2::formatNumber(chu2::convert(value, units[selected], target, sound)); }
                catch (const std::exception&) { text = L"範囲外"; overflow = true; }
            }
            app.resultRows.emplace_back(target.name, text);
        }
    }
    else if (valid && isArea(app.page))
    {
        try
        {
            const double firstSI = chu2::convert(value, units[selected], units[0], sound);
            if (firstSI < 0) throw std::invalid_argument("Negative area input");
            double area = firstSI;
            if (app.page == 22) area = chu2::circleArea(firstSI);
            if (app.page == 23)
            {
                const auto lengths = chu2::units(chu2::Category::Distance);
                double height{};
                const int itemIndex = static_cast<int>(SendMessageW(app.secondUnit, CB_GETCURSEL, 0, 0));
                const int unitIndex = itemIndex < 0 ? -1 : static_cast<int>(SendMessageW(app.secondUnit, CB_GETITEMDATA, itemIndex, 0));
                if (!chu2::parseNumber(readText(app.secondValue), height) || unitIndex < 0 || static_cast<size_t>(unitIndex) >= lengths.size())
                    throw std::invalid_argument("Invalid rectangle height");
                area = chu2::rectangleArea(firstSI, chu2::convert(height, lengths[unitIndex], lengths[0], sound));
            }
            const auto references = chu2::areaReferences();
            const int referenceIndex = static_cast<int>(SendMessageW(app.areaReference, CB_GETCURSEL, 0, 0));
            if (referenceIndex < 0 || static_cast<size_t>(referenceIndex) >= references.size())
                throw std::invalid_argument("Invalid area reference");
            const auto areaUnits = chu2::units(chu2::Category::Area);
            app.resultRows.emplace_back(references[referenceIndex].name,
                chu2::formatNumber(chu2::convert(area, areaUnits[0], references[referenceIndex], sound)));
            append(area, chu2::Category::Area, L"面積 / ");
            app.resultRows.emplace_back(L"比較基準1個・1畳 / m²", chu2::formatNumber(references[referenceIndex].factor));
        }
        catch (const std::invalid_argument&) { error = L"面積・半径・縦横には0以上の数値を入力してください。"; }
        catch (const std::exception&) { error = L"計算可能範囲を超えています。入力値を調整してください。"; }
    }
    else if (valid)
    {
        double second{};
        const auto secondUnits = chu2::units(secondCategory(app.page));
        const int secondItem = static_cast<int>(SendMessageW(app.secondUnit, CB_GETCURSEL, 0, 0));
        const int selectedSecond = secondItem < 0 ? -1 : static_cast<int>(SendMessageW(app.secondUnit, CB_GETITEMDATA, secondItem, 0));
        if (!chu2::parseNumber(readText(app.secondValue), second) || selectedSecond < 0 || static_cast<size_t>(selectedSecond) >= secondUnits.size())
            error = L"2つの入力欄に数値を入力してください。";
        else
        {
            try
            {
                const auto& firstBase = units[app.page == 5 || app.page == 8 ? 1 : 0];
                const double firstSI = chu2::convert(value, units[selected], firstBase, sound);
                const double secondSI = chu2::convert(second, secondUnits[selectedSecond], secondUnits[isDragFall(app.page) ? 1 : 0], sound);
                double thirdSI = 0;
                if (hasThirdInput(app.page))
                {
                    double third{};
                    const auto thirdUnits = chu2::units(thirdCategory(app.page));
                    const int itemIndex = static_cast<int>(SendMessageW(app.thirdUnit, CB_GETCURSEL, 0, 0));
                    if (itemIndex < 0 || !chu2::parseNumber(readText(app.thirdValue), third))
                        throw std::invalid_argument("Invalid third input");
                    const int unitIndex = static_cast<int>(SendMessageW(app.thirdUnit, CB_GETITEMDATA, itemIndex, 0));
                    if (unitIndex < 0 || static_cast<size_t>(unitIndex) >= thirdUnits.size())
                        throw std::invalid_argument("Invalid third unit");
                    thirdSI = chu2::convert(third, thirdUnits[unitIndex], thirdUnits[0], sound);
                }
                if (app.page == 18)
                {
                    append(chu2::powerFromEnergyTime(firstSI, secondSI), chu2::Category::Power, L"平均出力 / ");
                }
                else if (app.page == 19)
                {
                    append(chu2::timeFromEnergyPower(firstSI, secondSI), chu2::Category::Time, L"所要時間 / ");
                }
                else if (app.page == 20)
                {
                    append(chu2::energyFromPowerTime(firstSI, secondSI), chu2::Category::Energy, L"");
                }
                else if (isDragFall(app.page))
                {
                    std::array<double, 3> air{};
                    for (int index = 0; index < 3; ++index)
                        if (!chu2::parseNumber(readText(app.airInputs[index]), air[index]))
                            throw std::invalid_argument("Invalid air input");
                    const auto result = chu2::dragFall(app.page == 16, firstSI, secondSI, thirdSI, air[0], air[1], air[2]);
                    const auto speeds = chu2::units(chu2::Category::Speed);
                    app.resultRows.emplace_back(L"落下速度 / m/s", chu2::formatNumber(result.motion.speed));
                    app.resultRows.emplace_back(L"落下速度 / km/h", chu2::formatNumber(chu2::convert(result.motion.speed, speeds[0], speeds[5], sound)));
                    app.resultRows.emplace_back(L"落下距離 / m", chu2::formatNumber(result.motion.distance));
                    app.resultRows.emplace_back(L"落下時間 / 秒", chu2::formatNumber(result.motion.time));
                    app.resultRows.emplace_back(L"終端速度 / m/s", result.terminalSpeed ? chu2::formatNumber(*result.terminalSpeed) : L"なし（空気抵抗0）");
                    app.resultRows.emplace_back(L"終端速度 / km/h", result.terminalSpeed ? chu2::formatNumber(chu2::convert(*result.terminalSpeed, speeds[0], speeds[5], sound)) : L"なし（空気抵抗0）");
                }
                else if (app.page == 13 || app.page == 14)
                {
                    const auto result = app.page == 13 ? chu2::freeFallFromTime(firstSI, secondSI) : chu2::freeFallFromDistance(firstSI, secondSI);
                    const auto speeds = chu2::units(chu2::Category::Speed);
                    app.resultRows.emplace_back(L"落下速度 / m/s", chu2::formatNumber(result.speed));
                    app.resultRows.emplace_back(L"落下速度 / km/h", chu2::formatNumber(chu2::convert(result.speed, speeds[0], speeds[5], sound)));
                    app.resultRows.emplace_back(L"落下距離 / m", chu2::formatNumber(result.distance));
                    app.resultRows.emplace_back(L"落下距離 / km", chu2::formatNumber(result.distance / 1000));
                    app.resultRows.emplace_back(L"落下時間 / 秒", chu2::formatNumber(result.time));
                    app.resultRows.emplace_back(L"落下時間 / 分", chu2::formatNumber(result.time / 60));
                    app.resultRows.emplace_back(L"重力 / m/s²", chu2::formatNumber(result.acceleration));
                    app.resultRows.emplace_back(L"重力 / G", chu2::formatNumber(result.acceleration / 9.80665));
                }
                else if (app.page == 8)
                {
                    const auto result = chu2::impactFromDistance(firstSI, secondSI, thirdSI);
                    append(result.force, chu2::Category::Force, L"平均の力 / ");
                    append(result.deceleration, chu2::Category::Acceleration, L"減速 / ");
                    app.resultRows.emplace_back(L"停止時間 / 秒", chu2::formatNumber(result.time));
                    app.resultRows.emplace_back(L"エネルギー / J", chu2::formatNumber(result.energy));
                }
                else if (app.page == 9 || app.page == 11)
                {
                    const auto result = app.page == 9 ? chu2::rotationFromPeriod(firstSI, secondSI) : chu2::rotationFromAcceleration(firstSI, secondSI);
                    if (app.page == 11)
                    {
                        app.resultRows.emplace_back(L"回転周期 / 秒", chu2::formatNumber(result.period));
                        app.resultRows.emplace_back(L"回転周期 / 分", chu2::formatNumber(result.period / 60));
                    }
                    append(result.acceleration, chu2::Category::Acceleration, L"遠心加速度 / ");
                    const auto speeds = chu2::units(chu2::Category::Speed);
                    app.resultRows.emplace_back(L"接線速度 / m/s", chu2::formatNumber(result.speed));
                    app.resultRows.emplace_back(L"接線速度 / km/h", chu2::formatNumber(chu2::convert(result.speed, speeds[0], speeds[5], sound)));
                    app.resultRows.emplace_back(L"回転数 / rpm", chu2::formatNumber(result.rpm));
                }
                else if (app.page == 12)
                {
                    const auto result = chu2::jumpFromHeight(firstSI, secondSI, thirdSI);
                    app.resultRows.emplace_back(L"離陸速度 / m/s", chu2::formatNumber(result.speed));
                    const auto speeds = chu2::units(chu2::Category::Speed);
                    app.resultRows.emplace_back(L"離陸速度 / km/h", chu2::formatNumber(chu2::convert(result.speed, speeds[0], speeds[5], sound)));
                    app.resultRows.emplace_back(L"正味の加速度 / m/s²", chu2::formatNumber(result.acceleration));
                    app.resultRows.emplace_back(L"正味の加速度 / G", chu2::formatNumber(result.acceleration / 9.80665));
                    app.resultRows.emplace_back(L"支持力÷質量 / m/s²", chu2::formatNumber(result.supportAcceleration));
                    app.resultRows.emplace_back(L"体感G（支持力）", chu2::formatNumber(result.supportAcceleration / 9.80665));
                    app.resultRows.emplace_back(L"踏み切り時間 / 秒", chu2::formatNumber(result.pushTime));
                    app.resultRows.emplace_back(L"離陸→頂点 / 秒", chu2::formatNumber(result.peakTime));
                    app.resultRows.emplace_back(L"離陸後の滞空 / 秒", chu2::formatNumber(result.flightTime));
                }
                else if (app.page == 10)
                {
                    const auto result = chu2::projectile(firstSI, secondSI, thirdSI);
                    app.resultRows.emplace_back(L"最高到達高さ / m", chu2::formatNumber(result.height));
                    app.resultRows.emplace_back(L"水平飛距離 / m", chu2::formatNumber(result.range));
                    app.resultRows.emplace_back(L"滞空時間 / 秒", chu2::formatNumber(result.flightTime));
                    app.resultRows.emplace_back(L"頂点まで / 秒", chu2::formatNumber(result.peakTime));
                    app.resultRows.emplace_back(L"水平初速 / m/s", chu2::formatNumber(result.horizontalSpeed));
                    app.resultRows.emplace_back(L"鉛直初速 / m/s", chu2::formatNumber(result.verticalSpeed));
                }
                else if (app.page == 17)
                {
                    double mu{};
                    if (!chu2::parseNumber(readText(app.mu), mu) || mu <= 0)
                        throw std::invalid_argument("Invalid mu");
                    const auto result = chu2::escapeFromAltitude(secondSI, firstSI, mu);
                    const auto surface = chu2::escapeFromAltitude(secondSI, 0, mu);
                    const auto speeds = chu2::units(chu2::Category::Speed);
                    app.resultRows.emplace_back(L"脱出速度 / km/s", chu2::formatNumber(result.speed / 1000));
                    app.resultRows.emplace_back(L"脱出速度 / m/s", chu2::formatNumber(result.speed));
                    app.resultRows.emplace_back(L"脱出速度 / km/h", chu2::formatNumber(chu2::convert(result.speed, speeds[0], speeds[5], sound)));
                    app.resultRows.emplace_back(L"表面の脱出速度 / km/s", chu2::formatNumber(surface.speed / 1000));
                    app.resultRows.emplace_back(L"同高度の円軌道 / km/s", chu2::formatNumber(result.circularSpeed / 1000));
                    app.resultRows.emplace_back(L"同高度の重力 / m/s²", chu2::formatNumber(result.gravity));
                    app.resultRows.emplace_back(L"同高度の重力 / G", chu2::formatNumber(result.gravity / 9.80665));
                }
                else if (isOrbit(app.page))
                {
                    double mu{};
                    if (!chu2::parseNumber(readText(app.mu), mu) || mu <= 0)
                        throw std::invalid_argument("Invalid mu");
                    const auto result = app.page == 6 ? chu2::orbitFromAltitude(secondSI, firstSI, mu) : chu2::orbitFromSpeed(secondSI, firstSI, mu);
                    auto row = [&](const wchar_t* label, double number)
                    {
                        app.resultRows.emplace_back(label, chu2::formatNumber(number));
                    };
                    row(L"高度 / km", result.altitude / 1000);
                    row(L"高度 / m", result.altitude);
                    row(L"軌道半径 / km", result.radius / 1000);
                    row(L"天体半径 / km", secondSI / 1000);
                    row(L"速度 / m/s", result.speed);
                    row(L"速度 / km/s", result.speed / 1000);
                    const auto speedUnits = chu2::units(chu2::Category::Speed);
                    row(L"速度 / km/h", chu2::convert(result.speed, speedUnits[0], speedUnits[5], sound));
                    append(result.period, chu2::Category::Time, L"周回時間 / ");
                }
                else if (app.page == 5)
                {
                    append(chu2::kineticEnergy(firstSI, secondSI), chu2::Category::Energy, L"");
                }
                else if (app.page == 3)
                {
                    const auto result = chu2::fromAccelerationTime(firstSI, secondSI);
                    append(result.speed, chu2::Category::Speed, L"速度 / ");
                    append(result.distance, chu2::Category::Distance, L"距離 / ");
                }
                else
                {
                    const auto result = chu2::fromSpeedDistance(firstSI, secondSI);
                    append(result.acceleration, chu2::Category::Acceleration, L"加速度 / ");
                    append(result.time, chu2::Category::Time, L"時間 / ");
                }
            }
            catch (const std::invalid_argument&)
            {
                invalidArguments = true;
                error = app.page == 8 ? L"質量と停止距離は正、速さは0以上で入力してください。" : app.page == 9 ? L"半径と回転周期は0より大きい値を入力してください。" : app.page == 10 ? L"初速は0以上、角度は0～90度、重力は正の値にしてください。" : isOrbit(app.page) ? L"天体半径・μ・速度は正の値、高度は0以上にしてください。" : app.page == 5 ? L"質量と速さには0以上の値を入力してください。" : app.page == 3 ? L"加速度と時間には0以上の値を入力してください。" : L"速度と距離には0より大きい値を入力してください。";
            }
            catch (const std::domain_error&) { error = L"この速度の円軌道は天体内部になります。速度を下げてください。"; }
            catch (const std::exception&) { error = L"計算可能範囲を超えています。入力値を調整してください。"; }
        }
    }
    if (invalidArguments && app.page == 11)
        error = L"回転半径と目標重力には正の値を入力してください。";
    if (invalidArguments && app.page == 12)
        error = L"高さ・踏み切り距離・重力には正の値を入力してください。";
    if (invalidArguments && (app.page == 13 || app.page == 14))
        error = L"時間・距離は0以上、重力は正の値を入力してください。";
    if (invalidArguments && isDragFall(app.page))
        error = L"質量・重力は正、時間・距離・密度・Cd・面積は0以上にしてください。";
    if (invalidArguments && app.page == 18) error = L"エネルギーは0以上、放出時間は正の値を入力してください。";
    if (invalidArguments && app.page == 19) error = L"エネルギーは0以上、電力は正の値を入力してください。";
    if (invalidArguments && app.page == 20) error = L"電力と時間は0以上の値を入力してください。";
    if (!error.empty()) app.resultRows.clear();
    const int oldSelection = ListView_GetNextItem(app.results, -1, LVNI_SELECTED);
    SendMessageW(app.results, WM_SETREDRAW, FALSE, 0);
    ListView_DeleteAllItems(app.results);
    for (size_t index = 0; index < app.resultRows.size(); ++index)
    {
        auto& row = app.resultRows[index];
        LVITEMW item{};
        item.mask = LVIF_TEXT;
        item.iItem = static_cast<int>(index);
        item.pszText = row.first.data();
        ListView_InsertItem(app.results, &item);
        ListView_SetItemText(app.results, item.iItem, 1, row.second.data());
    }
    if (oldSelection >= 0 && oldSelection < static_cast<int>(app.resultRows.size()))
        ListView_SetItemState(app.results, oldSelection, LVIS_SELECTED, LVIS_SELECTED);
    SendMessageW(app.results, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(app.results, nullptr, TRUE);
    SetWindowTextW(app.status, !valid ? (app.page >= 6 ? L"数値と単位を確認してください。" : L"数値を入力してください。音速は正の値が必要です。") :
        !error.empty() ? error.c_str() : overflow ? L"一部の結果が計算可能範囲を超えています。" : L"計算結果：有効数字12桁 ／ 小数表記（指数表記なし）");
    EnableWindow(app.copy, valid && error.empty() && !app.resultRows.empty());
}
void updateOrbitNote(App& app)
{
    if (!isOrbit(app.page)) return;
    const int selected = static_cast<int>(SendMessageW(app.preset, CB_GETCURSEL, 0, 0));
    const auto presets = chu2::bodyPresets();
    const std::wstring note = std::wstring(app.page == 17 ? L"球対称天体・二体問題。大気・自転・他天体を無視した理論速度です。\n" : L"条件：球対称天体の円軌道・空気抵抗なし。高度0は表面での理論値。\n")
        + std::wstring(selected > 0 && static_cast<size_t>(selected) <= presets.size() ? presets[selected - 1].note : L"カスタム：天体半径とμ（GM）を手入力できます。");
    SetWindowTextW(app.assumption, note.c_str());
}
void applyBodyPreset(App& app)
{
    const int selected = static_cast<int>(SendMessageW(app.preset, CB_GETCURSEL, 0, 0));
    const auto presets = chu2::bodyPresets();
    if (!isOrbit(app.page) || selected <= 0 || static_cast<size_t>(selected) > presets.size())
    {
        updateOrbitNote(app);
        return;
    }
    const auto& body = presets[selected - 1];
    app.changing = true;
    // 半径の単位もkmに揃え、現在の入力へ適用する。
    const auto radius = chu2::formatNumber(body.radiusKm);
    SetWindowTextW(app.secondValue, radius.c_str());
    SendMessageW(app.secondUnit, CB_SETCURSEL, 1, 0);
    SetWindowTextW(app.mu, chu2::formatNumber(body.mu).c_str());
    app.secondValues[3] = app.secondValues[4] = app.secondValues[14] = radius;
    app.secondSelectedUnits[3] = app.secondSelectedUnits[4] = app.secondSelectedUnits[14] = 1;
    app.changing = false;
    updateOrbitNote(app);
    calculate(app);
}
void changePage(App& app, int page)
{
    app.values[app.page] = readText(app.value);
    app.selectedUnits[app.page] = static_cast<int>(SendMessageW(app.unit, CB_GETCURSEL, 0, 0));
    if (app.page >= 3)
    {
        app.secondValues[app.page - 3] = readText(app.secondValue);
        app.secondSelectedUnits[app.page - 3] = static_cast<int>(SendMessageW(app.secondUnit, CB_GETCURSEL, 0, 0));
        if (isOrbit(app.page))
        {
            app.secondValues[3] = app.secondValues[4] = app.secondValues[14] = readText(app.secondValue);
            app.secondSelectedUnits[3] = app.secondSelectedUnits[4] = app.secondSelectedUnits[14] = app.secondSelectedUnits[app.page - 3];
        }
    }
    if (hasThirdInput(app.page))
    {
        app.thirdValues[app.page - 8] = readText(app.thirdValue);
        app.thirdSelectedUnits[app.page - 8] = static_cast<int>(SendMessageW(app.thirdUnit, CB_GETCURSEL, 0, 0));
    }
    app.changing = true;
    app.page = page;
    SetWindowTextW(app.heading, PAGE_NAMES[page]);
    std::wstring tips(chu2::tipsForPage(page));
    SetWindowTextW(app.tips, tips.c_str());
    SendMessageW(app.tips, EM_SETSEL, 0, 0);
    SendMessageW(app.tips, EM_SCROLLCARET, 0, 0);
    SetWindowTextW(app.valueLabel, page == 8 ? L"衝突する物体の質量" : page == 9 ? L"回転半径" : page == 10 ? L"投射・跳躍の初速" : page == 6 ? L"天体表面からの高度" : page == 7 ? L"円軌道の速度" : page == 5 ? L"動いている物体の質量" : page == 3 ? L"加速度" : page == 4 ? L"到達速度" : L"値を入力し、単位を選択");
    if (page == 11) SetWindowTextW(app.valueLabel, L"回転半径");
    if (page == 12) SetWindowTextW(app.valueLabel, L"離陸位置からの目標高さ（真上への跳躍）");
    if (page == 13) SetWindowTextW(app.valueLabel, L"落下を続ける時間");
    if (page == 14) SetWindowTextW(app.valueLabel, L"落下する距離");
    if (page == 15) SetWindowTextW(app.valueLabel, L"落下を続ける時間");
    if (page == 16) SetWindowTextW(app.valueLabel, L"落下する距離");
    if (page == 17) SetWindowTextW(app.valueLabel, L"天体表面からの高度（表面は0）");
    if (page == 18) SetWindowTextW(app.valueLabel, L"放出・使用するエネルギー");
    if (page == 19) SetWindowTextW(app.valueLabel, L"蓄える・使用するエネルギー");
    if (page == 20) SetWindowTextW(app.valueLabel, L"一定の電力・出力");
    if (page == 21) SetWindowTextW(app.valueLabel, L"比較したい面積");
    if (page == 22) SetWindowTextW(app.valueLabel, L"円形の範囲の半径（直径ではありません）");
    if (page == 23) SetWindowTextW(app.valueLabel, L"長方形の範囲：横の長さ");
    SetWindowTextW(app.value, app.values[page].c_str());
    SendMessageW(app.unit, CB_RESETCONTENT, 0, 0);
    ListView_DeleteAllItems(app.results);
    const auto units = chu2::units(firstCategory(page));
    for (size_t index = 0; index < units.size(); ++index)
    {
        if (page >= 6 && units[index].usesSound) continue;
        const LRESULT itemIndex = SendMessageW(app.unit, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(units[index].name.data()));
        SendMessageW(app.unit, CB_SETITEMDATA, itemIndex, static_cast<LPARAM>(index));
        LVITEMW item{};
        item.mask = LVIF_TEXT;
        item.iItem = static_cast<int>(index);
        item.pszText = const_cast<wchar_t*>(units[index].name.data());
        ListView_InsertItem(app.results, &item);
    }
    SendMessageW(app.unit, CB_SETCURSEL, std::max(0, app.selectedUnits[page]), 0);
    for (HWND control : {app.secondLabel, app.secondValue, app.secondUnit, app.assumption})
        ShowWindow(control, page >= 3 ? SW_SHOW : SW_HIDE);
    if (page == 21 || page == 22)
        for (HWND control : {app.secondLabel, app.secondValue, app.secondUnit}) ShowWindow(control, SW_HIDE);
    if (page >= 3)
    {
        SetWindowTextW(app.secondLabel, page == 8 ? L"衝突直前の速さ" : page == 9 ? L"1回転する時間（周期）" : page == 10 ? L"水平からの打ち上げ角度（真上は90度）" : isOrbit(page) ? L"天体半径" : page == 5 ? L"衝突直前の速さ（停止する相手・地面を基準）" : page == 3 ? L"加速を続ける時間" : L"移動距離");
        if (page == 11) SetWindowTextW(app.secondLabel, L"目標の人工重力");
        if (page == 12) SetWindowTextW(app.secondLabel, L"踏み切り距離（加速中に重心が上昇する距離）");
        if (page == 13 || page == 14) SetWindowTextW(app.secondLabel, L"重力加速度（地球：9.80665、月の例：約1.62 m/s²）");
        if (isDragFall(page)) SetWindowTextW(app.secondLabel, L"落下する物体の質量");
        if (page == 18) SetWindowTextW(app.secondLabel, L"エネルギーを放出・使用する時間");
        if (page == 19) SetWindowTextW(app.secondLabel, L"充電・供給する電力");
        if (page == 20) SetWindowTextW(app.secondLabel, L"供給・動作を続ける時間");
        if (page == 23) SetWindowTextW(app.secondLabel, L"長方形の範囲：縦の長さ");
        SetWindowTextW(app.secondValue, app.secondValues[page - 3].c_str());
        SendMessageW(app.secondUnit, CB_RESETCONTENT, 0, 0);
        const auto secondUnits = chu2::units(secondCategory(page));
        for (size_t index = 0; index < secondUnits.size(); ++index)
        {
            if (page >= 6 && secondUnits[index].usesSound) continue;
            const LRESULT itemIndex = SendMessageW(app.secondUnit, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(secondUnits[index].name.data()));
            SendMessageW(app.secondUnit, CB_SETITEMDATA, itemIndex, static_cast<LPARAM>(index));
        }
        SendMessageW(app.secondUnit, CB_SETCURSEL, std::max(0, app.secondSelectedUnits[page - 3]), 0);
    }
    for (HWND control : {app.thirdLabel, app.thirdValue, app.thirdUnit}) ShowWindow(control, hasThirdInput(page) ? SW_SHOW : SW_HIDE);
    if (hasThirdInput(page))
    {
        SetWindowTextW(app.thirdLabel, page == 8 ? L"停止するまでの距離（変形・沈み込みを含む）" : L"重力加速度（地球：9.80665、月の例：約1.62 m/s²）");
        SetWindowTextW(app.thirdValue, app.thirdValues[page - 8].c_str());
        SendMessageW(app.thirdUnit, CB_RESETCONTENT, 0, 0);
        const auto thirdUnits = chu2::units(thirdCategory(page));
        for (size_t index = 0; index < thirdUnits.size(); ++index)
        {
            if (thirdUnits[index].usesSound) continue;
            const auto itemIndex = SendMessageW(app.thirdUnit, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(thirdUnits[index].name.data()));
            SendMessageW(app.thirdUnit, CB_SETITEMDATA, itemIndex, static_cast<LPARAM>(index));
        }
        SendMessageW(app.thirdUnit, CB_SETCURSEL, std::max(0, app.thirdSelectedUnits[page - 8]), 0);
    }
    for (HWND control : {app.muLabel, app.mu, app.presetLabel, app.preset}) ShowWindow(control, isOrbit(page) ? SW_SHOW : SW_HIDE);
    for (HWND control : {app.areaReferenceLabel, app.areaReference}) ShowWindow(control, isArea(page) ? SW_SHOW : SW_HIDE);
    for (int index = 0; index < 3; ++index)
    {
        ShowWindow(app.airLabels[index], isDragFall(page) ? SW_SHOW : SW_HIDE);
        ShowWindow(app.airInputs[index], isDragFall(page) ? SW_SHOW : SW_HIDE);
    }
    for (HWND control : {app.soundLabel, app.sound}) ShowWindow(control, page >= 6 ? SW_HIDE : SW_SHOW);
    SetWindowTextW(app.assumption, page == 8 ? L"静止した相手に衝突して停止するモデル。平均の力 = 運動エネルギー / 停止距離。\n減速Gと停止時間は一定減速を仮定。最大の衝撃力や損傷は求めません。" : page == 9 ? L"遠心加速度 a = 4π²r/T²。Gは標準重力との比です。\n回転中心から指定半径の位置における人工重力の目安です。" : page == 10 ? L"空気抵抗なし・一定重力。出発点と同じ高さに着地するモデル。\n高さは出発点からの増分。崖からの投射や地形は考慮しません。" : isOrbit(page) ?
        L"条件：球対称天体の円軌道・空気抵抗なし。高度0は表面での理論値。\nμはGM。地球以外は半径とμを変更してください（地球のμが初期値）。" : page == 5 ?
        L"E = mv²/2（非相対論）／ TNT 1 g = 4,184 J\n衝突前のエネルギーです。衝撃力は停止距離や時間によって変わります。" :
        L"条件：初速0・一定加速度・空気抵抗なし（非相対論）");
    EnableWindow(app.sound, page != 2);
    if (page == 11) SetWindowTextW(app.assumption, L"回転半径と目標重力から1回転に必要な時間を計算します。\nGは標準重力9.80665 m/s²との比です。");
    if (page == 12) SetWindowTextW(app.assumption, L"静止から一定加速度で踏み切り、離陸後は重力のみで上昇するモデル。\n目標高さは離陸位置から。正味の加速度と支持力由来の体感Gを区別します。");
    if (page == 13 || page == 14) SetWindowTextW(app.assumption, L"初速0・空気抵抗なし・一定重力。下向きの速度と距離を正で表示します。\n地面への衝突は考慮しません。このモデルには終端速度がありません。");
    if (isDragFall(page)) SetWindowTextW(app.assumption, L"初速0・二乗抵抗 F = ρCdAv²/2。密度・Cd・面積・重力は一定。\n地面への衝突や姿勢変化は考慮しません。密度などを0にすると抵抗なし。");
    if (page >= 18 && page <= 20) SetWindowTextW(app.assumption, L"エネルギー E = 電力 P × 時間 t。1 W = 1 J/s、1 kWh = 3600000 J。\n損失なし（効率100%）。充電は一定電力、放出は時間内の平均出力です。");
    if (isArea(page)) SetWindowTextW(app.assumption, L"東京ドームは建築面積46755 m²。畳は関東（江戸間）・関西（京間）を選択。\n面積の比率です。実際に配置できる個数・枚数を求めるものではありません。");
    updateOrbitNote(app);
    app.changing = false;
    layout(app);
    calculate(app);
}
void copyResults(App& app)
{
    std::wstring text;
    const int selected = ListView_GetNextItem(app.results, -1, LVNI_SELECTED);
    for (size_t index = 0; index < app.resultRows.size(); ++index)
    {
        if (selected >= 0 && static_cast<int>(index) != selected) continue;
        text += app.resultRows[index].first + L"\t" + app.resultRows[index].second + L"\r\n";
    }
    HGLOBAL memory = GlobalAlloc(GMEM_MOVEABLE, (text.size() + 1) * sizeof(wchar_t));
    if (!memory) return;
    void* data = GlobalLock(memory);
    if (!data) { GlobalFree(memory); return; }
    memcpy(data, text.c_str(), (text.size() + 1) * sizeof(wchar_t));
    GlobalUnlock(memory);
    bool copied = false;
    if (OpenClipboard(app.window))
    {
        if (EmptyClipboard()) copied = SetClipboardData(CF_UNICODETEXT, memory) != nullptr;
        CloseClipboard();
    }
    if (!copied) GlobalFree(memory);
    SetWindowTextW(app.status, copied ? L"クリップボードにコピーしました。" : L"コピーできませんでした。再度お試しください。");
}
void saveSettings(App& app)
{
    const auto path = app.settings.c_str();
    const std::wstring size = std::to_wstring(app.fontSpec.lfHeight);
    if (!WritePrivateProfileStringW(L"Display", L"Font", app.fontSpec.lfFaceName, path)
        || !WritePrivateProfileStringW(L"Display", L"Size", size.c_str(), path)
        || !WritePrivateProfileStringW(L"Display", L"DarkMode", app.dark ? L"1" : L"0", path)
        || !WritePrivateProfileStringW(L"Physics", L"SoundSpeed", readText(app.sound).c_str(), path))
    {
        const std::wstring message = L"設定を保存できませんでした。\n" + app.settings.wstring() + L"\nフォルダーへの書き込み権限を確認してください。";
        MessageBoxW(app.window, message.c_str(), L"Chu2Calc", MB_ICONWARNING);
    }
}
void createControls(App& app)
{
    auto add = [&](const wchar_t* type, const wchar_t* text, DWORD style, int id = 0)
    {
        HWND control = CreateWindowExW(0, type, text, WS_CHILD | WS_VISIBLE | style,
            0, 0, 0, 0, app.window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), GetModuleHandleW(nullptr), nullptr);
        app.controls.push_back(control);
        return control;
    };
    app.navigation = add(L"LISTBOX", L"", WS_TABSTOP | LBS_NOTIFY | WS_BORDER, CATEGORY);
    for (int page : NAV_ORDER)
    {
        const auto index = SendMessageW(app.navigation, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(PAGE_NAMES[page]));
        SendMessageW(app.navigation, LB_SETITEMDATA, index, page);
    }
    SendMessageW(app.navigation, LB_SETCURSEL, 0, 0);
    app.heading = add(L"STATIC", L"速度変換", 0);
    app.valueLabel = add(L"STATIC", L"値を入力し、単位を選択", 0);
    app.value = add(L"EDIT", L"1", WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, VALUE);
    app.unit = add(L"COMBOBOX", L"", WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL, UNIT);
    app.soundLabel = add(L"STATIC", L"基準音速 (m/s) ／ マッハ・音秒", 0);
    app.sound = add(L"EDIT", L"343", WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, SOUND);
    app.secondLabel = add(L"STATIC", L"", 0);
    app.secondValue = add(L"EDIT", L"10", WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, SECOND_VALUE);
    app.secondUnit = add(L"COMBOBOX", L"", WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL, SECOND_UNIT);
    app.areaReferenceLabel = add(L"STATIC", L"比較対象", 0);
    app.areaReference = add(L"COMBOBOX", L"", WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL, AREA_REFERENCE);
    for (const auto& reference : chu2::areaReferences())
        SendMessageW(app.areaReference, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(reference.name.data()));
    SendMessageW(app.areaReference, CB_SETCURSEL, 0, 0);
    app.thirdLabel = add(L"STATIC", L"", 0);
    app.thirdValue = add(L"EDIT", L"", WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, THIRD_VALUE);
    app.thirdUnit = add(L"COMBOBOX", L"", WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL, THIRD_UNIT);
    const wchar_t* airNames[] = {L"空気密度 ρ (kg/m³)", L"抵抗係数 Cd（無次元）", L"進行方向の断面積 A (m²)"};
    const wchar_t* airDefaults[] = {L"1.225", L"0.47", L"0.01"};
    for (int index = 0; index < 3; ++index)
    {
        app.airLabels[index] = add(L"STATIC", airNames[index], 0);
        app.airInputs[index] = add(L"EDIT", airDefaults[index], WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, AIR_DENSITY + index);
    }
    app.muLabel = add(L"STATIC", L"重力パラメータ μ = GM (m³/s²)", 0);
    app.mu = add(L"EDIT", L"398600441800000", WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, MU);
    app.presetLabel = add(L"STATIC", L"天体プリセット", 0);
    app.preset = add(L"COMBOBOX", L"", WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL, BODY_PRESET);
    SendMessageW(app.preset, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"カスタム（手入力）"));
    for (const auto& body : chu2::bodyPresets())
        SendMessageW(app.preset, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(body.name.data()));
    SendMessageW(app.preset, CB_SETCURSEL, 1, 0);
    app.assumption = add(L"STATIC", L"条件：初速0・一定加速度・空気抵抗なし（非相対論）", 0);
    app.results = add(WC_LISTVIEWW, L"", WS_TABSTOP | WS_BORDER | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
    SetWindowSubclass(ListView_GetHeader(app.results), headerProc, 1, reinterpret_cast<DWORD_PTR>(&app));
    ListView_SetExtendedListViewStyle(app.results, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES);
    LVCOLUMNW column{};
    column.mask = LVCF_TEXT | LVCF_WIDTH;
    column.cx = 180;
    column.pszText = const_cast<wchar_t*>(L"単位");
    ListView_InsertColumn(app.results, 0, &column);
    column.pszText = const_cast<wchar_t*>(L"換算値");
    ListView_InsertColumn(app.results, 1, &column);
    app.status = add(L"STATIC", L"", 0);
    app.copy = add(L"BUTTON", L"選択行をコピー（未選択は全件）", WS_TABSTOP | BS_OWNERDRAW, COPY);
    app.tipsLabel = add(L"STATIC", L"Tips（スクロール・選択してコピーできます）", 0);
    app.tips = add(L"EDIT", L"", WS_TABSTOP | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL);
    SendMessageW(app.tips, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(app.px(8), app.px(8)));
    app.fontButton = add(L"BUTTON", L"フォント設定…", WS_TABSTOP | BS_OWNERDRAW, FONT);
    app.themeButton = add(L"BUTTON", L"", WS_TABSTOP | BS_OWNERDRAW, THEME);
    wchar_t path[32768]{};
    GetModuleFileNameW(nullptr, path, static_cast<DWORD>(std::size(path)));
    app.settings = std::filesystem::path(path).parent_path() / L"Chu2Calc.ini";
    app.dark = GetPrivateProfileIntW(L"Display", L"DarkMode", 1, app.settings.c_str()) != 0;
    GetPrivateProfileStringW(L"Display", L"Font", L"Yu Gothic UI", app.fontSpec.lfFaceName, LF_FACESIZE, app.settings.c_str());
    app.fontSpec.lfHeight = std::clamp(GetPrivateProfileIntW(L"Display", L"Size", 10, app.settings.c_str()), 8U, 24U);
    app.fontSpec.lfCharSet = DEFAULT_CHARSET;
    wchar_t sound[128]{};
    GetPrivateProfileStringW(L"Physics", L"SoundSpeed", L"343", sound, 128, app.settings.c_str());
    double parsed{};
    SetWindowTextW(app.sound, chu2::parseNumber(sound, parsed) && parsed > 0 ? sound : L"343");
    applyFont(app);
    applyTheme(app);
    changePage(app, 0);
    layout(app);
}
LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto* app = reinterpret_cast<App*>(GetWindowLongPtrW(window, GWLP_USERDATA));
    if (message == WM_NCCREATE)
    {
        app = static_cast<App*>(reinterpret_cast<CREATESTRUCTW*>(lParam)->lpCreateParams);
        app->window = window;
        SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
    }
    if (!app) return DefWindowProcW(window, message, wParam, lParam);
    switch (message)
    {
    case WM_ERASEBKGND:
    {
        RECT rect{};
        GetClientRect(window, &rect);
        if (app->background) FillRect(reinterpret_cast<HDC>(wParam), &rect, app->background);
        return 1;
    }
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    {
        const HDC dc = reinterpret_cast<HDC>(wParam);
        const bool surface = message != WM_CTLCOLORSTATIC || reinterpret_cast<HWND>(lParam) == app->unit || reinterpret_cast<HWND>(lParam) == app->tips;
        SetTextColor(dc, app->textColor());
        SetBkColor(dc, surface ? app->surfaceColor() : app->backgroundColor());
        return reinterpret_cast<LRESULT>(surface ? app->surface : app->background);
    }
    case WM_DRAWITEM:
    {
        const auto& item = *reinterpret_cast<DRAWITEMSTRUCT*>(lParam);
        if (item.CtlType != ODT_BUTTON) break;
        FillRect(item.hDC, &item.rcItem, app->surface);
        SetTextColor(item.hDC, app->textColor());
        SetBkMode(item.hDC, TRANSPARENT);
        const auto text = readText(item.hwndItem);
        RECT rect = item.rcItem;
        FrameRect(item.hDC, &rect, GetSysColorBrush((item.itemState & ODS_SELECTED) ? COLOR_HIGHLIGHT : COLOR_GRAYTEXT));
        DrawTextW(item.hDC, text.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
        if (item.itemState & ODS_FOCUS)
        {
            InflateRect(&rect, -3, -3);
            DrawFocusRect(item.hDC, &rect);
        }
        return TRUE;
    }
    case WM_CREATE:
        app->dpi = GetDpiForWindow(window);
        createControls(*app);
        return 0;
    case WM_SIZE: layout(*app); return 0;
    case WM_GETMINMAXINFO:
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize = {app->px(800), app->px(800)};
        return 0;
    case WM_DPICHANGED:
    {
        app->dpi = HIWORD(wParam);
        const RECT& rect = *reinterpret_cast<RECT*>(lParam);
        SetWindowPos(window, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOACTIVATE);
        applyFont(*app);
        layout(*app);
        return 0;
    }
    case WM_COMMAND:
        if (!app->changing && isOrbit(app->page) &&
            (((LOWORD(wParam) == SECOND_VALUE || LOWORD(wParam) == MU) && HIWORD(wParam) == EN_CHANGE)
             || (LOWORD(wParam) == SECOND_UNIT && HIWORD(wParam) == CBN_SELCHANGE)))
        {
            SendMessageW(app->preset, CB_SETCURSEL, 0, 0);
            updateOrbitNote(*app);
        }
        if (LOWORD(wParam) == CATEGORY && HIWORD(wParam) == LBN_SELCHANGE)
        {
            const auto index = SendMessageW(app->navigation, LB_GETCURSEL, 0, 0);
            const auto page = SendMessageW(app->navigation, LB_GETITEMDATA, index, 0);
            if (page >= 0 && page < static_cast<LRESULT>(std::size(PAGE_NAMES))) changePage(*app, static_cast<int>(page));
        }
        else if ((LOWORD(wParam) == VALUE || LOWORD(wParam) == SOUND || LOWORD(wParam) == SECOND_VALUE || LOWORD(wParam) == THIRD_VALUE || LOWORD(wParam) == MU || (LOWORD(wParam) >= AIR_DENSITY && LOWORD(wParam) <= DRAG_AREA)) && HIWORD(wParam) == EN_CHANGE) calculate(*app);
        else if ((LOWORD(wParam) == UNIT || LOWORD(wParam) == SECOND_UNIT || LOWORD(wParam) == THIRD_UNIT) && HIWORD(wParam) == CBN_SELCHANGE) calculate(*app);
        else if (LOWORD(wParam) == BODY_PRESET && HIWORD(wParam) == CBN_SELCHANGE) applyBodyPreset(*app);
        else if (LOWORD(wParam) == AREA_REFERENCE && HIWORD(wParam) == CBN_SELCHANGE) calculate(*app);
        else if (LOWORD(wParam) == COPY) copyResults(*app);
        else if (LOWORD(wParam) == THEME)
        {
            app->dark = !app->dark;
            applyTheme(*app);
        }
        else if (LOWORD(wParam) == FONT)
        {
            LOGFONTW spec = app->fontSpec;
            spec.lfHeight = -MulDiv(spec.lfHeight, static_cast<int>(app->dpi), 72);
            CHOOSEFONTW choice{sizeof(CHOOSEFONTW)};
            choice.hwndOwner = window;
            choice.lpLogFont = &spec;
            choice.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_LIMITSIZE;
            choice.nSizeMin = 8;
            choice.nSizeMax = 24;
            if (ChooseFontW(&choice))
            {
                app->fontSpec = spec;
                app->fontSpec.lfHeight = choice.iPointSize / 10;
                applyFont(*app);
                layout(*app);
            }
        }
        return 0;
    case WM_CLOSE: saveSettings(*app); DestroyWindow(window); return 0;
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(window, message, wParam, lParam);
}
}
int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show)
{
    INITCOMMONCONTROLSEX controls{sizeof(controls), ICC_LISTVIEW_CLASSES};
    InitCommonControlsEx(&controls);
    WNDCLASSW windowClass{};
    windowClass.lpfnWndProc = windowProc;
    windowClass.hInstance = instance;
    windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    windowClass.hIcon = LoadIconW(instance, MAKEINTRESOURCEW(IDI_CHU2CALC));
    windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    windowClass.lpszClassName = L"Chu2Calc.Main";
    if (!RegisterClassW(&windowClass)) return 1;
    App app;
    HWND window = CreateWindowExW(0, windowClass.lpszClassName, L"Chu2Calc 【 ✟厨二病電卓✟ 】", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1000, 920, nullptr, nullptr, instance, &app);
    if (!window) return 1;
    const UINT iconDpi = GetDpiForWindow(window);
    for (const auto type : {ICON_SMALL, ICON_BIG})
    {
        const int size = GetSystemMetricsForDpi(type == ICON_SMALL ? SM_CXSMICON : SM_CXICON, iconDpi);
        HICON icon = static_cast<HICON>(LoadImageW(instance, MAKEINTRESOURCEW(IDI_CHU2CALC), IMAGE_ICON, size, size, LR_SHARED));
        if (icon) SendMessageW(window, WM_SETICON, type, reinterpret_cast<LPARAM>(icon));
    }
    ShowWindow(window, show);
    MSG message{};
    int result;
    while ((result = static_cast<int>(GetMessageW(&message, nullptr, 0, 0))) > 0)
    {
        if (!IsDialogMessageW(window, &message))
        {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }
    if (app.font) DeleteObject(app.font);
    if (app.background) DeleteObject(app.background);
    if (app.surface) DeleteObject(app.surface);
    return result < 0 ? 1 : static_cast<int>(message.wParam);
}
