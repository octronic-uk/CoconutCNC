/*
 * FileModel.h
 *
 * Created: 24 2018 by Ashley
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

#include <fstream>
#include <string>
#include <vector>
#include <deque>

#include "GCodeParser.h"
#include "GCodeViewParser.h"
#include "GCodeCommand.h"
#include "LineSegment.h"
#include "../../Common/File.h"

using std::string;
using std::vector;
using std::deque;

namespace Coconut
{
	class AppState;
	class GCodeFileModel
	{
	public:
		GCodeFileModel(AppState* state);
		~GCodeFileModel();

		void ClearState();
		void Load(vector<string> data);
		void Load(const string& fileName);
		double UpdateProgramEstimatedTime(const vector<LineSegment>& lines);
		string GetCurrentFileName();
		GCodeCommand& GetCommand(int index);
		GCodeCommand& GetCommandByID(long );
		int CountCommands();
		vector<GCodeCommand>& GetData();
		vector<GCodeCommand>& GetMarkers();
        GCodeViewParser& GetGCodeViewParser();

	protected:
		void PrintMarkers();

	private:
        AppState* mAppState;
        File mFile;
		vector<GCodeCommand> mData;
		vector<GCodeCommand> mMarkers;
		GCodeParser mGCodeParser;
        GCodeViewParser mGCodeViewParser;
	};
}
