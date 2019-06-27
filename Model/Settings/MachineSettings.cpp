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

#include "MachineSettings.h"
#include "../../Common/Logger.h"
#include <glm/gtc/type_ptr.hpp>

using glm::value_ptr;

namespace Coconut
{
	MachineSettings::MachineSettings()
		:  mQueryPeriod(100),
		  mProbeCmds("G21G91G38.2Z-30F100; G0Z1; G38.2Z-2F10"),
		  mSafePositionCmds("G21G90; G53G0Z0"),
		  mWorkArea(0)
	{

		debug("MachineSettings: Constructing");
	}

	float MachineSettings::GetQueryPeriod() const
	{
        return mQueryPeriod;
    }

    float* MachineSettings::GetQueryPeriodPtr()
    {
    	return &mQueryPeriod;
    }

	void MachineSettings::SetQueryPeriod(float queryPeriod)
	{
		mQueryPeriod = queryPeriod;
	}

	string MachineSettings::GetProbeCmds() const
	{
		return mProbeCmds;
	}

	void MachineSettings::SetProbeCmds(const string& probeCmds)
	{
		mProbeCmds = probeCmds;
	}

	string MachineSettings::GetSafePositionCmds() const
	{
		return mSafePositionCmds;
	}

	void MachineSettings::SetSafePositionCmds(const string& safePositionCmds)
	{
		mSafePositionCmds = safePositionCmds;
	}

	vec3 MachineSettings::GetWorkArea() const
	{
		return mWorkArea;
	}

    float* MachineSettings::GetWorkAreaArray()
    {
        return value_ptr(mWorkArea);
    }

	void MachineSettings::SetWorkArea(const vec3& workArea)
	{
		mWorkArea = workArea;
	}

    nlohmann::json MachineSettings::ToJson()
    {
		json j;

		j[MACHINE_QUERY_PERIOD] = GetQueryPeriod();
		j[MACHINE_WORK_AREA] = Vec3ToJson(GetWorkArea());
		j[MACHINE_PROBE_CMDS] = GetProbeCmds();
		j[MACHINE_SAFE_POS_CMDS] = GetSafePositionCmds();

        return j;
    }

    bool MachineSettings::FromJson(const nlohmann::json& j)
    {
        if (j.find(MACHINE_QUERY_PERIOD) != j.end() && j[MACHINE_QUERY_PERIOD].is_number())
        {
			SetQueryPeriod(j[MACHINE_QUERY_PERIOD]);
		}

		if (j.find(MACHINE_WORK_AREA) != j.end() && j[MACHINE_WORK_AREA].is_array())
        {
        	SetWorkArea(JsonToVec3(j[MACHINE_WORK_AREA]));
        }

		if (j.find(MACHINE_PROBE_CMDS) != j.end() && j[MACHINE_PROBE_CMDS].is_string())
        {
        	SetProbeCmds(j[MACHINE_PROBE_CMDS]);
        }

		if (j.find(MACHINE_SAFE_POS_CMDS) != j.end() && j[MACHINE_SAFE_POS_CMDS].is_string())
        {
			SetSafePositionCmds(j[MACHINE_SAFE_POS_CMDS]);
		}

        return true;
    }

    // User Command
    const string MachineSettings::MACHINE_QUERY_PERIOD = "query_period";
	const string MachineSettings::MACHINE_WORK_AREA = "work_area";
	const string MachineSettings::MACHINE_PROBE_CMDS = "probe_cmds";
	const string MachineSettings::MACHINE_SAFE_POS_CMDS = "safe_pos_cmds";
}
