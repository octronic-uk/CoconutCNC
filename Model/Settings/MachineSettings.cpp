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

namespace Coconut
{
	MachineSettings::MachineSettings()
		:  mQueryPeriod(100),
		  mUnits(false),
		  mRapidSpeed(500),
		  mAcceleration(500),
		  mSpindleMin(0),
		  mSpindleMax(10000),
		  mProbeCmds("G21G91G38.2Z-30F100; G0Z1; G38.2Z-2F10"),
		  mSafePositionCmds("G21G90; G53G0Z0"),
		  mRestoreOrigin(false),
		  mRestoreType(false),
		  mUserCmd1(""),
		  mUserCmd2(""),
		  mUserCmd3(""),
		  mUserCmd4(""),
		  mWorkAreaWidth(1),
		  mWorkAreaHeight(1)
	{

		debug("MachineSettings: Constructing");
	}

	int MachineSettings::GetQueryPeriod() const
	{
		return mQueryPeriod;
	}

	void MachineSettings::SetQueryPeriod(int queryPeriod)
	{
		mQueryPeriod = queryPeriod;
	}

	bool MachineSettings::GetUnits() const
	{
		return mUnits;
	}

	void MachineSettings::SetUnits(bool units)
	{
		mUnits = units;
	}

	float MachineSettings::GetRapidSpeed() const
	{
		return mRapidSpeed;
	}

	void MachineSettings::SetRapidSpeed(float rapidSpeed)
	{
		mRapidSpeed = rapidSpeed;
	}

	float MachineSettings::GetAcceleration() const
	{
		return mAcceleration;
	}

	void MachineSettings::SetAcceleration(float acceleration)
	{
		mAcceleration = acceleration;
	}

	int MachineSettings::GetSpindleMin() const
	{
		return mSpindleMin;
	}

	void MachineSettings::SetSpindleMin(int spindleMin)
	{
		mSpindleMin = spindleMin;
	}

	int MachineSettings::GetSpindleMax() const
	{
		return mSpindleMax;
	}

	void MachineSettings::SetSpindleMax(int spindleMax)
	{
		mSpindleMax = spindleMax;
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

	bool MachineSettings::GetRestoreOrigin() const
	{
		return mRestoreOrigin;
	}

	void MachineSettings::SetRestoreOrigin(bool restoreOrigin)
	{
		mRestoreOrigin = restoreOrigin;
	}

	bool MachineSettings::GetRestoreType() const
	{
		return mRestoreType;
	}

	void MachineSettings::SetRestoreType(bool restoreType)
	{
		mRestoreType = restoreType;
	}

	string MachineSettings::GetUserCmd1() const
	{
		return mUserCmd1;
	}

	void MachineSettings::SetUserCmd1(const string& userCmd1)
	{
		mUserCmd1 = userCmd1;
	}

	string MachineSettings::GetUserCmd2() const
	{
		return mUserCmd2;
	}

	void MachineSettings::SetUserCmd2(const string& userCmd2)
	{
		mUserCmd2 = userCmd2;
	}

	string MachineSettings::GetUserCmd3() const
	{
		return mUserCmd3;
	}

	void MachineSettings::SetUserCmd3(const string& userCmd3)
	{
		mUserCmd3 = userCmd3;
	}

	string MachineSettings::GetUserCmd4() const
	{
		return mUserCmd4;
	}

	void MachineSettings::SetUserCmd4(const string& userCmd4)
	{
		mUserCmd4 = userCmd4;
	}

	int MachineSettings::GetWorkAreaWidth() const
	{
		return mWorkAreaWidth;
	}

	void MachineSettings::SetWorkAreaWidth(int workAreaWidth)
	{
		mWorkAreaWidth = workAreaWidth;
	}

	int MachineSettings::GetWorkAreaHeight() const
	{
		return mWorkAreaHeight;
	}

	void MachineSettings::SetWorkAreaHeight(int workAreaHeight)
	{
        mWorkAreaHeight = workAreaHeight;
    }

    nlohmann::json MachineSettings::ToJson()
    {
		json j;

		j[MACHINE_QUERY_PERIOD] = GetQueryPeriod();
		j[MACHINE_SPINDLE_MIN] = GetSpindleMin();
		j[MACHINE_SPINDLE_MAX] = GetSpindleMax();
		j[MACHINE_WORK_WIDTH] = GetWorkAreaWidth();
		j[MACHINE_WORK_HEIGHT] = GetWorkAreaHeight();
		j[MACHINE_RAPID_SPEED] = GetRapidSpeed();
		j[MACHINE_ACCELEREATION] = GetAcceleration();
		j[MACHINE_UNITS] = GetUnits();
		j[MACHINE_RESTORE_ORIGIN] = GetRestoreOrigin();
		j[MACHINE_RESTORE_TYPE] = GetRestoreType();
		j[MACHINE_PROBE_CMDS] = GetProbeCmds();
		j[MACHINE_SAFE_POS_CMDS] = GetSafePositionCmds();
		j[MACHINE_USER_COMMAND_1] = GetUserCmd1();
		j[MACHINE_USER_COMMAND_2] = GetUserCmd2();
		j[MACHINE_USER_COMMAND_3] = GetUserCmd3();
		j[MACHINE_USER_COMMAND_4] = GetUserCmd4();

        return j;
    }

    bool MachineSettings::FromJson(const nlohmann::json& j)
    {
        if (!j[MACHINE_QUERY_PERIOD].is_number()) return false;
        SetQueryPeriod(j[MACHINE_QUERY_PERIOD]);

		if (!j[MACHINE_SPINDLE_MIN].is_number()) return false;
        SetSpindleMin(j[MACHINE_SPINDLE_MIN]);

		if (!j[MACHINE_SPINDLE_MAX].is_number()) return false;
        SetSpindleMax(j[MACHINE_SPINDLE_MAX]);

		if (!j[MACHINE_WORK_WIDTH].is_number()) return false;
        SetWorkAreaWidth(j[MACHINE_WORK_WIDTH]);

		if (!j[MACHINE_WORK_HEIGHT].is_number()) return false;
        SetWorkAreaHeight(j[MACHINE_WORK_HEIGHT]);

		if (!j[MACHINE_RAPID_SPEED].is_number()) return false;
        SetRapidSpeed(j[MACHINE_RAPID_SPEED]);

		if (!j[MACHINE_ACCELEREATION].is_number()) return false;
        SetAcceleration(j[MACHINE_ACCELEREATION]);

		if (!j[MACHINE_UNITS].is_boolean()) return false;
        SetUnits(j[MACHINE_UNITS]);

		if (!j[MACHINE_RESTORE_ORIGIN].is_boolean()) return false;
        SetRestoreOrigin(j[MACHINE_RESTORE_ORIGIN]);

		if (!j[MACHINE_RESTORE_TYPE].is_boolean()) return false;
        SetRestoreType(j[MACHINE_RESTORE_TYPE]);

		if (!j[MACHINE_PROBE_CMDS].is_string()) return false;
        SetProbeCmds(j[MACHINE_PROBE_CMDS]);

		if (!j[MACHINE_SAFE_POS_CMDS].is_string()) return false;
        SetSafePositionCmds(j[MACHINE_SAFE_POS_CMDS]);

		if (!j[MACHINE_USER_COMMAND_1].is_string()) return false;
        SetUserCmd1(j[MACHINE_USER_COMMAND_1]);

		if (!j[MACHINE_USER_COMMAND_2].is_string()) return false;
        SetUserCmd2(j[MACHINE_USER_COMMAND_2]);

		if (!j[MACHINE_USER_COMMAND_3].is_string()) return false;
        SetUserCmd3(j[MACHINE_USER_COMMAND_3]);

		if (!j[MACHINE_USER_COMMAND_4].is_string()) return false;
        SetUserCmd4(j[MACHINE_USER_COMMAND_4]);

        return true;
    }

    // User Command
    const string MachineSettings::MACHINE_QUERY_PERIOD = "query_period";
	const string MachineSettings::MACHINE_SPINDLE_MIN = "spindle_min";
	const string MachineSettings::MACHINE_SPINDLE_MAX = "spindle_max";
	const string MachineSettings::MACHINE_WORK_WIDTH = "work_width";
	const string MachineSettings::MACHINE_WORK_HEIGHT = "work_height";
	const string MachineSettings::MACHINE_RAPID_SPEED = "rapid_speed";
	const string MachineSettings::MACHINE_ACCELEREATION = "acceleration";
	const string MachineSettings::MACHINE_UNITS = "units";
	const string MachineSettings::MACHINE_RESTORE_ORIGIN = "restore_origin";
	const string MachineSettings::MACHINE_RESTORE_TYPE = "restore_type";
	const string MachineSettings::MACHINE_PROBE_CMDS = "probe_cmds";
	const string MachineSettings::MACHINE_SAFE_POS_CMDS = "safe_pos_cmds";
	const string MachineSettings::MACHINE_USER_COMMAND_1 = "user_command_1";
	const string MachineSettings::MACHINE_USER_COMMAND_2 = "user_command_2";
	const string MachineSettings::MACHINE_USER_COMMAND_3 = "user_command_3";
	const string MachineSettings::MACHINE_USER_COMMAND_4 = "user_command_4";
}
