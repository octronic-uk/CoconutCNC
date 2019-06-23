#include "ControlWindow.h"
#include "../../AppState.h"

using std::stringstream;

namespace Coconut
{
    ControlWindow::ControlWindow(AppState* project)
        : ImGuiWidget(project, "Control")
	{}

    ControlWindow::~ControlWindow ()
	{}

    void
    ControlWindow::Draw
    ()
    {
        ImGui::Begin(mName.c_str(), &mVisible);
		ImGui::End();

    }
}

