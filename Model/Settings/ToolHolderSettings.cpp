/*
 * Copyright 2018 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "ToolHolderSettings.h"
#include "../../Common/Logger.h"

#include <string>
using std::string;

namespace Coconut
{
	ToolHolderSettings::ToolHolderSettings(string name, int id) :
    	mName(name),
    	mID(id < 0 ? rand() : id)
	{
		debug("ToolHolder: Creating with id {}", id);
	}

	int ToolHolderSettings::GetID() const
	{
        return mID;
    }

    void ToolHolderSettings::SetID(int id)
    {
    	mID = id;
    }

    string ToolHolderSettings::GetName() const
    {
        return mName;
    }

    void ToolHolderSettings::SetName(const string& name)
    {
        mName = name;
    }

    nlohmann::json ToolHolderSettings::ToJson()
    {
        info("SettingsModel: {}",__FUNCTION__);
        json j;
        j[TOOLHOLDER_ID] = GetID();
        j[TOOLHOLDER_NAME] = GetName();

        j[TOOLHOLDER_CYLINDERS] = json::array();
        for (Cylinder c : mCylinders)
        {
       		j[TOOLHOLDER_CYLINDERS].push_back(c.ToJson());
        }
        return j;
    }

    bool ToolHolderSettings::FromJson(const nlohmann::json& j)
    {
        info("SettingsModel: {}",__FUNCTION__);
        if (j.find(TOOLHOLDER_ID) != j.end() && j[TOOLHOLDER_ID].is_number())
        {
        	SetID(j[TOOLHOLDER_ID]);
        }

        if (j.find(TOOLHOLDER_NAME) != j.end() && j[TOOLHOLDER_NAME].is_string())
        {
        	SetName(j[TOOLHOLDER_NAME]);
        }

 		if (j.find(TOOLHOLDER_CYLINDERS) != j.end() && j[TOOLHOLDER_CYLINDERS].is_array())
        {
			mCylinders.clear();
			for (const json& cylinder_json : j[TOOLHOLDER_CYLINDERS])
			{
				Cylinder cylinder;
				cylinder.FromJson(cylinder_json);
				mCylinders.push_back(cylinder);
			}
        }
        return true;
    }

    vector<Cylinder>& ToolHolderSettings::GetCylindersVector()
	{
        return mCylinders;
	}

    const string ToolHolderSettings::TOOLHOLDER_ID = "id";
    const string ToolHolderSettings::TOOLHOLDER_NAME = "name";
    const string ToolHolderSettings::TOOLHOLDER_CYLINDERS = "cylinders";


}
