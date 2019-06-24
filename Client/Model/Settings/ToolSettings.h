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
#include <vector>

#include "JsonSerialization.h"
#include "Cylinder.h"

using std::vector;
using std::string;

namespace Coconut
{
	class ToolSettings : public JsonSerialization
	{
	public:
		ToolSettings(string name="Unnamed Tool", int id=-1, int tool_number = -1,
			float length = 1, float diameter=1, int faces = 6);

        int  GetID() const;
		void SetID(int id);

        string GetName() const;
        void   SetName(const string& name);

        int  GetToolNumber() const;
        void SetToolNumber(int num);

        int GetToolHolderID() const;
        void SetToolHolderID(int toolHolderID);

		float GetLength() const;
		void  SetLength(float height);

       	int GetFaces() const;
		void SetFaces(int faces);

        float GetDiameter() const;
		void  SetDiameter(float diameter);


        void AddCylinder(const Cylinder& c);

        // JsonSerialization interface
        json ToJson() override;
        bool FromJson(const json& j) override;

        vector<Cylinder>& GetCylindersVector();

    private:
        int    mID;
        string mName;
        int    mToolNumber;
        int    mToolHolderID;
		float  mDiameter;
		int    mFaces;

        vector<Cylinder> mCylinders;

		const static string TOOL_ID;
		const static string TOOL_NAME;
		const static string TOOL_NUMBER;
		const static string TOOL_HOLDER_ID;
		const static string TOOL_DIAMETER;
        const static string TOOL_FACES;
        const static string TOOL_CYLINDERS;
    };
}
