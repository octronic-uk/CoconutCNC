#include "OverridesWindow.h"
#include "../../AppState.h"

using std::stringstream;

namespace Coconut
{
    OverridesWindow::OverridesWindow(AppState* project)
        : ImGuiWidget(project, "Overrides")
	{}

    OverridesWindow::~OverridesWindow ()
	{}

    void
    OverridesWindow::Draw
    ()
    {
        GrblMachineModel& grbl = mAppState->GetGrblMachineModel();

        ImGui::Begin(mName.c_str(), &mVisible);

        ImVec2 buttonSize(-1,20);

        // Spindle
        ImGui::Text("Spindle Override");

        ImGui::Columns(5);

        if (ImGui::Button("-10%##Spindle",buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::SpindleOvMinusTen());
        }
        ImGui::NextColumn();
        if (ImGui::Button("-1%##Spindle",buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::SpindleOvMinusTen());
        }
        ImGui::NextColumn();
        if (ImGui::Button("100%##Spindle",buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::SpindleOvDefault());
        }
        ImGui::NextColumn();
        if (ImGui::Button("+1%##Spindle",buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::SpindleOvPlusOne());
        }
        ImGui::NextColumn();
        if (ImGui::Button("+10%##Spindle",buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::SpindleOvPlusTen());
        }

        // Feed
        ImGui::Columns(1);
        ImGui::Text("Feed Rate Override");
        ImGui::Columns(5);

        if (ImGui::Button("-10%##Feed",buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::FeedOvMinusTen());
        }
        ImGui::NextColumn();
        if (ImGui::Button("-1%##Feed",buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::FeedOvMinusOne());
        }
        ImGui::NextColumn();
        if (ImGui::Button("100%##Feed",buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::FeedOvDefault());
        }
        ImGui::NextColumn();
        if (ImGui::Button("+1%##Feed",buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::FeedOvPlusOne());
        }
        ImGui::NextColumn();
        if (ImGui::Button("+10%##Feed",buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::FeedOvPlusTen());
        }
        ImGui::NextColumn();


        // Rapid
        ImGui::Columns(1);
        ImGui::Text("Rapid Override");
        ImGui::Columns(3);

        if (ImGui::Button("25%##Rapid",buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::RapidOv25Percent());
        }
        ImGui::NextColumn();
        if (ImGui::Button("50%##Rapid",buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::RapidOv50Percent());
        }
        ImGui::NextColumn();
        if (ImGui::Button("100%##Rapid",buttonSize))
        {
            grbl.SendManualGCodeCommand(GCodeCommand::RapidOvDefault());
        }
        ImGui::NextColumn();

        ImGui::Columns(1);

		ImGui::End();

    }
}

