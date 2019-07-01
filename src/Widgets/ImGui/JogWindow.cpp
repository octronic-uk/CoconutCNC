#include "JogWindow.h"
#include "../../AppState.h"

using std::stringstream;

namespace Coconut
{
    JogWindow::JogWindow(AppState* project) : ImGuiWidget(project, "Jog"),
        mJogFeedRate(1000.f),
		mJogStepValue(10.f),
        mJogAbsoluteX(0.0f),
        mJogAbsoluteY(0.0f),
        mJogAbsoluteZ(0.0f)
	{}

    JogWindow::~JogWindow ()
	{}

    void
    JogWindow::Draw
    ()
    {
        GrblMachineModel& grbl = mAppState->GetGrblMachineModel();

        ImGui::Begin(mName.c_str(), &mVisible);


        ImVec2 buttonSize(-1, 20);

        // Row 1
        ImGui::Columns(4,0,false);

        if (ImGui::Button("-X/+Y", buttonSize))
        {
    		GCodeCommand jogCmd = GCodeCommand::JogCommand(-mJogStepValue,mJogStepValue,0,mJogFeedRate);
            grbl.SendManualGCodeCommand(jogCmd);
        }
        ImGui::NextColumn();

        if (ImGui::Button("+Y", buttonSize))
        {
    		GCodeCommand jogCmd = GCodeCommand::JogCommand(0,mJogStepValue,0,mJogFeedRate);
            grbl.SendManualGCodeCommand(jogCmd);
        }
        ImGui::NextColumn();

        if (ImGui::Button("+X/+Y", buttonSize))
        {
    		GCodeCommand jogCmd = GCodeCommand::JogCommand(mJogStepValue,mJogStepValue,0,mJogFeedRate);
            grbl.SendManualGCodeCommand(jogCmd);
        }
        ImGui::NextColumn();

        if (ImGui::Button("Z+", buttonSize))
        {
    		GCodeCommand jogCmd = GCodeCommand::JogCommand(0,0,mJogStepValue,mJogFeedRate);
            grbl.SendManualGCodeCommand(jogCmd);
        }
        ImGui::NextColumn();

        // Row 2
		if (ImGui::Button("-X", buttonSize))
        {
    		GCodeCommand jogCmd = GCodeCommand::JogCommand(-mJogStepValue,0,0,mJogFeedRate);
            grbl.SendManualGCodeCommand(jogCmd);
        }
        ImGui::NextColumn();


		if (ImGui::Button("Home XY", buttonSize))
        {
    		GCodeCommand jogCmd = GCodeCommand::GoToXYOriginCommand();
            grbl.SendManualGCodeCommand(jogCmd);
        }
        ImGui::NextColumn();

		if (ImGui::Button("+X", buttonSize))
        {
    		GCodeCommand jogCmd = GCodeCommand::JogCommand(+mJogStepValue,0,0,mJogFeedRate);
            grbl.SendManualGCodeCommand(jogCmd);
        }
        ImGui::NextColumn();

		if (ImGui::Button("Home Z", buttonSize))
        {
    		GCodeCommand jogCmd = GCodeCommand::GoToZOriginCommand();
            grbl.SendManualGCodeCommand(jogCmd);
        }
        ImGui::NextColumn();

		// Row 3
		if (ImGui::Button("-X/-Y", buttonSize))
        {
    		GCodeCommand jogCmd = GCodeCommand::JogCommand(-mJogStepValue,-mJogStepValue,0,mJogFeedRate);
            grbl.SendManualGCodeCommand(jogCmd);
        }
        ImGui::NextColumn();


		if (ImGui::Button("-Y", buttonSize))
        {
    		GCodeCommand jogCmd = GCodeCommand::JogCommand(0,-mJogStepValue,0,mJogFeedRate);
            grbl.SendManualGCodeCommand(jogCmd);
        }
        ImGui::NextColumn();


		if (ImGui::Button("+X/-Y", buttonSize))
        {
    		GCodeCommand jogCmd = GCodeCommand::JogCommand(mJogStepValue,-mJogStepValue,0,mJogFeedRate);
            grbl.SendManualGCodeCommand(jogCmd);
        }
        ImGui::NextColumn();


        if (ImGui::Button("-Z", buttonSize))
        {
    		GCodeCommand jogCmd = GCodeCommand::JogCommand(0,0,-mJogStepValue,mJogFeedRate);
            grbl.SendManualGCodeCommand(jogCmd);
        }

        ImGui::Columns(1);

        ImGui::Text("Step");


        ImGui::PushItemWidth(-1);
        if (ImGui::DragFloat("##Step", &mJogStepValue, 1.0f,0.0f,0.0f,"%.3f mm"))
        {
			if (mJogStepValue < 0.0f)
            {
                mJogStepValue = 0.01f;
            }
            if (mJogStepValue > 100.0f)
            {
               mJogStepValue = 100.0f;
            }
        }

        ImGui::Columns(5);

        if (ImGui::Button("0.01",buttonSize))
        {
            mJogStepValue = 0.01f;
        }
        ImGui::NextColumn();

        if (ImGui::Button("0.1",buttonSize))
        {
            mJogStepValue = 0.1f;
        }
        ImGui::NextColumn();

        if (ImGui::Button("1",buttonSize))
        {
            mJogStepValue = 1.f;
        }
        ImGui::NextColumn();

        if (ImGui::Button("10",buttonSize))
        {
            mJogStepValue = 10.f;
        }
        ImGui::NextColumn();

        if (ImGui::Button("100",buttonSize))
        {
            mJogStepValue = 100.f;
        }
        ImGui::NextColumn();

        ImGui::Columns(1);

        ImGui::Text("Feed Rate");

        ImGui::PushItemWidth(-1);

        ImGui::DragInt("##FeedRate",&mJogFeedRate,1.0f,0.0f,0.0f,"%d mm/min");

        ImGui::Text("Jog Absolute");

		if (ImGui::Button("GoTo X"))
        {

    	}
        ImGui::SameLine();
		ImGui::InputFloat("##JogAbsoluteX",&mJogAbsoluteX);

        if (ImGui::Button("GoTo Y"))
        {

    	}
        ImGui::SameLine();
        ImGui::InputFloat("##JogAbsoluteY",&mJogAbsoluteY);

        if (ImGui::Button("GoTo Z"))
        {

    	}
        ImGui::SameLine();
        ImGui::InputFloat("##JogAbsoluteZ",&mJogAbsoluteZ);

		ImGui::End();
    }
}

