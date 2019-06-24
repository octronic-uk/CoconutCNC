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

#pragma once

#include <string>
#include "JsonSerialization.h"
using std::string;

namespace Coconut
{
	class MachineSettings : public JsonSerialization
	{
	public:
		MachineSettings();

		int GetQueryPeriod() const;
		void SetQueryPeriod(int queryPeriod);

		bool GetUnits() const;
		void SetUnits(bool units);

		float GetRapidSpeed() const;
		void SetRapidSpeed(float rapidSpeed);

		float GetAcceleration() const;
		void SetAcceleration(float acceleration);

		int GetSpindleMin() const;
		void SetSpindleMin(int spindleMin);

		int GetSpindleMax() const;
		void SetSpindleMax(int spindleMax);

		string GetProbeCmds() const;
		void SetProbeCmds(const string& probeCmds);

		string GetSafePositionCmds() const;
		void SetSafePositionCmds(const string& safePositionCmds);

		bool GetRestoreOrigin() const;
		void SetRestoreOrigin(bool restoreOrigin);

		bool GetRestoreType() const;
		void SetRestoreType(bool restoreType);

		string GetUserCmd1() const;
		void SetUserCmd1(const string& userCmd1);

		string GetUserCmd2() const;
		void SetUserCmd2(const string& userCmd2);

		string GetUserCmd3() const;
		void SetUserCmd3(const string& userCmd3);

		string GetUserCmd4() const;
		void SetUserCmd4(const string& userCmd4);

		int GetWorkAreaWidth() const;
		void SetWorkAreaWidth(int workAreaWidth);

		int GetWorkAreaHeight() const;
		void SetWorkAreaHeight(int workAreaHeight);

		// JsonSerialization interface
        json ToJson() override;
        bool FromJson(const json& j) override;
	private:
		int mQueryPeriod;
		int mSpindleMin;
		int mSpindleMax;
     	int mWorkAreaWidth;
		int mWorkAreaHeight;
     	float mRapidSpeed;
		float mAcceleration;
		bool mUnits;
        bool mRestoreOrigin;
		bool mRestoreType;
		string mProbeCmds;
		string mSafePositionCmds;
		string mUserCmd1;
		string mUserCmd2;
		string mUserCmd3;
		string mUserCmd4;

        // User Command
		const static string MACHINE_QUERY_PERIOD;
		const static string MACHINE_SPINDLE_MIN;
		const static string MACHINE_SPINDLE_MAX;
		const static string MACHINE_WORK_WIDTH;
		const static string MACHINE_WORK_HEIGHT;
		const static string MACHINE_RAPID_SPEED;
		const static string MACHINE_ACCELEREATION;
		const static string MACHINE_UNITS;
		const static string MACHINE_RESTORE_ORIGIN;
		const static string MACHINE_RESTORE_TYPE;
		const static string MACHINE_PROBE_CMDS;
		const static string MACHINE_SAFE_POS_CMDS;
		const static string MACHINE_USER_COMMAND_1;
		const static string MACHINE_USER_COMMAND_2;
		const static string MACHINE_USER_COMMAND_3;
		const static string MACHINE_USER_COMMAND_4;
	};
}
