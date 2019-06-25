#pragma once

#include "JsonSerialization.h"

#include <vector>
#include "ConnectionSettings.h"
#include "InterfaceSettings.h"
#include "MachineSettings.h"
#include "ToolHolderSettings.h"
#include "ToolSettings.h"

using std::vector;

namespace Coconut
{
    class AppState;
	class SettingsModel : public JsonSerialization
	{
	public:
		SettingsModel(AppState* state);

        json ToJson() override;
        bool FromJson(const json& j) override;

        ConnectionSettings& GetConnectionSettings();
        InterfaceSettings& GetInterfaceSettings();
        MachineSettings& GetMachineSettings();
        vector<ToolHolderSettings>& GetToolHolderSettingsVector();
        vector<ToolSettings>& GetToolSettingsVector();

        void AddToolSettings();
        void RemoveToolSettings(const ToolSettings& tsm);
        void AddToolHolderSettings();

    private:
        AppState* mAppState;
        ConnectionSettings mConnectionSettings;
        InterfaceSettings mInterfaceSettings;
        MachineSettings mMachineSettings;
        vector<ToolHolderSettings> mToolHolderSettingsVector;
        vector<ToolSettings> mToolSettingsVector;

        const static string SETTINGS_CONNECTION;
        const static string SETTINGS_INTERFACE;
        const static string SETTINGS_MACHINE;
        const static string SETTINGS_TOOLHOLDERS;
        const static string SETTINGS_TOOLS;
    };
}

