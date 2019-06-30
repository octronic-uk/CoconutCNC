#include "GCodeTableWindow.h"
#include "../../AppState.h"
#include "../../Model/Grbl/GCodeFileModel.h"

namespace Coconut
{
    GCodeTableWindow::GCodeTableWindow(AppState* project)
        : ImGuiWidget(project, "GCodeTable")

	{}

    GCodeTableWindow::~GCodeTableWindow ()
	{}

    void
    GCodeTableWindow::Draw
    ()
    {
        ImGui::Begin(mName.c_str(), &mVisible);
		DrawTablePane();
		ImGui::End();
    }

    void GCodeTableWindow::DrawTablePane()
    {


        GrblMachineModel& grbl = mAppState->GetGrblMachineModel();
        // Buttons

		if (ImGui::Button("Check Mode ($C)"))
		{
			grbl.SendManualGCodeCommand(GCodeCommand::CheckModeCommand());
		}

		ImGui::SameLine();

		if (ImGui::Button("Send"))
		{
			grbl.SendProgram();
		}

		ImGui::Separator();


        ImGui::BeginChild("GCodeTable");
        {
			// Table
			ImGui::Columns(4);

			// Header

			ImGui::Text("Line");
			ImGui::NextColumn();

			ImGui::Text("Command");
			ImGui::NextColumn();

			ImGui::Text("Status");

			ImGui::NextColumn();

			ImGui::Text("Response");
			ImGui::NextColumn();

			ImGui::Separator();
			ImGui::Separator();

			// Data
			vector<GCodeCommand>& commands = mAppState->GetGCodeFileModel().GetData();
			for (GCodeCommand& cmd : commands)
			{
				ImGui::Text("%d",cmd.GetLine());
				ImGui::NextColumn();

				ImGui::Text("%s",cmd.GetCommand().c_str());
				ImGui::NextColumn();

				ImGui::Text("%s",cmd.GetStateString().c_str());
				ImGui::NextColumn();

				ImGui::Text("%s",cmd.GetResponse().GetData().c_str());
				ImGui::NextColumn();

				ImGui::Separator();
			}
        }
        ImGui::EndChild();
    }
}

