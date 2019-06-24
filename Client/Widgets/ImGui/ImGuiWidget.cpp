/**
  ImGuiWidget.cpp
*/

#include "ImGuiWidget.h"

namespace Coconut
{
    ImGuiWidget::ImGuiWidget
    (AppState* project, string name, bool visible)
        : Widget(project, name, visible)
    {
    }

    ImGuiWidget::~ImGuiWidget() {}

    bool ImGuiWidget::StringCombo(const char* label, int* current_item,
        const vector<string>& items, int height_in_items)
    {
        return ImGui::Combo
        (
            label,
            current_item,
            [](void* data, int idx, const char** out_text)
            {
                auto _data = ((const vector<string>*)data);
                *out_text = (*_data)[idx].c_str();
                return true;
            },
            (void*)&items,
            items.size(),
            height_in_items
            );
    }
}
