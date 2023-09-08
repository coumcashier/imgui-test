#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "colors.h"
#include "style.h"
#include "imgui_internal.h"
#include "imgui.h"

namespace ImGui {
    bool CheckboxEx(const char* label, bool* v);
    bool MyButtonStyled(const char *label, const ImVec2 &size_arg, ImGuiButtonFlags flags, ImVec4 active_border_col);
    bool MyButtonEx(const char *label, const ImVec2 &size_arg, ImGuiButtonFlags flags);
}
