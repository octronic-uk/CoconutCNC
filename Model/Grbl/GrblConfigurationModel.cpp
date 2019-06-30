/*
 * GrblConfiguration.cpp
 *
 * Created: 18 2018 by Ashley
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
#include "GrblConfigurationModel.h"
#include "../../Common/Logger.h"

namespace Coconut
{
	GrblConfigurationModel::GrblConfigurationModel(AppState* state)
        : mAppState(state)
	{
	}

    string GrblConfigurationModel::GetValue(int v)
    {
        info("GrblConfigurationModel: Get value of param {}",v);
        return mValues[v];
    }

    void GrblConfigurationModel::SetValue(int i, string v)
    {
        info("GrblConfigurationModel: Set value of param {} = {}",i,v);
        mValues[i] = v;
    }

	map<int,string>& GrblConfigurationModel::GetValues()
	{
	   return mValues;
	}

    const map<int,string> GrblConfigurationModel::ConfigurationKeys =
    {
		{0,   "Step pulse, (µs)"},
		{1,   "Step idle delay, (ms)"},
		{2,   "Step port invert (bit-mask)"},
		{3,   "Direction port invert (bit-mask)"},
		{4,   "Step enable invert (boolean)"},
		{5,   "Limit pins invert (boolean)"},
		{6,   "Probe pin invert (boolean)"},
		{10,  "Status report (bit-mask)"},
		{11,  "Junction deviation (mm)"},
		{12,  "Arc tolerance (mm)"},
		{13,  "Report inches (boolean)"},
		{20,  "Soft limits (boolean)"},
		{21,  "Hard limits (boolean)"},
		{22,  "Homing cycle (boolean)"},
		{23,  "Homing dir invert (bit-mask)"},
		{24,  "Homing feed (mm/min)"},
		{25,  "Homing seek (mm/min)"},
		{26,  "Homing debounce (ms)"},
		{27,  "Homing pull-off (mm)"},
		{30,  "Max spindle speed (RPM)"},
		{31,  "Min spindle speed (RPM)"},
		{32,  "Laser mode (boolean)"},
		{100, "X (steps/mm)"},
		{101, "Y (steps/mm)"},
		{102, "Z (steps/mm)"},
		{110, "X Max rate (mm/min)"},
		{111, "Y Max rate (mm/min)"},
		{112, "Z Max rate (mm/min)"},
		{120, "X Acceleration (mm/sec^2)"},
		{121, "Y Acceleration (mm/sec^2)"},
		{122, "Z Acceleration (mm/sec^2)"},
		{130, "X Max travel (mm)"},
		{131, "Y Max travel (mm)"},
		{132, "Z Max travel (mm)"}
	};
}
