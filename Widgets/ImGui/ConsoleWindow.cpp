#include "ConsoleWindow.h"
#include "../../AppState.h"

using std::stringstream;

namespace Coconut
{
    ConsoleWindow::ConsoleWindow(AppState* project)
        : ImGuiWidget(project, "Console"),
          mCommandTextColor(1.f,1.f,1.f,1.f),
          mResponseTextColor(.5f,.5f,.5f,1.f)
	{}

    ConsoleWindow::~ConsoleWindow ()
	{}

    void
    ConsoleWindow::Draw
    ()
    {
        ImGui::Begin(mName.c_str(), &mVisible);

        ImVec2 space = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("CommandHistory",ImVec2(-1,space.y - 30));
        for (ConsoleLine& line : mConsoleLineBuffer)
        {
            switch (line.Type)
            {
                case Command:
                    ImGui::PushStyleColor(ImGuiCol_Text, mCommandTextColor);
                    break;
                case Response:
                    ImGui::PushStyleColor(ImGuiCol_Text, mResponseTextColor);
                    break;
            }

            ImGui::Text("%s", line.Data.c_str());
            ImGui::PopStyleColor();
            ImGui::SetScrollHere(1.0f);
        }

        ImGui::EndChild();
        ImGui::InputText("##MdiCommand", &mMdiBuffer[0], MDI_BUFFER_SIZE);
        ImGui::SameLine();
        ImGui::Button("Send");
		ImGui::End();
    }

    void ConsoleWindow::PushConsoleLine(const ConsoleLine& c)
    {
        if (!c.Data.empty()) mConsoleLineBuffer.push_back(c);
    }
}

