#include "MenuBar.h"
#include "../../AppState.h"
#include "../../Common/Logger.h"
#include "../../Window.h"
#include <nfd.h>
#include "../../Model/Grbl/GrblMachineModel.h"

namespace Coconut
{
    MenuBar::MenuBar(AppState* state)
        : ImGuiWidget(state, "Menu Bar"),
          mFileSettingsClicked(false),
          mFileOpenClicked(false),
          mFileQuitClicked(false),
          mFileCloseClicked(false)
    {}

    MenuBar::~MenuBar()
    {

    }

    void MenuBar::Draw()
    {
        SerialPortModel& serial_model = mAppState->GetSerialPortModel();
        GrblMachineModel& grbl = mAppState->GetGrblMachineModel();
        ConnectionSettings& cs = mAppState->GetSettingsModel().GetConnectionSettings();

        if (ImGui::BeginMainMenuBar())
        {
            DrawFileMenu();
            DrawViewMenu();
            DrawWizardsMenu();
            DrawLoggingMenu();
            if (ImGui::Button(serial_model.IsPortOpen() ? "Disconnect":"Connect"))
            {
				if (serial_model.IsPortOpen())
                {
                    grbl.JoinWorkThread();
                    serial_model.CloseSerialPort();
                }
                else
                {
                    serial_model.SetTimeout(cs.GetTimeout());
                    serial_model.SetBaudRate(cs.GetSerialBaudRate());
                    serial_model.SetSerialPort(cs.GetSerialPort());
                	serial_model.OpenSerialPort();
                    grbl.StartWorkThread();
                }
            }
            ImGui::EndMainMenuBar();
        }

        HandleFileMenuActions();
        HandleWizardMenuActions();
    }

    void MenuBar::DrawFileMenu()
    {
        if (ImGui::BeginMenu("File"))
		{
			mFileOpenClicked = ImGui::MenuItem("Open...");
			mFileCloseClicked = ImGui::MenuItem("Close File");
            ImGui::Separator();
			mFileQuitClicked = ImGui::MenuItem("Quit");
			ImGui::EndMenu();
		}
    }



    void MenuBar::DrawViewMenu()
    {
        if (ImGui::BeginMenu("View"))
        {
			for (ImGuiWidget* widget : mAppState->GetWindow().GetImGuiWidgetsVector())
			{
                if (widget != this)
                {
					ImGui::Checkbox(widget->GetName().c_str(),widget->VisiblePointer());
                }
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

            if (ImGui::RadioButton("Info", currentLogLevel == spdlog::level::info))
			{
				currentLogLevel = spdlog::level::info;
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
        nfdchar_t *outPath = NULL;
		nfdresult_t result = NFD_OpenDialog("nc,gcode", NULL, &outPath );

		if ( result == NFD_OKAY )
        {
			debug("Success! {}",outPath);
			free(outPath);
		}
		else if ( result == NFD_CANCEL )
        {
			debug("User pressed cancel.");
		}
		else
        {
			error("Error: %s\n", NFD_GetError() );
		}
        mFileOpenClicked = false;
    }

    void MenuBar::FileCloseAction()
    {
        debug("MenuBar: CloseFileAction");
        mFileCloseClicked = false;
    }

    void MenuBar::FileSettingsAction()
    {
        mFileSettingsClicked = false;
    }

    void MenuBar::FileQuitAction()
    {
        mAppState->SetLooping(false);
        mFileQuitClicked = false;
    }

    void MenuBar::HandleFileMenuActions()
    {
 		// Handle File Menu Actions
		if (mFileOpenClicked)
		{
			FileOpenAction();
		}
        else if (mFileCloseClicked)
        {
			FileCloseAction();
        }
        else if (mFileSettingsClicked)
        {
            FileSettingsAction();
        }
        else if (mFileQuitClicked)
		{
            FileQuitAction();
		}
    }

    void MenuBar::HandleWizardMenuActions()
    {

    }


}
