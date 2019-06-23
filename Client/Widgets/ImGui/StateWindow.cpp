#include "StateWindow.h"
#include "../../AppState.h"

using std::stringstream;

namespace Coconut
{
    StateWindow::StateWindow(AppState* project)
        : ImGuiWidget(project, "Machine State")
	{}

    StateWindow::~StateWindow ()
	{}

    void
    StateWindow::Draw
    ()
    {
        ImGui::Begin(mName.c_str(), &mVisible);
		ImGui::End();
    }
}

