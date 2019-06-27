/*
 * FileModel.cpp
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


#include "GCodeFileModel.h"
#include "GCodeParser.h"
#include "GCodeCommandNotFoundException.h"
#include <iostream>
#include "../../Common/Util.h"
#include "../../Common/Logger.h"

using std::cout;
using std::endl;

namespace Coconut
{
	GCodeFileModel::GCodeFileModel()
		: mProgramLoading(false),
		  mFileOpen(false)
	{
		info("GCodeFileModel: Constructing");
	}

	GCodeFileModel::~GCodeFileModel()
	{
		info("GCodeFileModel: Destructing");
		mData.clear();
		mProgramLoading = false;
	}

	void GCodeFileModel::Load(const deque<string>& data)
	{
		info("GCodeFileModel: load(vector<string>)");

		// Reset tables
		//emit gcodeFileLoadStartedSignal();
		// Prepare parser
		mGcodeParser.SetTraverseSpeed(1);//mSettingsForm->rapidSpeed());
		//if (mCodeDrawer->getIgnoreZ()) gp.reset(QVector3D(qQNaN(), qQNaN(), 0));

		// Block parser updates on table changes
		mProgramLoading = true;

		// Prepare model
		debug("GCodeFileModel: Clearing data");
		mData.clear();
		//emit clearExistingGcodeFileSignal();
		//emit reserveGcodeRowsSignal(data.count());


		// TODO - This should probably be moved to GcodeParser

		int index = 0;
		while (!data.empty())
		{
			string command;
			string trimmed;
			vector<string> args;
			GCodeCommand item;
			command = data.front();
			trimmed = Util::trim_copy(command);
			item.SetCommand(command);

			debug("GCodeFileModel: Next Line {}", command);
			if (!trimmed.empty())
			{
				item.SetLine(mGcodeParser.GetCommandNumber());
				item.SetTableIndex(index);

				mGcodeParser.AddCommand(item);

				if (item.GetCommand() == "%")
				{
					debug("GCodeFileModel: Skipping % at index ", index);
					continue;
				}

				if (item.GetArgs().empty())
				{
					string marker = GCodeParser::ParseComment(command);
					debug("GCodeFileModel: marker ", marker);
					item.SetMarker(marker);
					item.SetState(GcodeCommandState::Marker);
					if (!item.GetMarker().empty())
					{
						mMarkers.push_back(item);
					}
				}
				else
				{
					item.SetCommand(trimmed + '\r');
					item.SetState(GcodeCommandState::InQueue);
				}

				mData.push_back(item);
				index++;
			}
		}
		mProgramLoading = false;
		PrintMarkers();
		//emit gcodeFileLoadFinishedSignal(this);
		//emit gcodeParserUpdatedSignal(mGcodeParser.data());
		mFileOpen = true;
	}

	void GCodeFileModel::Load(const string& fileName)
	{
		info("GCodeFileModel: load(string fileName = {})",fileName);
		mFile.SetPath(fileName);

		// Read lines
		vector<string> data_vec = mFile.ReadAsLines();
		deque<string> data;
        data.insert(data.begin(), data_vec.begin(), data_vec.end());

		Load(data);
	}

	double GCodeFileModel::UpdateProgramEstimatedTime(const vector<LineSegment>& lines)
	{

		//cout << "GCodeFileModel: updateProgramEstimatedTime(vector<LineSegment*> lines)";
		/*
		double time = 0;

		for (int i = 0; i < lines.count(); i++) {
			LineSegment *ls = lines[i];
		//    foreach (LineSegment *ls, lines) {
			double length = (ls->getEnd() - ls->getStart()).length();

			if (!qIsNaN(length) && !qIsNaN(ls->getSpeed()) && ls->getSpeed() != 0) time +=
					length / ((mUi->chkFeedOverride->isChecked() && !ls->isFastTraverse())
							  ? (ls->getSpeed() * mUi->txtFeed->value() / 100) : ls->getSpeed());

	//        //cout << "GCodeFileModel: length/time:" << length << ((mUi->chkFeedOverride->isChecked() && !ls->isFastTraverse())
	//                                                 ? (ls->getSpeed() * mUi->txtFeed->value() / 100) : ls->getSpeed())
	//                 << time;

	//        if (qIsNaN(length)) //cout << "GCodeFileModel: length nan:" << i << ls->getLineNumber() << ls->getStart() << ls->getEnd();
	//        if (qIsNaN(ls->getSpeed())) //cout << "GCodeFileModel: speed nan:" << ls->getSpeed();
		}

		time *= 60;

		QTime t;

		t.setHMS(0, 0, 0);
		t = t.addSecs(time);

		mUi->glwVisualizer->setSpendTime(QTime(0, 0, 0));
		mUi->glwVisualizer->setEstimatedTime(t);

		return t;
		*/
		return 0.0;
	}

	string GCodeFileModel::GetCurrentFileName()
	{
	   debug("GCodeFileModel: GetCurrentFileName()");
	   return mFile.NameWithExtension();
	}

	GCodeCommand& GCodeFileModel::GetCommandByID(long id)
	{
	   for (GCodeCommand& next : mData)
	   {
		   if (next.HasID(id))
		   {
			   return next;
		   }
	   }
	   throw GCodeCommandNotFoundException(id);
	}

	GCodeCommand& GCodeFileModel::GetCommand(int index)
	{
		if (index < 0 || index > mData.size() -1)
		{
			throw GCodeCommandNotFoundException(index);
		}

		return mData.at(index);
	}

	int GCodeFileModel::CountCommands()
	{
		return mData.size();
	}

	vector<GCodeCommand>& GCodeFileModel::GetData()
	{
		return mData;
	}

	vector<GCodeCommand>& GCodeFileModel::GetMarkers()
	{
		return mMarkers;
	}

	bool GCodeFileModel::IsOpen()
	{
	   return mFileOpen;
	}

	void GCodeFileModel::PrintMarkers()
	{
	   debug("GCodeFileModel: Markers");
	   for (GCodeCommand& marker : mMarkers)
	   {
		   debug("GCodeFileModel: {} marker->GetMarker()", marker.GetTableIndex());
	   }
	}

	bool GCodeFileModel::IsGcodeFile(const string& fileName)
	{
        auto extStart = fileName.find_last_of(".");
        if (extStart != string::npos)
        {
            auto ext = fileName.substr(extStart+1);
			return ext == ".txt" || ext == ".gcode"|| ext == ".nc"
				|| ext == ".ncc" || ext == ".ngc" || ext == ".tap";
        }
        return false;
	}
}
