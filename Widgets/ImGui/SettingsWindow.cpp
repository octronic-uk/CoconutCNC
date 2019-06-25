#include "SettingsWindow.h"
#include "../../AppState.h"

#include "../../Model/Settings/ToolSettings.h"
#include "../../Model/Settings/ToolHolderSettings.h"

namespace Coconut
{
    SettingsWindow::SettingsWindow(AppState* project)
        : ImGuiWidget(project, "Settings"),
        mConnectionPortComboIndex(0),
        mSelectedToolSettingsIndex(-1)
	{}

    SettingsWindow::~SettingsWindow ()
	{}

    void
    SettingsWindow::Draw
    ()
    {
        ImGui::Begin(mName.c_str(), &mVisible);

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None |
        	ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

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
        if (ImGui::Button("Scan Serial Ports"))
        {

        }

        if(StringCombo("Port",&mConnectionPortComboIndex,mConnectionPortList))
        {

        }

        if (StringCombo("Baudrate",&mConnectionBaudRateComboIndex, mConnectionBaudRateList))
        {

        }
    }

    void SettingsWindow::DrawInterfaceSettings()
    {

    }

    void SettingsWindow::DrawMachineSettings()
    {

    }

    void SettingsWindow::DrawToolHolderSettings()
    {

    }

    void SettingsWindow::DrawToolSettings()
    {
    	SettingsModel* settings = mAppState->GetSettingsModel();

		ImGui::Columns(2);

        // Left Pane -----------------------------------------------------------

        ImGui::BeginChild("ToolsLeftPane");
        {

			if(ImGui::Button("Add Tool"))
			{
				settings->AddToolSettings();
			}


            for (int i=0; i<settings->GetToolSettingsVector().size(); i++ )
            {
                ToolSettings& ts = settings->GetToolSettingsVector().at(i);
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
                ToolSettings& ts = settings->GetToolSettingsVector().at(mSelectedToolSettingsIndex);
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
					settings->RemoveToolSettings(ts);
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
}



