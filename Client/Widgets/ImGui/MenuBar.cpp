#include "MenuBar.h"
#include "../../AppState.h"
#include "../../Logger.h"
#include "../../Window.h"
#include <imguifilesystem.h>

namespace Coconut
{
    MenuBar::MenuBar(AppState* state)
        : ImGuiWidget(state, "Menu Bar"),
        mOpenButtonClicked(false),
        mQuitButtonClicked(false),
        mCloseButtonClicked(false)
    {

    }

    MenuBar::~MenuBar()
    {

    }

    void MenuBar::Draw()
    {
        if (ImGui::BeginMainMenuBar())
        {
            DrawFileMenu();
            DrawViewMenu();
            DrawWizardsMenu();
            DrawLoggingMenu();
            ImGui::EndMainMenuBar();
        }

        // Handle File Menu Actions
		if (mOpenButtonClicked)
		{
			FileOpenAction();
		}
        else if (mCloseButtonClicked)
        {
			FileCloseAction();
        }
        else if (mQuitButtonClicked)
		{
            FileQuitAction();
		}
    }

    void MenuBar::DrawFileMenu()
    {
        if (ImGui::BeginMenu("File"))
		{
			mOpenButtonClicked = ImGui::MenuItem("Open...");
			mCloseButtonClicked = ImGui::MenuItem("Close File");
            ImGui::Separator();
			mQuitButtonClicked = ImGui::MenuItem("Quit");
			ImGui::EndMenu();
		}
    }

    void MenuBar::DrawViewMenu()
    {
        if (ImGui::BeginMenu("View"))
        {
			for (ImGuiWidget* widget : mAppState->GetWindow()->GetImGuiWidgetsVector())
			{
				ImGui::Checkbox(widget->GetName().c_str(),widget->VisiblePointer());
			}
            ImGui::EndMenu();
        }
    }

    void MenuBar::DrawWizardsMenu()
    {
 		if (ImGui::BeginMenu("Wizards"))
		{
			ImGui::EndMenu();
		}
    }

    void MenuBar::DrawLoggingMenu()
    {
        if(ImGui::BeginMenu("Logging"))
		{
            auto log = spdlog::get("");
			spdlog::level::level_enum currentLogLevel =  log->level();

			if (ImGui::RadioButton("Off", currentLogLevel == spdlog::level::off))
			{
				currentLogLevel = spdlog::level::off;
				spdlog::set_level(currentLogLevel);
			}

			if (ImGui::RadioButton("Error", currentLogLevel == spdlog::level::err))
			{
				currentLogLevel = spdlog::level::err;
				spdlog::set_level(currentLogLevel);
			}
			if (ImGui::RadioButton("Warning", currentLogLevel == spdlog::level::warn))
			{
				currentLogLevel = spdlog::level::warn;
				spdlog::set_level(currentLogLevel);
			}

			if (ImGui::RadioButton("Debug", currentLogLevel == spdlog::level::debug))
			{
				currentLogLevel = spdlog::level::debug;
				spdlog::set_level(currentLogLevel);
			}

			ImGui::EndMenu();
		}
    }

    void MenuBar::FileOpenAction()
    {
        debug("MenuBar: OpenFileAction");
        mCloseButtonClicked = false;
    }

    void MenuBar::FileCloseAction()
    {
        debug("MenuBar: CloseFileAction");
        mCloseButtonClicked = false;
    }

    void MenuBar::FileQuitAction()
    {
        mAppState->SetLooping(false);
        mQuitButtonClicked = false;
    }
}
