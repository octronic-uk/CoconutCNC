#include "StateWindow.h"
#include "../../AppState.h"
#include "../../Common/Time.h"

using std::stringstream;

namespace Coconut
{
    StateWindow::StateWindow(AppState* project)
		: ImGuiWidget(project, "Machine State"),
        mLastFlashTime(0),
        mFlashInterval(500)
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

        ImVec4 state_text_color(1.f,1.f,1.f,1.f);

        if (Time::GetCurrentTime() > mLastFlashTime+mFlashInterval)
        {
        	mLastFlashTime = Time::GetCurrentTime();
            mFlash = !mFlash;
        }

		switch (grbl.GetState())
		{
		    case GrblMachineState::Jog:
            case GrblMachineState::Home:
            case GrblMachineState::Hold:
            case GrblMachineState::Queue:
				state_text_color = ImVec4(.9f,.6f,.25f,1.f);
			   break;
		   case GrblMachineState::Idle:
				state_text_color = ImVec4(0.f,1.f,0.f,1.f);
				break;
			case GrblMachineState::Alarm:
            case GrblMachineState::Door:
            case GrblMachineState::Check:
            case GrblMachineState::Locked:
				if (mFlash) state_text_color = ImVec4(1.f,0.f,0.f,1.f);
            case GrblMachineState::Error:
				if (mFlash) state_text_color = ImVec4(1.f,0.f,0.f,1.f);
				else state_text_color = ImVec4(0.f,1.f,0.f,1.f);
				break;
            case GrblMachineState::Run:
                state_text_color = ImVec4(0.f,1.f,0.f,1.f);
                break;
            case GrblMachineState::Unknown:
            case GrblMachineState::Unlocked:
                break;

		}

        ImGui::PushStyleColor(ImGuiCol_Text, state_text_color);

        ImGui::Text("%s",grbl.GetStateAsString().c_str());     ImGui::NextColumn();
        ImGui::PopStyleColor();

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
        ImGui::Text("%d RPM",grbl.GetSpindleSpeed());      ImGui::NextColumn();
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
    	ImGui::ProgressBar(grbl.GetPercentGrblBufferUsed(),ImVec2(-1,16), "");
        ImGui::NextColumn();
    }
}

