#include "SettingsWindow.h"
#include "../../AppState.h"

#include "../GL/ToolDrawer.h"
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
        mSelectedToolSettingsIndex(-1),
        mToolSettingsIndexToRemove(-1),
        mSelectedToolHolderSettingsIndex(-1),
        mToolHolderSettingsIndexToRemove(-1),
        mUpdateToolDrawer(false)
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

        SettingsModel& settings = mAppState->GetSettingsModel();

        if (mToolSettingsIndexToRemove > -1)
        {
            auto itr = settings.GetToolSettingsVector().begin() + mToolSettingsIndexToRemove;
            settings.RemoveToolSettings(*itr);
        	mToolSettingsIndexToRemove = -1;
        	mSelectedToolSettingsIndex = -1;
        }

        if (mToolHolderSettingsIndexToRemove > -1)
        {
            auto itr = settings.GetToolHolderSettingsVector().begin() + mToolHolderSettingsIndexToRemove;
            settings.RemoveToolHolderSettings(*itr);
	   		mToolHolderSettingsIndexToRemove = -1;
	   		mSelectedToolHolderSettingsIndex = -1;
        }

        if (mUpdateToolDrawer)
        {
            mAppState->GetToolDrawer().SetNeedsGeometryUpdate(true);
            mUpdateToolDrawer = false;
        }
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
			serial_model.SetPortName(ports.at(connectionPortComboIndex));
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

        ImGui::DragInt("Timeout",cs.GetTimeoutPtr(),1,0,0,"%d ms");

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

    void SettingsWindow::DrawMachineSettings()
    {
        ImGui::Columns(2);

        MachineSettings& ms = mAppState->GetSettingsModel().GetMachineSettings();
        ImGui::DragInt("Status Query Interval", ms.GetStatusQueryIntervalPtr(),1.,1.f,0,"%d ms");
        ImGui::NextColumn();
        ImGui::DragInt("Program Send Interval", ms.GetProgramSendIntervalPtr(),1.,1.f,0,"%d ms");
		ImGui::NextColumn();

        const char* data = ms.GetSafePositionCmds().c_str();
        char safePosBuffer[BUFSIZ] = {0};
        strncpy(safePosBuffer, data, BUFSIZ);
        if (ImGui::InputText("Safe Position Command",safePosBuffer,BUFSIZ))
        {
    		ms.SetSafePositionCmds(string(safePosBuffer));
    	}
		ImGui::NextColumn();

        const char* probeCmdData = ms.GetProbeCmds().c_str();
        char probeCmdsBuffer[BUFSIZ] = {0};
        strncpy(probeCmdsBuffer,probeCmdData,BUFSIZ);
        ImGui::InputText("Z Probe Command",probeCmdsBuffer,BUFSIZ);
		ImGui::NextColumn();

        vec3 working_area_vec = ms.GetWorkArea();
        float working_area[3] = {
            working_area_vec.x,
            working_area_vec.y,
            working_area_vec.z
    	};
        if (ImGui::InputFloat3("Working Area", working_area,"%.2f mm"))
        {
            working_area_vec.x = working_area[0];
            working_area_vec.y = working_area[1];
            working_area_vec.z = working_area[2];
            ms.SetWorkArea(working_area_vec);
        }
        ImGui::Columns(1);
    }

    void SettingsWindow::DrawToolHolderSettings()
    {
        SettingsModel& settings = mAppState->GetSettingsModel();

		ImGui::Columns(2);

        // Left Pane -----------------------------------------------------------

        ImGui::BeginChild("ToolHoldersLeftPane");
        {

			if(ImGui::Button("Add Tool Holder"))
			{
				settings.AddToolHolderSettings();
			}

            if (!settings.GetToolHolderSettingsVector().empty())
            {

            	for (int i=0; i<settings.GetToolHolderSettingsVector().size(); i++ )
            	{
					ToolHolderSettings& ts = settings.GetToolHolderSettingsVector().at(i);
					if (ImGui::TreeNodeEx((void*)((intptr_t)ts.GetID()),
						ImGuiTreeNodeFlags_Leaf, "%s", ts.GetName().c_str()))
					{
						ImGui::TreePop();
					}

					if (ImGui::IsItemClicked())
					{
						mSelectedToolHolderSettingsIndex = i;
						info("SettingsWindow: Selected ToolHolder {}", i);
					}

				}

            }
        }
        ImGui::EndChild();

        ImGui::NextColumn();

        // Right Pane ----------------------------------------------------------

        ImGui::BeginChild("ToolHoldersRightPane");
        {
            if (mSelectedToolHolderSettingsIndex > -1)
            {
                ToolHolderSettings& ts = settings.GetToolHolderSettingsVector().at(mSelectedToolHolderSettingsIndex);
				char name_buffer[128] = {0};
                strncpy(name_buffer, ts.GetName().c_str(),128);
				if (ImGui::InputText("Name",name_buffer, 128))
				{
					ts.SetName(name_buffer);
				}

				if(ImGui::Button("Remove Tool Holder"))
				{
                    mToolHolderSettingsIndexToRemove = mSelectedToolHolderSettingsIndex;
				}

				ImGui::Separator();

				ImGui::Text("Tool Holder Geometry");

				ImGui::Columns(5);

				ImGui::Text("Length"); ImGui::NextColumn();
				ImGui::Text("Upper Diameter"); ImGui::NextColumn();
				ImGui::Text("Lower Diameter"); ImGui::NextColumn();
				ImGui::Text("Faces"); ImGui::NextColumn();
				ImGui::Text("Remove"); ImGui::NextColumn();
				ImGui::Separator();
				vector<Cylinder>& cylinders = ts.GetCylindersVector();
				vector<Cylinder>::iterator cylinder_to_remove = cylinders.end();
				for (Cylinder& c : cylinders)
				{
					float length, ud, ld;
					length = c.GetLength();
					ud = c.GetUpperDiameter();
					ld = c.GetLowerDiameter();
					int faces = c.GetFaces();

                    ImGui::PushID(c.GetID());

					if (ImGui::InputFloat("##Length", &length))
					{
					   c.SetLength(length);
                       mUpdateToolDrawer = true;
					}
					ImGui::NextColumn();

					if (ImGui::InputFloat("##Upper Diameter",&ud))
					{
					   c.SetUpperDiameter(ud);
                       mUpdateToolDrawer = true;
					}
					ImGui::NextColumn();

					if(ImGui::InputFloat("##Lower Diameter",&ld))
					{
					   c.SetLowerDiameter(ld);
                       mUpdateToolDrawer = true;
					}
					ImGui::NextColumn();

					if(ImGui::InputInt("##Faces",&faces))
					{
					   c.SetFaces(faces);
                       mUpdateToolDrawer = true;
					}
					ImGui::NextColumn();

					if (ImGui::Button("Remove"))
					{
						cylinder_to_remove = find(cylinders.begin(),cylinders.end(),c);
                        mUpdateToolDrawer = true;
					}
					ImGui::NextColumn();

                    ImGui::PopID();
				}

                if (cylinder_to_remove != cylinders.end())
                {
                    cylinders.erase(cylinder_to_remove);
                }

				ImGui::Columns(1);

				if(ImGui::Button("Add Cylinder"))
				{
				   ts.AddCylinder(Cylinder());
				   mUpdateToolDrawer = true;
				}
            }
        }
        ImGui::EndChild();

        ImGui::Columns(1);
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

            if (!settings.GetToolSettingsVector().empty())
            {

				for (int i=0; i<settings.GetToolSettingsVector().size(); i++ )
				{
					ToolSettings& ts = settings.GetToolSettingsVector().at(i);
					if (ImGui::TreeNodeEx((void*)((intptr_t)ts.GetID()),
						ImGuiTreeNodeFlags_Leaf, "%s", ts.GetName().c_str()))
					{
						ImGui::TreePop();
					}

					if (ImGui::IsItemClicked())
					{
						mSelectedToolSettingsIndex = i;
						info("SettingsWindow: Selected tool {}", i);
					}

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

				vector<ToolHolderSettings>& tool_holders = settings.GetToolHolderSettingsVector();

				auto selected_th = find(tool_holders.begin(), tool_holders.end(),
                     settings.GetToolHolderSettingsByID(ts.GetToolHolderID()));

				int current_tool_holder = distance(tool_holders.begin(),selected_th);

                vector<string> names;
				for (ToolHolderSettings& th : tool_holders)
				{
					names.push_back(th.GetName());
				}

				if (StringCombo("Tool Holder",&current_tool_holder,names))
				{
					ts.SetToolHolderID(tool_holders.at(current_tool_holder).GetID());
				}

				if(ImGui::Button("Remove Tool"))
				{
                    mToolSettingsIndexToRemove = mSelectedToolSettingsIndex;
                    mSelectedToolSettingsIndex = -1;
				}

				ImGui::Separator();

				ImGui::Text("Tool Geometry");

				ImGui::Columns(5);

				ImGui::Text("Length"); ImGui::NextColumn();
				ImGui::Text("Upper Diameter"); ImGui::NextColumn();
				ImGui::Text("Lower Diameter"); ImGui::NextColumn();
				ImGui::Text("Faces"); ImGui::NextColumn();
				ImGui::Text("Remove"); ImGui::NextColumn();
				ImGui::Separator();
				vector<Cylinder>& cylinders = ts.GetCylindersVector();
				vector<Cylinder>::iterator cylinder_to_remove = cylinders.end();

				for (Cylinder& c : cylinders)
				{
					float length, ud, ld;
					length = c.GetLength();
					ud = c.GetUpperDiameter();
					ld = c.GetLowerDiameter();
					int faces = c.GetFaces();

                    ImGui::PushID(c.GetID());

					if (ImGui::InputFloat("##Length", &length))
					{
					   c.SetLength(length);
                       mUpdateToolDrawer = true;
					}
					ImGui::NextColumn();

					if (ImGui::InputFloat("##Upper Diameter",&ud))
					{
					   c.SetUpperDiameter(ud);
                       mUpdateToolDrawer = true;
					}
					ImGui::NextColumn();

					if(ImGui::InputFloat("##Lower Diameter",&ld))
					{
					   c.SetLowerDiameter(ld);
                       mUpdateToolDrawer = true;
					}
					ImGui::NextColumn();

					if(ImGui::InputInt("##Faces",&faces))
					{
					   c.SetFaces(faces);
                       mUpdateToolDrawer = true;
					}
					ImGui::NextColumn();

					if (ImGui::Button("Remove"))
					{
						cylinder_to_remove = find(cylinders.begin(),cylinders.end(), c);
                       mUpdateToolDrawer = true;
					}

					ImGui::NextColumn();

                    ImGui::PopID();
				}

                if (cylinder_to_remove != cylinders.end())
                {
                    cylinders.erase(cylinder_to_remove);
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
        GrblMachineModel& grbl = mAppState->GetGrblMachineModel();
        GrblConfigurationModel& configModel = grbl.GetConfigurationModel();
		if (ImGui::Button("Read Firmware Settings"))
        {
			grbl.SendManualGCodeCommand(GCodeCommand::GetFirmwareConfigurationCommand());
        }

        ImGui::BeginChild("Table");
        ImGui::Columns(4);
        ImGui::SetColumnWidth(0,50);
        ImGui::Text("Variable"); ImGui::NextColumn();
        ImGui::Text("Description"); ImGui::NextColumn();
        ImGui::Text("Value"); ImGui::NextColumn();
        ImGui::Text("Set"); ImGui::NextColumn();

        ImGui::Separator();

        for (pair<int,string> p : GrblConfigurationModel::ConfigurationKeys)
        {
            ImGui::PushID(p.first);

			ImGui::Text("$%d",p.first);
            ImGui::NextColumn();

			ImGui::Text("%s",p.second.c_str());
            ImGui::NextColumn();

            char buf[BUFSIZ] = {0};
            strncpy(buf, configModel.GetValue(p.first).c_str(),BUFSIZ);
            ImGui::PushItemWidth(-1);
			if(ImGui::InputText("##",buf,BUFSIZ))
            {
               configModel.SetValue(p.first,buf);
            }
            ImGui::NextColumn();

            if (ImGui::Button("Set"))
            {
    			grbl.SendManualGCodeCommand(
    				GCodeCommand::SetFirmwareConfigurationCommand(
    					p.first,
    					configModel.GetValue(p.first).c_str()
    				)
    			);
            }
            ImGui::NextColumn();

        	ImGui::Separator();
            ImGui::PopID();
        }
        ImGui::EndChild();
    }
}



