#include "ConsoleWindow.h"
#include "../../AppState.h"

using std::stringstream;

namespace Coconut
{
    ConsoleWindow::ConsoleWindow(AppState* project)
        : ImGuiWidget(project, "Console")
	{}

    ConsoleWindow::~ConsoleWindow ()
	{}

    void
    ConsoleWindow::Draw
    ()
    {
        ImGui::Begin(mName.c_str(), &mVisible);
		ImGui::End();
    }
}

