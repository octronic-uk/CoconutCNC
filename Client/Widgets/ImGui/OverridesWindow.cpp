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
        ImGui::Begin(mName.c_str(), &mVisible);
		ImGui::End();

    }
}

