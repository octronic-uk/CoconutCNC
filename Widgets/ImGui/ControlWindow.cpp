#include "ControlWindow.h"
#include "../../AppState.h"
#include "../../Deps/IconFontAwesome/IconsFontAwesome5_c.h"

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
        ImVec2 buttonSize((space.x - ImGui::GetItemsLineHeightWithSpacing())/4.0f,
                          (space.y - (ImGui::GetItemsLineHeightWithSpacing()/2.0f))/3.0f);

        // Row 1
        if (ImGui::Button("Home", buttonSize))
        {}
        ImGui::SameLine();
        if (ImGui::Button("Z\nProbe", buttonSize))
        {}
        ImGui::SameLine();
		if (ImGui::Button("Zero\nXY", buttonSize))
        {}
        ImGui::SameLine();
		if (ImGui::Button("Zero\nZ", buttonSize))
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
		if (ImGui::Button("Clear\nAlarm", buttonSize))
        {}

        // Row 3
        if (ImGui::Button("User\n1", buttonSize))
        {}
        ImGui::SameLine();
        if (ImGui::Button("User\n2", buttonSize))
        {}
        ImGui::SameLine();
		if (ImGui::Button("User\n3", buttonSize))
        {}
        ImGui::SameLine();
		if (ImGui::Button("User\n4", buttonSize))
        {}
		ImGui::End();
    }
}

