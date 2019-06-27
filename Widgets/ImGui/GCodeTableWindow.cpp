#include "GCodeTableWindow.h"
#include "../../AppState.h"

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
			for (int i=0; i<5; i++)
            {
                if(ImGui::TreeNodeEx((void*)(intptr_t)i,ImGuiTreeNodeFlags_Leaf,"..."))
                {
					ImGui::TreePop();
                }
            }
        }
        ImGui::EndChild();

    }

    void GCodeTableWindow::DrawTablePane()
    {
        ImVec2 space = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("GCodeTable",ImVec2(-1,space.y - 30));

		// Buttons

		if (ImGui::Button("Check Mode ($C)"))
		{

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

        ImGui::EndChild();
    }
}

