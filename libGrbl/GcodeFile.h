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

#include <string>
#include <vector>

#include "GcodeTable.h"
#include "GcodeParser.h"
#include "GCodeCommand.h"

using std::string;
using std::vector;

namespace Coconut
{
	class LineSegment;

	class GcodeFileModel
	{
	public:
		GcodeFileModel();
		~GcodeFileModel();

		void initialise();
		void load(vector<string> data);
		void load(string fileName);
		bool save(string fileName, GcodeTableModel *model);
		bool isGcodeFile(string fileName);
		bool hasFileChanged();
		void setFileChanged(bool changed);
		QTime updateProgramEstimatedTime(vector<LineSegment*> lines);
		string getCurrentFileName();
		GCodeCommand* getCommand(int index) const;
		GCodeCommand* getCommandByID(long ) const;
		int countCommands();
		vector<GCodeCommand*> getData() const;
		vector<GCodeCommand*> getMarkers() const;
		bool isOpen();

		void statusBarUpdateSignal(string);
		void gcodeFileLoadStartedSignal();
		void gcodeFileLoadFinishedSignal(GcodeFileModel*);
		void nextGcodeLineReadySignal(GCodeCommand*);
		void clearExistingGcodeFileSignal();
		void reserveGcodeRowsSignal(int count);
		void gcodeParserUpdatedSignal(GcodeParser*);

	private:
		bool mProgramLoading;
		bool mFileChanged;
		QFile mFile;
		bool mFileOpen;
		vector<GCodeCommand*> mData;
		GcodeParser* mGcodeParser;
		vector<GCodeCommand*> mMarkers;

	private:
		void printMarkers();
	};
}
