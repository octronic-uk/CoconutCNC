#include "SettingsModel.h"



namespace Coconut
{
	SettingsModel::SettingsModel(AppState* state)
        : mAppState(state)
	{

	}

	json SettingsModel::ToJson()
	{
        json j;
        j[SETTINGS_CONNECTION] = mConnectionSettings.ToJson();
        j[SETTINGS_INTERFACE] = mInterfaceSettings.ToJson();
        j[SETTINGS_MACHINE] = mMachineSettings.ToJson();

        j[SETTINGS_TOOLHOLDERS] = json::array();
        for (ToolHolderSettings& ths : mToolHolderSettingsVector)
        {
            j[SETTINGS_TOOLHOLDERS].push_back(ths.ToJson());
        }

        j[SETTINGS_TOOLS] = json::array();
        for (ToolSettings& ts : mToolSettingsVector)
        {
            j[SETTINGS_TOOLS].push_back(ts.ToJson());
        }

        return j;
	}

	bool SettingsModel::FromJson(const json& j)
	{
        if (!j[SETTINGS_CONNECTION].is_object() ||
            mConnectionSettings.FromJson(j[SETTINGS_CONNECTION])) return false;

        if (!j[SETTINGS_INTERFACE].is_object() ||
            !mInterfaceSettings.FromJson(j[SETTINGS_INTERFACE])) return false;

        if (!j[SETTINGS_MACHINE].is_object() ||
        	!mMachineSettings.FromJson(j[SETTINGS_MACHINE])) return false;

        if (!j[SETTINGS_TOOLHOLDERS].is_array()) return false;
        for (const json& toolholder_j : j[SETTINGS_TOOLHOLDERS])
        {
			ToolHolderSettings ths;
            ths.FromJson(toolholder_j);
            mToolHolderSettingsVector.push_back(ths);
        }

        if (!j[SETTINGS_TOOLS].is_array()) return false;
        for (const json& tool_j : j[SETTINGS_TOOLS])
        {
			ToolSettings ts;
            ts.FromJson(tool_j);
            mToolSettingsVector.push_back(ts);
        }

        return true;
	}

	vector<ToolSettings>& SettingsModel::GetToolSettingsVector()
	{
		return mToolSettingsVector;
	}

	vector<ToolHolderSettings>& SettingsModel::GetToolHolderSettingsVector()
	{
		return mToolHolderSettingsVector;
	}

	MachineSettings& SettingsModel::GetMachineSettings()
	{
		return mMachineSettings;
	}

	InterfaceSettings& SettingsModel::GetInterfaceSettings()
	{
		return mInterfaceSettings;
	}

	ConnectionSettings& SettingsModel::GetConnectionSettings()
	{
		return mConnectionSettings;
	}

    void SettingsModel::AddToolSettings()
    {
        mToolSettingsVector.push_back(ToolSettings());
    }

    void SettingsModel::AddToolHolderSettings()
    {
        mToolHolderSettingsVector.push_back(ToolHolderSettings());
    }

    void SettingsModel::RemoveToolSettings(const ToolSettings& tsm)
	{
    	auto itr = mToolSettingsVector.begin();
        while (itr != mToolSettingsVector.end() && (*itr).GetID() != tsm.GetID())
        {
            itr++;
        }
        if (itr != mToolSettingsVector.end())
        {
            mToolSettingsVector.erase(itr);
        }
	}

    const string SettingsModel::SETTINGS_CONNECTION = "connection";
	const string SettingsModel::SETTINGS_INTERFACE = "interface";
	const string SettingsModel::SETTINGS_MACHINE = "machine";
	const string SettingsModel::SETTINGS_TOOLHOLDERS = "toolholders";
	const string SettingsModel::SETTINGS_TOOLS = "tools";
}
