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
#include <glm/vec3.hpp>
#include "JsonSerialization.h"

using glm::vec3;
using std::string;

namespace Coconut
{
	class MachineSettings : public JsonSerialization
	{
	public:
		MachineSettings();

		int  GetStatusQueryInterval() const;
  		int* GetStatusQueryIntervalPtr();
		void SetStatusQueryInterval(float queryInterval);

        int  GetProgramSendInterval() const;
        int* GetProgramSendIntervalPtr();
        void SetProgramSendInterval(int psInterval);

		string GetProbeCmds() const;
		void SetProbeCmds(const string& probeCmds);

		string GetSafePositionCmds() const;
		void SetSafePositionCmds(const string& safePositionCmds);

		vec3 GetWorkArea() const;
        float* GetWorkAreaArray();
		void SetWorkArea(const vec3& y);

		// JsonSerialization interface
        json ToJson() override;
        bool FromJson(const json& j) override;

	private:
		int mStatusQueryInterval;
		int mProgramSendInterval;
     	vec3 mWorkArea;
		string mProbeCmds;
		string mSafePositionCmds;

        // User Command
		const static string MACHINE_STATUS_QUERY_INTERVAL;
		const static string MACHINE_PROGRAM_SEND_INTERVAL;
		const static string MACHINE_WORK_AREA;
		const static string MACHINE_PROBE_CMDS;
		const static string MACHINE_SAFE_POS_CMDS;
	};
}
