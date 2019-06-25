#include "StateWindow.h"
#include "../../AppState.h"

using std::stringstream;

namespace Coconut
{
    StateWindow::StateWindow(AppState* project)
        : ImGuiWidget(project, "Machine State")
	{}

    StateWindow::~StateWindow ()
	{}

    void
    StateWindow::Draw
    ()
    {
        ImGui::Begin(mName.c_str(), &mVisible);

        ImGui::Columns(4);

        // Headers
        ImGui::Text("");  ImGui::NextColumn();
        ImGui::Text("X"); ImGui::NextColumn();
        ImGui::Text("Y"); ImGui::NextColumn();
        ImGui::Text("Z"); ImGui::NextColumn();
        ImGui::Separator();

        // WPos
        ImGui::Text("WPos:"); ImGui::NextColumn();
        ImGui::Text("...");   ImGui::NextColumn();
        ImGui::Text("...");   ImGui::NextColumn();
        ImGui::Text("...");   ImGui::NextColumn();
        ImGui::Separator();

        // MPos
		ImGui::Text("MPos:"); ImGui::NextColumn();
        ImGui::Text("...");   ImGui::NextColumn();
        ImGui::Text("...");   ImGui::NextColumn();
        ImGui::Text("...");   ImGui::NextColumn();
        ImGui::Separator();

        ImGui::Columns(2);

        // Status
        ImGui::Text("Status:"); ImGui::NextColumn();
        ImGui::Text("...");     ImGui::NextColumn();
        ImGui::Separator();

        // Tool
        ImGui::Text("Tool:"); ImGui::NextColumn();
        ImGui::Text("...");   ImGui::NextColumn();
        ImGui::Separator();

        // Feed
        ImGui::Text("Feed:"); ImGui::NextColumn();
        ImGui::Text("...");   ImGui::NextColumn();
        ImGui::Separator();

        // Spindle
        ImGui::Text("Spindle:"); ImGui::NextColumn();
        ImGui::Text("...");      ImGui::NextColumn();

		ImGui::End();
    }
}

