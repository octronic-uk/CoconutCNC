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
            DrawLoggingMenu();
            DrawHowToMenu();

            ImVec2 menu_size = ImGui::GetContentRegionMax();
            ImVec2 buttonSize(100,20);
            ImVec2 buttonAt(menu_size.x-buttonSize.x,menu_size.y);
            ImGui::SameLine(buttonAt.x);

            if (ImGui::Button(serial_model.IsPortOpen() ? "Disconnect":"Connect",buttonSize))
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
                    serial_model.SetPortName(cs.GetSerialPort());
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
			mFileOpenClicked = ImGui::MenuItem("Open File...","CTRL+O");
			mFileCloseClicked = ImGui::MenuItem("Close File","CTRL+W");
            ImGui::Separator();
			mFileQuitClicked = ImGui::MenuItem("Quit","CTRL+Q");
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

    void MenuBar::DrawHowToMenu()
    {
 		if (ImGui::BeginMenu("How To"))
		{
            if (ImGui::MenuItem("Connect to GRBL"))
            {

            }

            if(ImGui::MenuItem("Configure Grbl Machine"))
            {

            }

            if (ImGui::MenuItem("Setup Tool or Tool Holder"))
            {

            }

            if (ImGui::MenuItem("Prepare Workspace"))
            {

            }

            if (ImGui::MenuItem("Run GCode"))
            {

            }

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
        nfdchar_t *selected_file_path = NULL;
		nfdresult_t result = NFD_OpenDialog("nc,gcode", NULL, &selected_file_path );

		if ( result == NFD_OKAY )
        {
			info("Success! {}",selected_file_path);
            GCodeFileModel& file_model = mAppState->GetGCodeFileModel();
            file_model.ClearState();
            file_model.Load(string(selected_file_path));
			free(selected_file_path);
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
        mAppState->GetGCodeFileModel().ClearState();
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
