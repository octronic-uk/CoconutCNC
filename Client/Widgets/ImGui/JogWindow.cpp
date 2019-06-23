#include "JogWindow.h"
#include "../../AppState.h"

using std::stringstream;

namespace Coconut
{
    JogWindow::JogWindow(AppState* project)
        : ImGuiWidget(project, "Jog")
	{}

    JogWindow::~JogWindow ()
	{}

    void
    JogWindow::Draw
    ()
    {
        ImGui::Begin(mName.c_str(), &mVisible);
		ImGui::End();

    }
}

