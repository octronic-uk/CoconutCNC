/**
  ImGuiWidget.h
*/

#pragma once

#include "../Widget.h"
#include "../../Deps/ImGui/imgui.h"
#include <vector>
#include <string>

using std::string;
using std::vector;

namespace Coconut
{
    class ImGuiWidget : public Widget
    {
    public:
        ImGuiWidget(AppState* project, string name, bool visible = true);
        ~ImGuiWidget() override;
        bool StringCombo(const char* label, int* current_item,
			const vector<string>& items, int height_in_items = -1);
    };
}
