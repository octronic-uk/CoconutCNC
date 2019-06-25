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

#include "ToolHolderSettings.h"

#include <string>
#include "vector"

#include "JsonSerialization.h"
#include "Cylinder.h"

using std::vector;
using std::string;

namespace Coconut
{
	class ToolHolderSettings : public JsonSerialization
	{
	public:
		ToolHolderSettings(string name = "Unnamed ToolHolder", int id=-1);

		int GetID() const;
        void SetID(int id);

        string GetName() const;
        void SetName(const string& name);

        json ToJson() override;
        bool FromJson(const json& j) override;

        vector<Cylinder>& GetCylindersVector();

    private:
        int mID;
        string mName;
        vector<Cylinder> mCylinders;

        const static string TOOLHOLDER_ID;
        const static string TOOLHOLDER_NAME;
        const static string TOOLHOLDER_CYLINDERS;
	};
}
