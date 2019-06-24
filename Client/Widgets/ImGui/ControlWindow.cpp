#include "ControlWindow.h"
#include "../../AppState.h"

using std::stringstream;

namespace Coconut
{
    ControlWindow::ControlWindow(AppState* project)
        : ImGuiWidget(project, "Control")
	{}

    ControlWindow::~ControlWindow ()
	{}

    void
    ControlWindow::Draw
    ()
    {
        ImGui::Begin(mName.c_str(), &mVisible);

        ImVec2 space = ImGui::GetContentRegionAvail();
        ImVec2 buttonSize(
    		(space.x -= ImGui::GetItemsLineHeightWithSpacing()) / 4,
    		space.y/4);

        // Row 1
        if (ImGui::Button("Home", buttonSize))
        {}
        ImGui::SameLine();
        if (ImGui::Button("Z-Probe", buttonSize))
        {}
        ImGui::SameLine();
		if (ImGui::Button("Zero XY", buttonSize))
        {}
        ImGui::SameLine();
		if (ImGui::Button("Zero Z", buttonSize))
        {}

        // Row 2
 		if (ImGui::Button("G28.1", buttonSize))
        {}
        ImGui::SameLine();
        if (ImGui::Button("G28", buttonSize))
        {}
        ImGui::SameLine();
		if (ImGui::Button("Reset", buttonSize))
        {}
        ImGui::SameLine();
		if (ImGui::Button("Clear Alarm", buttonSize))
        {}

        // Row 3
        if (ImGui::Button("User 1", buttonSize))
        {}
        ImGui::SameLine();
        if (ImGui::Button("User 2", buttonSize))
        {}
        ImGui::SameLine();
		if (ImGui::Button("User 3", buttonSize))
        {}
        ImGui::SameLine();
		if (ImGui::Button("User 4", buttonSize))
        {}

        // UI Unlock
        if (ImGui::Button("Unlock UI", ImVec2(space.x -= ImGui::GetItemsLineHeightWithSpacing(), -1)))//buttonSize.y)))
        {

        }

		ImGui::End();
    }
}

