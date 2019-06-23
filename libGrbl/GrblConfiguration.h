/*
 * GrblConfigurationModel.h
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

#pragma once

#include <vector>
#include <string>

using std::string;
using std::vector;

namespace Coconut
{
    class GrblConfigurationEntry
    {
        public:

        int ID;
    	string Value;
        string Description;

    	GrblConfigurationEntry(int id, string value, string desc) :
        ID(id), Value(value), Description(desc) {}
    };

	class GrblConfiguration
	{
	public:
		GrblConfiguration();

		int RowCount() const;
		GrblConfigurationEntry Data(int index) const;
		void SetData(int row, const GrblConfigurationEntry& value);
		vector<GrblConfigurationEntry>& GetData();

    protected:
        void SetupData();

	private:
		vector<GrblConfigurationEntry> mData;
	};
}
