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

using std::cout;
using std::endl;

namespace Coconut
{
	GCodeFileModel::GCodeFileModel()
		: mProgramLoading(false),
		  mFileOpen(false)
	{
		cout << "GCodeFileModel: Constructing" << endl;
	}

	GCodeFileModel::~GCodeFileModel()
	{
		cout << "GCodeFileModel: Destructing" << endl;
		for (GCodeCommand* item : mData)
		{
			delete item;
		}
		mData.clear();
		mProgramLoading = false;
	}

	void GCodeFileModel::Load(deque<string> data)
	{
		cout << "GCodeFileModel: load(vector<string>)" << endl;

		// Reset tables
		//emit gcodeFileLoadStartedSignal();
		// Prepare parser
		mGcodeParser.setTraverseSpeed(1);//mSettingsForm->rapidSpeed());
		//if (mCodeDrawer->getIgnoreZ()) gp.reset(QVector3D(qQNaN(), qQNaN(), 0));

		// Block parser updates on table changes
		mProgramLoading = true;

		// Prepare model
		cout << "GCodeFileModel: Clearing data";
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
			GCodeCommand* item = new GCodeCommand();
			command = data.front();
			trimmed = Util::trim_copy(command);
			item->setCommand(command);

			cout << "GCodeFileModel: Next Line" << command;
			if (!trimmed.empty())
			{
				item->setLine(mGcodeParser.getCommandNumber());
				item->setTableIndex(index);

				mGcodeParser.addCommand(item);

				if (item->getCommand() == "%")
				{
					cout << "GCodeFileModel: Skipping % at index " << index;
					continue;
				}

				if (item->getArgs().empty())
				{
					string marker = GCodeParser::parseComment(command);
					cout << "GCodeFileModel: marker " << marker;
					item->setMarker(marker);
					item->setState(GcodeCommandState::Marker);
					if (!item->getMarker().empty())
					{
						mMarkers.push_back(item);
					}
				}
				else
				{
					item->setCommand(trimmed + '\r');
					item->setState(GcodeCommandState::InQueue);
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

	void GCodeFileModel::Load(string fileName)
	{
		cout << "GCodeFileModel: load(string fileName)";
		mFile.SetPath(fileName);

		// Read lines
		vector<string> data_vec = mFile.ReadAsLines();
		deque<string> data;
        data.insert(data.begin(), data_vec.begin(), data_vec.end());

		Load(data);
	}

	double GCodeFileModel::UpdateProgramEstimatedTime(vector<LineSegment*> lines)
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
	   cout << "GCodeFileModel: getCurrentFileName()";
	   return mFile.NameWithExtension();
	}

	GCodeCommand* GCodeFileModel::GetCommandByID(long id) const
	{
	   for (GCodeCommand* next : mData)
	   {
		   if (next->hasID(id))
		   {
			   return next;
		   }
	   }
	   throw GCodeCommandNotFoundException(id);
	}

	GCodeCommand* GCodeFileModel::GetCommand(int index) const
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

	vector<GCodeCommand*> GCodeFileModel::GetData() const
	{
		return mData;
	}

	vector<GCodeCommand*> GCodeFileModel::GetMarkers() const
	{
		return mMarkers;
	}

	bool GCodeFileModel::IsOpen()
	{
	   return mFileOpen;
	}

	void GCodeFileModel::PrintMarkers()
	{
	   cout << "GCodeFileModel: Markers";
	   for (GCodeCommand* marker : mMarkers)
	   {
		   cout << "GCodeFileModel: " << marker->getTableIndex()
					<< ":" << marker->getMarker();
	   }
	}

	bool GCodeFileModel::IsGcodeFile(string fileName)
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
