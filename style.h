#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <optional>
#include <iostream>
#include <string>
#include "imgui.h"
#include "imgui_internal.h"

 struct WidgetStyle {
    std::optional<ImVec4> hover_border;
    std::optional<ImVec4> active_border;
    std::optional<ImVec4> regular_border;
    std::optional<ImVec4> hover_bg;
    std::optional<ImVec4> active_bg;
    std::optional<ImVec4> regular_bg;
    std::optional<ImVec4> hover_color;
    std::optional<ImVec4> active_color;
    std::optional<ImVec4> regular_color;
};