#include "OverridesWindow.h"
#include "../../AppState.h"

using std::stringstream;

namespace Coconut
{
    OverridesWindow::OverridesWindow(AppState* project)
        : ImGuiWidget(project, "Overrides")
	{}

    OverridesWindow::~OverridesWindow ()
	{}

    void
    OverridesWindow::Draw
    ()
    {
        ImGui::Begin(mName.c_str(), &mVisible);


 		ImVec2 space = ImGui::GetContentRegionAvail();
        float threeButtonWidth = (space.x - ImGui::GetItemsLineHeightWithSpacing())/3.0f;
        float fiveButtonWidth = (space.x - ImGui::GetItemsLineHeightWithSpacing())/5.0f;
        float buttonHeight = 20.0f;

        // Spindle
        ImGui::Text("Spindle Override");
        if (ImGui::Button("-10%##Spindle",ImVec2(fiveButtonWidth,buttonHeight)))
        {}
        ImGui::SameLine();
        if (ImGui::Button("-1%##Spindle",ImVec2(fiveButtonWidth,buttonHeight)))
        {}
        ImGui::SameLine();
        if (ImGui::Button("100%##Spindle",ImVec2(fiveButtonWidth,buttonHeight)))
        {}
        ImGui::SameLine();
        if (ImGui::Button("+1%##Spindle",ImVec2(fiveButtonWidth,buttonHeight)))
        {}
        ImGui::SameLine();
        if (ImGui::Button("+10%##Spindle",ImVec2(fiveButtonWidth,buttonHeight)))
        {}

        // Feed
        ImGui::Text("Feed Rate Override");
        if (ImGui::Button("-10%##Feed",ImVec2(fiveButtonWidth,buttonHeight)))
        {}
        ImGui::SameLine();
        if (ImGui::Button("-1%##Feed",ImVec2(fiveButtonWidth,buttonHeight)))
        {}
        ImGui::SameLine();
        if (ImGui::Button("100%##Feed",ImVec2(fiveButtonWidth,buttonHeight)))
        {}
        ImGui::SameLine();
        if (ImGui::Button("+1%##Feed",ImVec2(fiveButtonWidth,buttonHeight)))
        {}
        ImGui::SameLine();
        if (ImGui::Button("+10%##Feed",ImVec2(fiveButtonWidth,buttonHeight)))
        {}

        // Rapid
        ImGui::Text("Rapid Override");
        if (ImGui::Button("25%##Rapid",ImVec2(threeButtonWidth,buttonHeight)))
        {}
        ImGui::SameLine();
        if (ImGui::Button("50%##Rapid",ImVec2(threeButtonWidth,buttonHeight)))
        {}
        ImGui::SameLine();
        if (ImGui::Button("100%##Rapid",ImVec2(threeButtonWidth,buttonHeight)))
        {}

		ImGui::End();

    }
}

