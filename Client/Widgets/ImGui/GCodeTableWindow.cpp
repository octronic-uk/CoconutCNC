#include "GCodeTableWindow.h"
#include "../../AppState.h"

namespace Coconut
{
    GCodeTableWindow::GCodeTableWindow(AppState* project)
        : ImGuiWidget(project, "GCodeTable"),
          mProgress(0.5f),
          mBufferUsage(0.5f)
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

        ImGui::Columns(1);

        ImGui::NextColumn();

        DrawBufferUsageBar();
        ImGui::SameLine();
        DrawProgressBar();

        ImGui::Columns(1);
		ImGui::End();
    }

    void GCodeTableWindow::DrawProgressBar()
    {
        ImGui::Text("Progress");
        ImGui::SameLine();
    	ImGui::ProgressBar(mProgress,ImVec2(200,20));
    }

    void GCodeTableWindow::DrawBufferUsageBar()
    {
        ImGui::Text("Buffer");
        ImGui::SameLine();
    	ImGui::ProgressBar(mBufferUsage,ImVec2(200,20));
    }

    void GCodeTableWindow::DrawMarkersPane()
    {
 		ImGui::Text("Markers");

        ImGui::PushItemWidth(-1);
		ImGui::BeginChild("MarkersChild",ImVec2(200,-1));
        {
			for (int i=0; i<5; i++) ImGui::Text("...");
        }
        ImGui::EndChild();

    }

    void GCodeTableWindow::DrawTablePane()
    {
 		ImGui::BeginChild("GCodeTable");

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

