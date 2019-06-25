/*
 * MachineState.h
 *
 * Created: 22 2018 by Ashley
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

#pragma once

#include <string>
#include "../../Common/Util.h"

using std::string;

namespace Coconut
{

	enum class GrblMachineState
	{
		Unknown = 0,
		Idle,
		Alarm,
		Run,
		Home,
		Hold,
		Queue,
		Check,
		Door,
		Locked,
		Unlocked,
		Jog,
		Error
	};

	static string stateToString(GrblMachineState state)
	{
		switch (state)
		{
			case GrblMachineState::Error:
				return "Error";
			case GrblMachineState::Unknown:
				return "Unknown";
			case GrblMachineState::Idle:
				return "Idle";
			case GrblMachineState::Alarm:
				return "Alarm";
			case GrblMachineState::Run:
				return "Run";
			case GrblMachineState::Home:
				return "Home";
			case GrblMachineState::Hold:
				return "Hold";
			case GrblMachineState::Queue:
				return "Queue";
			case GrblMachineState::Check:
				return "Check";
			case GrblMachineState::Door:
				return "Door";
			case GrblMachineState::Locked:
				return "Locked";
			case GrblMachineState::Unlocked:
				return "Unlocked";
			case GrblMachineState::Jog:
				return "Jog";
		}
		return "Unknown";
	}

	static GrblMachineState stateFromString(string stateStr)
	{
		if (Util::to_upper(stateStr) == "IDLE")
		{
			return GrblMachineState::Idle;
		}
		else if (Util::to_upper(stateStr) == "ALARM")
		{
			return GrblMachineState::Alarm;
		}
		else if (Util::to_upper(stateStr) == "RUN")
		{
			return GrblMachineState::Run;
		}
		else if (Util::to_upper(stateStr) == "HOME")
		{
			return GrblMachineState::Home;
		}
		else if (Util::to_upper(stateStr) == "HOLD")
		{
			return GrblMachineState::Hold;
		}
		else if (Util::to_upper(stateStr) == "QUEUE")
		{
			return GrblMachineState::Queue;
		}
		else if (Util::to_upper(stateStr) == "CHECK")
		{
			return GrblMachineState::Check;
		}
		else if (Util::to_upper(stateStr) == "DOOR")
		{
			return GrblMachineState::Door;
		}
		else if (Util::to_upper(stateStr) == "LOCKED")
		{
			return GrblMachineState::Locked;
		}
		else if (Util::to_upper(stateStr) == "UNLOCKED")
		{
			return GrblMachineState::Unlocked;
		}
		else if (Util::to_upper(stateStr) == "JOG")
		{
			return GrblMachineState::Jog;
		}
		else if (Util::to_upper(stateStr) == "ERROR")
		{
			return GrblMachineState::Error;
		}

		return GrblMachineState::Unknown;
	}
}
