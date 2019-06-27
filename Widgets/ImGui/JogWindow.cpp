#include "JogWindow.h"
#include "../../AppState.h"

using std::stringstream;

namespace Coconut
{
    JogWindow::JogWindow(AppState* project) : ImGuiWidget(project, "Jog"),
		mJogStepValue(10.f),
		mJogFeedRate(1000.0f),
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
        ImGui::Begin(mName.c_str(), &mVisible);

        ImVec2 space = ImGui::GetContentRegionAvail();
        ImVec2 buttonSize((space.x - ImGui::GetItemsLineHeightWithSpacing())/4.0f,
                          (space.y/3.0f - (ImGui::GetItemsLineHeightWithSpacing()/2.0f))/3.0f);

        // Row 1
        if (ImGui::Button("-X/+Y", buttonSize))
        {

        }
        ImGui::SameLine();

        if (ImGui::Button("+Y", buttonSize))
        {

        }
        ImGui::SameLine();

        if (ImGui::Button("+X/+Y", buttonSize))
        {

        }
        ImGui::SameLine();

        if (ImGui::Button("Z+", buttonSize))
        {

        }

        // Row 2
		if (ImGui::Button("-X", buttonSize))
        {

        }

        ImGui::SameLine();

		if (ImGui::Button("Home\nXY", buttonSize))
        {

        }

        ImGui::SameLine();

		if (ImGui::Button("+X", buttonSize))
        {

        }

        ImGui::SameLine();

		if (ImGui::Button("Home\nZ", buttonSize))
        {

        }

		// Row 3
		if (ImGui::Button("-X/-Y", buttonSize))
        {

        }

        ImGui::SameLine();

		if (ImGui::Button("-Y", buttonSize))
        {

        }

        ImGui::SameLine();

		if (ImGui::Button("+X/-Y", buttonSize))
        {

        }

        ImGui::SameLine();

        if (ImGui::Button("-Z", buttonSize))
        {

        }

        ImGui::Separator();

        ImGui::Text("Step");
        float stepButtonWidth = (space.x - ImGui::GetItemsLineHeightWithSpacing())/5.0f;

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

        if (ImGui::Button("0.01",ImVec2(stepButtonWidth,20)))
        {
            mJogStepValue = 0.01f;
        }
        ImGui::SameLine();

        if (ImGui::Button("0.1",ImVec2(stepButtonWidth,20)))
        {
            mJogStepValue = 0.1f;
        }
        ImGui::SameLine();

        if (ImGui::Button("1",ImVec2(stepButtonWidth,20)))
        {
            mJogStepValue = 1.f;
        }
        ImGui::SameLine();

        if (ImGui::Button("10",ImVec2(stepButtonWidth,20)))
        {
            mJogStepValue = 10.f;
        }
        ImGui::SameLine();

        if (ImGui::Button("100",ImVec2(stepButtonWidth,20)))
        {
            mJogStepValue = 100.f;
        }

        ImGui::Separator();

        ImGui::Text("Feed Rate");

        ImGui::PushItemWidth(-1);
        if(ImGui::DragFloat("##FeedRate",&mJogFeedRate,1.0f,0.0f,0.0f,"%.3f mm/min"))
        {

        }

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

