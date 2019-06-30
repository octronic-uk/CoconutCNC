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

        ImVec2 buttonSize(-1,20);

        // Spindle
        ImGui::Text("Spindle Override");

        ImGui::Columns(5);

        if (ImGui::Button("-10%##Spindle",buttonSize))
        {}
        ImGui::NextColumn();
        if (ImGui::Button("-1%##Spindle",buttonSize))
        {}
        ImGui::NextColumn();
        if (ImGui::Button("100%##Spindle",buttonSize))
        {}
        ImGui::NextColumn();
        if (ImGui::Button("+1%##Spindle",buttonSize))
        {}
        ImGui::NextColumn();
        if (ImGui::Button("+10%##Spindle",buttonSize))
        {}

        // Feed
        ImGui::Columns(1);
        ImGui::Text("Feed Rate Override");
        ImGui::Columns(5);

        if (ImGui::Button("-10%##Feed",buttonSize))
        {}
        ImGui::NextColumn();
        if (ImGui::Button("-1%##Feed",buttonSize))
        {}
        ImGui::NextColumn();
        if (ImGui::Button("100%##Feed",buttonSize))
        {}
        ImGui::NextColumn();
        if (ImGui::Button("+1%##Feed",buttonSize))
        {}
        ImGui::NextColumn();
        if (ImGui::Button("+10%##Feed",buttonSize))
        {}
        ImGui::NextColumn();


        // Rapid
        ImGui::Columns(1);
        ImGui::Text("Rapid Override");
        ImGui::Columns(3);

        if (ImGui::Button("25%##Rapid",buttonSize))
        {}
        ImGui::NextColumn();
        if (ImGui::Button("50%##Rapid",buttonSize))
        {}
        ImGui::NextColumn();
        if (ImGui::Button("100%##Rapid",buttonSize))
        {}
        ImGui::NextColumn();

        ImGui::Columns(1);

		ImGui::End();

    }
}

