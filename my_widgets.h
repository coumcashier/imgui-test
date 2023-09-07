#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "colors.h"
#include "imgui_internal.h"
#include "imgui.h"

namespace ImGui {
    bool CheckboxEx(const char* label, bool* v);
    bool MyButtonEx(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags);
}
