#pragma once


#include <vector>

#include "../../Common/File.h"

#include "JsonSerialization.h"
#include "ConnectionSettings.h"
#include "InterfaceSettings.h"
#include "MachineSettings.h"
#include "ToolHolderSettings.h"
#include "ToolSettings.h"

#define SETTINGS_FILE_PATH "coconut_settings.json"

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
        ToolSettings& GetToolSettingsByToolNumber(int id);
        void RemoveToolSettings(const ToolSettings& tsm);

        void AddToolHolderSettings();
        ToolHolderSettings& GetToolHolderSettingsByID(int id);
        void RemoveToolHolderSettings(const ToolHolderSettings& tsm);

        bool SaveSettingsFile();

    protected:
    	void CreateOrOpenSettingsFile();

    private:
        File mSettingsFile;
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

