#include "SettingsWindow.h"
#include "../../AppState.h"

#include "../../Model/Grbl/GrblConfigurationModel.h"
#include "../../Model/Settings/ToolSettings.h"
#include "../../Model/Settings/ToolHolderSettings.h"
#include "../../Model/Settings/ConnectionSettings.h"

using std::pair;
using std::distance;
using std::find;

namespace Coconut
{
    SettingsWindow::SettingsWindow(AppState* project)
        : ImGuiWidget(project, "Settings"),
        mSelectedToolSettingsIndex(-1)
	{}

    SettingsWindow::~SettingsWindow ()
	{}

    void
    SettingsWindow::Draw
    ()
    {
        ImGui::Begin(mName.c_str(), &mVisible);

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

        if (ImGui::BeginTabBar("Settings", tab_bar_flags))
		{
		   if (ImGui::BeginTabItem("Connection"))
		   {
        		DrawConnectionSettings();
				ImGui::EndTabItem();
		   }
		   if (ImGui::BeginTabItem("Interface"))
		   {
        		DrawInterfaceSettings();
				ImGui::EndTabItem();
		   }
		   if (ImGui::BeginTabItem("Machine"))
		   {
        		DrawMachineSettings();
			    ImGui::EndTabItem();
		   }
           if (ImGui::BeginTabItem("GRBL"))
           {
               DrawGrblSettings();
               ImGui::EndTabItem();
           }
           if (ImGui::BeginTabItem("Tool Holders"))
		   {
        		DrawToolHolderSettings();
			    ImGui::EndTabItem();
		   }
           if (ImGui::BeginTabItem("Tools"))
		   {
        		DrawToolSettings();
			    ImGui::EndTabItem();
		   }
		   ImGui::EndTabBar();
	   }
        ImGui::End();
    }

    void SettingsWindow::DrawConnectionSettings()
    {
        ConnectionSettings& cs = mAppState->GetSettingsModel().GetConnectionSettings();
        SerialPortModel& serial_model = mAppState->GetSerialPortModel();
        vector<string> ports = serial_model.GetSerialPortNames();

        ImGui::Columns(2);

        int connectionPortComboIndex = distance(
    		ports.begin(),
    	    find(ports.begin(), ports.end(), cs.GetSerialPort())
        );

        if(StringCombo("Port",&connectionPortComboIndex,ports))
        {
            cs.SetSerialPort(ports.at(connectionPortComboIndex));
			serial_model.SetSerialPort(ports.at(connectionPortComboIndex));
        }

        ImGui::NextColumn();

        if (ImGui::Button("Scan Ports"))
        {
			serial_model.ProbeSerialPortNames();
        }

        ImGui::NextColumn();

        int connectionBaudRateComboIndex = distance(
		   SerialPortModel::BaudRateNames.begin(),
		   find(
		   	SerialPortModel::BaudRateNames.begin(),
		   	SerialPortModel::BaudRateNames.end(),
		   	std::to_string(cs.GetSerialBaudRate()))
        );

        if (StringCombo("Baudrate",&connectionBaudRateComboIndex, SerialPortModel::BaudRateNames))
        {
			cs.SetSerialBaudRate(stoi(SerialPortModel::BaudRateNames.at(connectionBaudRateComboIndex)));
			serial_model.SetBaudRate(stoi(SerialPortModel::BaudRateNames.at(connectionBaudRateComboIndex)));
        }

        ImGui::NextColumn();

        ImGui::DragInt("Timeout",cs.GetTimeoutPtr());

        ImGui::Columns(1);

    }

    void SettingsWindow::DrawInterfaceSettings()
    {
        InterfaceSettings& ifs = mAppState->GetSettingsModel().GetInterfaceSettings();

        ImGui::Text("Console");

        bool show_ui_commands = ifs.GetShowUiCommands();
        if (ImGui::Checkbox("Show Ui Commands",&show_ui_commands))
        {
            ifs.SetShowUiCommands(show_ui_commands);
        }

        ImGui::Separator();

        ImGui::Text("Preview");

        ImGui::Columns(2);

		ImGui::DragFloat("Line Width", ifs.GetLineWidthPtr());
        ImGui::NextColumn();

        ImGui::ColorEdit4("Visualiser Color",ifs.GetVisualiserColorArray());
        ImGui::NextColumn();
        ImGui::ColorEdit4("Background Color",ifs.GetBackgroundColorArray());
        ImGui::NextColumn();
        ImGui::ColorEdit4("Tool Color",ifs.GetToolColorArray());
        ImGui::NextColumn();
        ImGui::ColorEdit4("ToolPath Color",ifs.GetToolPathColorArray());
        ImGui::NextColumn();
        ImGui::ColorEdit4("Normal Color",ifs.GetNormalColorArray());
        ImGui::NextColumn();
        ImGui::ColorEdit4("Highlight Color",ifs.GetHighlightColorArray());
        ImGui::NextColumn();
        ImGui::ColorEdit4("Start Point Color",ifs.GetStartPointColorArray());
        ImGui::NextColumn();
        ImGui::ColorEdit4("End Point Color",ifs.GetEndPointColorArray());
        ImGui::NextColumn();
        ImGui::ColorEdit4("Text Color",ifs.GetTextColorArray());
        ImGui::NextColumn();
        ImGui::ColorEdit4("Drawn Color",ifs.GetDrawnColorArray());
        ImGui::NextColumn();
        ImGui::ColorEdit4("Z-Movement Color",ifs.GetZMovementColorArray());
        ImGui::NextColumn();
        ImGui::Columns(1);

        ImGui::Separator();


    }
// MACINE // -------------------------------------------------------------------
    void SettingsWindow::DrawMachineSettings()
    {
        MachineSettings& ms = mAppState->GetSettingsModel().GetMachineSettings();
        ImGui::DragFloat("Status Query Period", ms.GetQueryPeriodPtr());


        const char* data = ms.GetSafePositionCmds().c_str();
        char safePosBuffer[BUFSIZ] = {0};
        strncpy(safePosBuffer, data, BUFSIZ);
        if (ImGui::InputText("Safe Position Command",safePosBuffer,BUFSIZ))
        {
    		ms.SetSafePositionCmds(string(safePosBuffer));
    	}

        const char* probeCmdData = ms.GetProbeCmds().c_str();
        char probeCmdsBuffer[BUFSIZ] = {0};
        strncpy(probeCmdsBuffer,probeCmdData,BUFSIZ);
        ImGui::InputText("Z Probe Command",probeCmdsBuffer,BUFSIZ);

        ImGui::InputFloat3("Working Area", ms.GetWorkAreaArray());
    }

    void SettingsWindow::DrawToolHolderSettings()
    {

    }

    void SettingsWindow::DrawToolSettings()
    {
    	SettingsModel& settings = mAppState->GetSettingsModel();

		ImGui::Columns(2);

        // Left Pane -----------------------------------------------------------

        ImGui::BeginChild("ToolsLeftPane");
        {

			if(ImGui::Button("Add Tool"))
			{
				settings.AddToolSettings();
			}


            for (int i=0; i<settings.GetToolSettingsVector().size(); i++ )
            {
                ToolSettings& ts = settings.GetToolSettingsVector().at(i);
				if (ImGui::TreeNodeEx((void*)((intptr_t)ts.GetID()),
            		ImGuiTreeNodeFlags_Leaf, "%s", ts.GetName().c_str()))
                {
                    mSelectedToolSettingsIndex = i;
                    ImGui::TreePop();
                }

            }
        }
        ImGui::EndChild();

        ImGui::NextColumn();

        // Right Pane ----------------------------------------------------------

        ImGui::BeginChild("ToolsRightPane");
        {
            if (mSelectedToolSettingsIndex > -1)
            {
                ToolSettings& ts = settings.GetToolSettingsVector().at(mSelectedToolSettingsIndex);
				char name_buffer[128] = {0};
                strncpy(name_buffer, ts.GetName().c_str(),128);
				if (ImGui::InputText("Name",name_buffer, 128))
				{
					ts.SetName(name_buffer);
				}

				int toolNumber = ts.GetToolNumber();
				if(ImGui::InputInt("Tool Number",&toolNumber))
				{
					ts.SetToolNumber(toolNumber);
				}

				int current_tool_holder = 0;
				vector<string> th;
				th.push_back("test");
				if (StringCombo("Tool Holder",&current_tool_holder,th))
				{

				}

				if(ImGui::Button("Remove Tool"))
				{
					settings.RemoveToolSettings(ts);
				}

				ImGui::Separator();

				ImGui::Text("Tool Geometry");

				ImGui::Columns(5);
				{
					ImGui::Text("Length"); ImGui::NextColumn();
					ImGui::Text("Upper Diameter"); ImGui::NextColumn();
					ImGui::Text("Lower Diameter"); ImGui::NextColumn();
					ImGui::Text("Faces"); ImGui::NextColumn();
					ImGui::Text("Remove"); ImGui::NextColumn();
					ImGui::Separator();
                    vector<Cylinder>& cylinders = ts.GetCylindersVector();
                    for (Cylinder& c : cylinders)
                    {
                        float length, ud, ld, faces;
                        length = c.GetLength();
                        ud = c.GetUpperDiameter();
                        ld = c.GetLowerDiameter();
                        faces = c.GetFaces();

						if (ImGui::InputFloat("##Length", &length))
                        {
                           c.SetLength(length);
                        }
                        ImGui::NextColumn();

						if (ImGui::InputFloat("##Upper Diameter",&ud))
                        {
                           c.SetUpperDiameter(ud);
                        }
                        ImGui::NextColumn();

						if(ImGui::InputFloat("##Lower Diameter",&ld))
                        {
                           c.SetLowerDiameter(ld);
                        }
                        ImGui::NextColumn();

						if(ImGui::InputFloat("##Faces",&faces))
                        {
                           c.SetFaces(faces);
                        }
                        ImGui::NextColumn();

                        if (ImGui::Button("Remove"))
                        {

                        }
                        ImGui::NextColumn();
                    }
				}

				ImGui::Columns(1);

				if(ImGui::Button("Add Cylinder"))
				{
				   ts.AddCylinder(Cylinder());
				}
            }
        }
        ImGui::EndChild();

        ImGui::Columns(1);

    }

	void SettingsWindow::DrawGrblSettings()
    {
        GrblConfigurationModel& configModel = mAppState->GetGrblMachineModel().GetConfigurationModel();
		if (ImGui::Button("Read Firmware Settings"))
        {

        }

        ImGui::SameLine();

        if (ImGui::Button("Write Firmware Settings"))
        {

        }

        ImGui::BeginChild("Table");
        ImGui::Columns(3);
        ImGui::Text("Variable"); ImGui::NextColumn();
        ImGui::Text("Description"); ImGui::NextColumn();
        ImGui::Text("Value"); ImGui::NextColumn();

        ImGui::Separator();

        for (pair<int,string> p : GrblConfigurationModel::ConfigurationKeys)
        {
			ImGui::Text("$%d",p.first); ImGui::NextColumn();
			ImGui::Text("%s",p.second.c_str()); ImGui::NextColumn();
            char buf[BUFSIZ] = {0};
            strncpy(buf, configModel.GetValue(p.first).c_str(),BUFSIZ);
            ImGui::PushItemWidth(-1);
			if(ImGui::InputText("##",buf,BUFSIZ))
            {
               configModel.SetValue(p.first,buf);
            }
            ImGui::NextColumn();
        	ImGui::Separator();
        }
        ImGui::EndChild();
    }
}



