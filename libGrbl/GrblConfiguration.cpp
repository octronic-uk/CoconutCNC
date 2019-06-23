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
#include "GrblConfiguration.h"

namespace Coconut
{
	GrblConfiguration::GrblConfiguration()
	{
	}

	void GrblConfiguration::SetupData()
	{
		mData.push_back(GrblConfigurationEntry(0,  "Step pulse, (µs)",""));
		mData.push_back(GrblConfigurationEntry(1,  "Step idle delay, (ms)",""));
		mData.push_back(GrblConfigurationEntry(2,  "Step port invert (bit-mask)",""));
		mData.push_back(GrblConfigurationEntry(3,  "Direction port invert (bit-mask)",""));
		mData.push_back(GrblConfigurationEntry(4,  "Step enable invert (boolean)",""));
		mData.push_back(GrblConfigurationEntry(5,  "Limit pins invert (boolean)",""));
		mData.push_back(GrblConfigurationEntry(6,  "Probe pin invert (boolean)",""));
		mData.push_back(GrblConfigurationEntry(10, "Status report (bit-mask)",""));
		mData.push_back(GrblConfigurationEntry(11, "Junction deviation (mm)",""));
		mData.push_back(GrblConfigurationEntry(12, "Arc tolerance (mm)",""));
		mData.push_back(GrblConfigurationEntry(13, "Report inches (boolean)",""));
		mData.push_back(GrblConfigurationEntry(20, "Soft limits (boolean)",""));
		mData.push_back(GrblConfigurationEntry(21, "Hard limits (boolean)",""));
		mData.push_back(GrblConfigurationEntry(22, "Homing cycle (boolean)",""));
		mData.push_back(GrblConfigurationEntry(23, "Homing dir invert (bit-mask)",""));
		mData.push_back(GrblConfigurationEntry(24, "Homing feed (mm/min)",""));
		mData.push_back(GrblConfigurationEntry(25, "Homing seek (mm/min)",""));
		mData.push_back(GrblConfigurationEntry(26, "Homing debounce (ms)",""));
		mData.push_back(GrblConfigurationEntry(27, "Homing pull-off (mm)",""));
		mData.push_back(GrblConfigurationEntry(30, "Max spindle speed (RPM)",""));
		mData.push_back(GrblConfigurationEntry(31, "Min spindle speed (RPM)",""));
		mData.push_back(GrblConfigurationEntry(32, "Laser mode (boolean)",""));
		mData.push_back(GrblConfigurationEntry(100,"X (steps/mm)",""));
		mData.push_back(GrblConfigurationEntry(101,"Y (steps/mm)",""));
		mData.push_back(GrblConfigurationEntry(102,"Z (steps/mm)",""));
		mData.push_back(GrblConfigurationEntry(110,"X Max rate (mm/min)",""));
		mData.push_back(GrblConfigurationEntry(111,"Y Max rate (mm/min)",""));
		mData.push_back(GrblConfigurationEntry(112,"Z Max rate (mm/min)",""));
		mData.push_back(GrblConfigurationEntry(120,"X Acceleration (mm/sec^2)",""));
		mData.push_back(GrblConfigurationEntry(121,"Y Acceleration (mm/sec^2)",""));
		mData.push_back(GrblConfigurationEntry(122,"Z Acceleration (mm/sec^2)",""));
		mData.push_back(GrblConfigurationEntry(130,"X Max travel (mm)",""));
		mData.push_back(GrblConfigurationEntry(131,"Y Max travel (mm)",""));
		mData.push_back(GrblConfigurationEntry(132,"Z Max travel (mm)",""));
	}

	int GrblConfiguration::RowCount() const
	{
		return mData.size();
	}

	GrblConfigurationEntry GrblConfiguration::Data(int index) const
	{
        for(GrblConfigurationEntry d : mData)
        {
            if (d.ID == index) return d;
        }
        return GrblConfigurationEntry(-1,"","");
	}

	void GrblConfiguration::SetData(int index, const GrblConfigurationEntry& value)
	{
        for(GrblConfigurationEntry d : mData)
        {
            if (d.ID == index)
            {
                d.Value = value.Value;
            }
        }

	}

	vector<GrblConfigurationEntry>& GrblConfiguration::GetData()
	{
	   return mData;
	}
}
