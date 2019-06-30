#include "StateWindow.h"
#include "../../AppState.h"

using std::stringstream;

namespace Coconut
{
    StateWindow::StateWindow(AppState* project)
		: ImGuiWidget(project, "Machine State")
	{

    }

    StateWindow::~StateWindow ()
	{}

    void
    StateWindow::Draw
    ()
    {
        GrblMachineModel& grbl = mAppState->GetGrblMachineModel();
        ImGui::Begin(mName.c_str(), &mVisible);

        ImGui::Columns(4);

        // Headers
        ImGui::Text("");  ImGui::NextColumn();
        ImGui::Text("X"); ImGui::NextColumn();
        ImGui::Text("Y"); ImGui::NextColumn();
        ImGui::Text("Z"); ImGui::NextColumn();
        ImGui::Separator();

        // WPos
        vec3 wpos = grbl.GetWorkPosition();
        ImGui::Text("WPos"); ImGui::NextColumn();
        ImGui::Text("%.3f",wpos.x);   ImGui::NextColumn();
        ImGui::Text("%.3f",wpos.y);   ImGui::NextColumn();
        ImGui::Text("%.3f",wpos.z);   ImGui::NextColumn();
        ImGui::Separator();

        // MPos
        vec3 mpos = grbl.GetMachinePosition();
		ImGui::Text("MPos"); ImGui::NextColumn();
        ImGui::Text("%.3f",mpos.x);   ImGui::NextColumn();
        ImGui::Text("%.3f",mpos.y);   ImGui::NextColumn();
        ImGui::Text("%.3f",mpos.z);   ImGui::NextColumn();
        ImGui::Separator();

        ImGui::Columns(2);

        // Status
        ImGui::Text("Status"); ImGui::NextColumn();
        ImGui::Text("%s",grbl.GetStateAsString().c_str());     ImGui::NextColumn();
        ImGui::Separator();

        // Tool
        ImGui::Text("Tool"); ImGui::NextColumn();
        ImGui::Text("%d",grbl.GetToolNumber());   ImGui::NextColumn();
        ImGui::Separator();

        // Feed
        ImGui::Text("Feed"); ImGui::NextColumn();
        ImGui::Text("%.3f mm/min",grbl.GetFeedRate());   ImGui::NextColumn();
        ImGui::Separator();

        // Spindle
        ImGui::Text("Spindle"); ImGui::NextColumn();
        ImGui::Text("%d rpm",grbl.GetSpindleSpeed());      ImGui::NextColumn();
        ImGui::Separator();

        DrawBufferUsageBar();
        ImGui::Separator();

        DrawProgressBar();
        ImGui::Separator();

		ImGui::End();
    }

    void StateWindow::DrawProgressBar()
    {
        GrblMachineModel& grbl = mAppState->GetGrblMachineModel();
        ImGui::Text("Progress");
        ImGui::NextColumn();
    	ImGui::ProgressBar(grbl.GetPercentCompleted(),ImVec2(-1,16), "");
        ImGui::NextColumn();
    }

    void StateWindow::DrawBufferUsageBar()
    {
        GrblMachineModel& grbl = mAppState->GetGrblMachineModel();
        ImGui::Text("Buffer");
        ImGui::NextColumn();
    	ImGui::ProgressBar(grbl.GetPercentBufferUsed(),ImVec2(-1,16), "");
        ImGui::NextColumn();
    }
}

