#include "ControlWindow.h"
#include "../../AppState.h"
#include <IconsFontAwesome5_c.h>

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
		GrblMachineModel& grbl = mAppState->GetGrblMachineModel();

        ImGui::Begin(mName.c_str(), &mVisible);

        ImVec2 buttonSize(-1, 20);

        ImGui::Columns(3,0,false);

        // Row 1
        if (ImGui::Button("Home", buttonSize))
        {
       		grbl.SendManualGCodeCommand(GCodeCommand::HomingCommand());
        }
        ImGui::NextColumn();

        if (ImGui::Button("Reset", buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::ResetCommand());
        }
        ImGui::NextColumn();

		if (ImGui::Button("Unlock", buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::UnlockCommand());
        }
        ImGui::NextColumn();

        // Row 2
        if (ImGui::Button("Z Probe", buttonSize))
        {
        }
        ImGui::NextColumn();

        if (ImGui::Button("Zero XY", buttonSize))
        {
       		grbl.SendManualGCodeCommand(GCodeCommand::ZeroXYCommand());
        }
        ImGui::NextColumn();

		if (ImGui::Button("Zero Z", buttonSize))
        {
       		grbl.SendManualGCodeCommand(GCodeCommand::ZeroZCommand());
        }
        ImGui::NextColumn();

        // Row 3
 		if (ImGui::Button("G28.1", buttonSize))
        {
       		grbl.SendManualGCodeCommand(GCodeCommand::SetSafePositionCommand());
        }
        ImGui::NextColumn();

        if (ImGui::Button("G28", buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::GoToSafePositionCommand());
        }
        ImGui::NextColumn();
        ImGui::NextColumn();




        ImGui::Columns(1);

		ImGui::End();
    }
}

