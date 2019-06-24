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

        // Spindle
        if (ImGui::Button("-10%##Spindle")) {} ImGui::SameLine();
        if (ImGui::Button("-1%##Spindle"))  {} ImGui::SameLine();
        if (ImGui::Button("100%##Spindle")) {} ImGui::SameLine();
        if (ImGui::Button("+1%##Spindle"))  {} ImGui::SameLine();
        if (ImGui::Button("+10%##Spindle")) {}
        // Feed
        if (ImGui::Button("-10%##Feed")) {} ImGui::SameLine();
        if (ImGui::Button("-1%##Feed"))  {} ImGui::SameLine();
        if (ImGui::Button("100%##Feed")) {} ImGui::SameLine();
        if (ImGui::Button("+1%##Feed"))  {} ImGui::SameLine();
        if (ImGui::Button("+10%##Feed")) {}

        // Rapid
        if (ImGui::Button("25%##Rapid"))  {} ImGui::SameLine();
        if (ImGui::Button("50%##Rapid"))  {} ImGui::SameLine();
        if (ImGui::Button("100%##Rapid")) {}

		ImGui::End();

    }
}

