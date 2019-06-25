/*
 * ToolSettings.cpp
 *
 * Created: 06 2018 by Ashley
 *
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

#include "ToolSettings.h"
#include "../../Common/Logger.h"

namespace Coconut
{
	ToolSettings::ToolSettings
	(string name, int id, int tool_number, float length, float diameter, int faces)
		: mName(name),
          mID(id < 0 ? rand() : id),
          mToolNumber(tool_number),
		  mDiameter(diameter),
		  mFaces(faces)
	{
		debug("ToolModelGeometryItem: Creating with id {} num {}" , id, tool_number);
	}

	float ToolSettings::GetDiameter() const
	{
		return mDiameter;
	}

	void ToolSettings::SetDiameter(float upperDiameter)
	{
		mDiameter = upperDiameter <= 0 ? 1 : upperDiameter;
	}

	int ToolSettings::GetID() const
	{
		return mID;
	}

	void ToolSettings::SetID(int id)
	{
        mID = id;
    }

    std::string ToolSettings::GetName() const
    {
       return mName;
    }

    void ToolSettings::SetName(const std::string& name)
    {
       mName = name;
    }

	int ToolSettings::GetFaces() const
	{
		return mFaces;
	}

	void ToolSettings::SetFaces(int faces)
	{
		mFaces = faces;
    }

    int ToolSettings::GetToolHolderID() const
    {
        return mToolHolderID;
    }

    void ToolSettings::SetToolHolderID(int toolHolderID)
    {
        mToolHolderID = toolHolderID;
    }

    int  ToolSettings::GetToolNumber() const
    {
        return mToolNumber;
    }

    void ToolSettings::SetToolNumber(int num)
    {
        mToolNumber = num;
    }

    json ToolSettings::ToJson()
	{
        json j;
        j[TOOL_ID]        = GetID();
        j[TOOL_NAME]      = GetName();
        j[TOOL_NUMBER]    = GetToolNumber();
        j[TOOL_HOLDER_ID] = GetToolHolderID();
        j[TOOL_DIAMETER]  = GetDiameter();
        j[TOOL_FACES]     = GetFaces();

        j[TOOL_CYLINDERS] = json::array();
        for (Cylinder c : mCylinders)
        {
            j[TOOL_CYLINDERS].push_back(c.ToJson());
        }

        return j;
	}

	bool ToolSettings::FromJson(const json& j)
	{
        if (!j[TOOL_ID].is_number()) return false;
        SetID(j[TOOL_ID]);

        if (!j[TOOL_NAME].is_string()) return false;
        SetName(j[TOOL_NAME]);

        if (!j[TOOL_NUMBER].is_number()) return false;
        SetToolNumber(j[TOOL_NUMBER]);

        if (!j[TOOL_HOLDER_ID].is_number()) return false;
        SetToolHolderID(j[TOOL_HOLDER_ID]);

        if (!j[TOOL_DIAMETER].is_number()) return false;
        SetDiameter(j[TOOL_DIAMETER]);

        if (!j[TOOL_FACES].is_number()) return false;
        SetFaces(j[TOOL_FACES]);

        if (!j[TOOL_CYLINDERS].is_array()) return false;
        mCylinders.clear();
        for (const json& cylinder_json : j[TOOL_CYLINDERS])
        {
            Cylinder cylinder;
            cylinder.FromJson(cylinder_json);
            mCylinders.push_back(cylinder);
        }

        return true;
	}

	vector<Cylinder>& ToolSettings::GetCylindersVector()
	{
        return mCylinders;
    }

    void ToolSettings::AddCylinder(const Cylinder& c)
	{
        mCylinders.push_back(c);
    }


    // Tool
	const string ToolSettings::TOOL_ID = "id";
	const string ToolSettings::TOOL_NAME = "name";
	const string ToolSettings::TOOL_NUMBER = "number";
	const string ToolSettings::TOOL_HOLDER_ID = "holder_id";
	const string ToolSettings::TOOL_DIAMETER = "diameter";
	const string ToolSettings::TOOL_FACES = "faces";
	const string ToolSettings::TOOL_CYLINDERS = "cylinders";
}


