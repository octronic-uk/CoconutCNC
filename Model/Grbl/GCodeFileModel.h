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
#include "GCodeCommand.h"
#include "../../Common/File.h"

using std::string;
using std::vector;
using std::deque;

namespace Coconut
{
	class LineSegment;

	class GCodeFileModel
	{
	public:
		GCodeFileModel();
		~GCodeFileModel();

		void Initialise();
		void Load(const deque<string>& data);
		void Load(const string& fileName);
		bool IsGcodeFile(const string& fileName);
		bool HasFileChanged();
		void SetFileChanged(bool changed);
		double UpdateProgramEstimatedTime(const vector<LineSegment>& lines);
		string GetCurrentFileName();
		GCodeCommand& GetCommand(int index);
		GCodeCommand& GetCommandByID(long );
		int CountCommands();
		vector<GCodeCommand>& GetData();
		vector<GCodeCommand>& GetMarkers();
		bool IsOpen();

	protected:
		void PrintMarkers();

	private:
		bool mProgramLoading;
		File mFile;
		bool mFileOpen;
		vector<GCodeCommand> mData;
		GCodeParser mGcodeParser;
		vector<GCodeCommand> mMarkers;


	};
}
