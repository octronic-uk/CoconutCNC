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
        ImGui::Columns(2);

        DrawMarkersPane();

        ImGui::NextColumn();

		DrawTablePane();

		ImGui::End();
    }



    void GCodeTableWindow::DrawMarkersPane()
    {
 		ImGui::Text("Markers");

        ImVec2 space = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("MarkersChild",ImVec2(-1,space.y - 30));
        {
            vector<GCodeCommand>& markers = mAppState->GetGCodeFileModel().GetMarkers();
            int i=0;
            for (GCodeCommand& marker : markers)
            {
                if(ImGui::TreeNodeEx((void*)(intptr_t)i,ImGuiTreeNodeFlags_Leaf,"%s",marker.GetCommand().c_str()))
                {
					ImGui::TreePop();
                }
                i++;
            }
        }
        ImGui::EndChild();

    }

    void GCodeTableWindow::DrawTablePane()
    {
        GrblMachineModel& grbl = mAppState->GetGrblMachineModel();
        ImVec2 space = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("GCodeTable",ImVec2(-1,space.y - 30));
        {

			// Buttons

			if (ImGui::Button("Check Mode ($C)"))
			{
				grbl.SendManualGCodeCommand(GCodeCommand::CheckModeCommand());
			}

			ImGui::SameLine();

			if (ImGui::Button("Auto Scroll"))
			{

			}

			ImGui::SameLine();

			if (ImGui::Button("Send"))
			{

			}

			ImGui::Separator();

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

