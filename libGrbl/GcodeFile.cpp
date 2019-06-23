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


#include "GcodeFile.h"
#include "GCodeCommandNotFoundException.h"
#include <iostream>

using std::cout;
using std::endl;

namespace Coconut
{
	GcodeFileModel::GcodeFileModel()
		: mProgramLoading(false),
		  mFileChanged(false),
		  mFileOpen(false)
	{
		cout << "GcodeFileModel: Constructing" << endl;
		mGcodeParser = new GcodeParser(this);
	}

	GcodeFileModel::~GcodeFileModel()
	{
		cout << "GcodeFileModel: Destructing" << endl;
		for (GCodeCommand* item : mData)
		{
			delete item;
		}
        delete mGcodeParser;
		mData.clear();
		mProgramLoading = false;
		mFileChanged = false;
	}

	void GcodeFileModel::load(vector<string> data)
	{
		cout << "GcodeFileModel: load(vector<string>)" << endl;

		// Reset tables
		emit gcodeFileLoadStartedSignal();
		// Prepare parser
		mGcodeParser->setTraverseSpeed(1);//mSettingsForm->rapidSpeed());
		//if (mCodeDrawer->getIgnoreZ()) gp.reset(QVector3D(qQNaN(), qQNaN(), 0));

		// Block parser updates on table changes
		mProgramLoading = true;

		// Prepare model
		cout << "GcodeFileModel: Clearing data";
		mData.clear();
		emit clearExistingGcodeFileSignal();
		emit reserveGcodeRowsSignal(data.count());


		// TODO - This should probably be moved to GcodeParser

		int index = 0;
		while (!data.isEmpty())
		{
			string command;
			string trimmed;
			vector<string> args;
			GCodeCommand* item = new GCodeCommand();
			command = data.takeFirst();
			trimmed = command.trimmed();
			item->setCommand(command);

			cout << "GcodeFileModel: Next Line" << command;
			if (!trimmed.isEmpty())
			{
				item->setLine(mGcodeParser->getCommandNumber());
				item->setTableIndex(index);

				mGcodeParser->addCommand(item);

				if (item->getCommand() == "%")
				{
					cout << "GcodeFileModel: Skipping % at index " << index;
					continue;
				}

				if (item->getArgs().isEmpty())
				{
					string marker = GcodeParser::parseComment(command);
					cout << "GcodeFileModel: marker " << marker;
					item->setMarker(marker);
					item->setState(GcodeCommandState::Marker);
					if (!item->getMarker().isEmpty())
					{
						mMarkers.append(item);
					}
				}
				else
				{
					item->setCommand(trimmed + '\r');
					item->setState(GcodeCommandState::InQueue);
				}

				mData.append(item);
				index++;
			}
		}
		mProgramLoading = false;
		printMarkers();
		emit gcodeFileLoadFinishedSignal(this);
		emit gcodeParserUpdatedSignal(mGcodeParser.data());
		mFileOpen = true;
	}

	void GcodeFileModel::load(string fileName)
	{
		cout << "GcodeFileModel: load(string fileName)";
		mFile.setFileName(fileName);

		if (!mFile.open(QIODevice::ReadOnly))
		{
			emit statusBarUpdateSignal(string(tr("Can't open file") + fileName));
			return;
		}

		// Prepare text stream
		QTextStream textStream(&mFile);

		// Read lines
		vector<string> data;
		while (!textStream.atEnd())
		{
			data.append(textStream.readLine());
		}

		mFile.close();
		load(data);
	}

	QTime GcodeFileModel::updateProgramEstimatedTime(vector<LineSegment*> lines)
	{

		Q_UNUSED(lines)
		//cout << "GcodeFileModel: updateProgramEstimatedTime(vector<LineSegment*> lines)";
		/*
		double time = 0;

		for (int i = 0; i < lines.count(); i++) {
			LineSegment *ls = lines[i];
		//    foreach (LineSegment *ls, lines) {
			double length = (ls->getEnd() - ls->getStart()).length();

			if (!qIsNaN(length) && !qIsNaN(ls->getSpeed()) && ls->getSpeed() != 0) time +=
					length / ((mUi->chkFeedOverride->isChecked() && !ls->isFastTraverse())
							  ? (ls->getSpeed() * mUi->txtFeed->value() / 100) : ls->getSpeed());

	//        //cout << "GcodeFileModel: length/time:" << length << ((mUi->chkFeedOverride->isChecked() && !ls->isFastTraverse())
	//                                                 ? (ls->getSpeed() * mUi->txtFeed->value() / 100) : ls->getSpeed())
	//                 << time;

	//        if (qIsNaN(length)) //cout << "GcodeFileModel: length nan:" << i << ls->getLineNumber() << ls->getStart() << ls->getEnd();
	//        if (qIsNaN(ls->getSpeed())) //cout << "GcodeFileModel: speed nan:" << ls->getSpeed();
		}

		time *= 60;

		QTime t;

		t.setHMS(0, 0, 0);
		t = t.addSecs(time);

		mUi->glwVisualizer->setSpendTime(QTime(0, 0, 0));
		mUi->glwVisualizer->setEstimatedTime(t);

		return t;
		*/
		return QTime();
	}

	string GcodeFileModel::getCurrentFileName()
	{
	   cout << "GcodeFileModel: getCurrentFileName()";
	   return mFile.fileName();
	}

	GCodeCommand* GcodeFileModel::getCommandByID(long id) const
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

	GCodeCommand* GcodeFileModel::getCommand(int index) const
	{
		if (index < 0 || index > mData.count() -1)
		{
			throw GCodeCommandNotFoundException(index);
		}

		return mData.at(index);
	}

	int GcodeFileModel::countCommands()
	{
		return mData.count();
	}

	vector<GCodeCommand*> GcodeFileModel::getData() const
	{
		return mData;
	}

	vector<GCodeCommand*> GcodeFileModel::getMarkers() const
	{
		return mMarkers;
	}

	bool GcodeFileModel::isOpen()
	{
	   return mFileOpen;
	}

	void GcodeFileModel::printMarkers()
	{
	   cout << "GcodeFileModel: Markers";
	   for (GCodeCommand* marker : mMarkers)
	   {
		   cout << "GcodeFileModel: " << marker->getTableIndex()
					<< ":" << marker->getMarker();
	   }
	}


	bool GcodeFileModel::save(string fileName, GcodeTableModel *model)
	{
		Q_UNUSED(fileName)
		Q_UNUSED(model)
		cout << "GcodeFileModel:save(string, GcodeTableModel)";

		QFile file(fileName);
		QDir dir;

		cout << "GcodeFileModel: Saving program";

		if (file.exists())
		{
			dir.remove(file.fileName());
		}

		if (!file.open(QIODevice::WriteOnly))
		{
			return false;
		}

		QTextStream textStream(&file);

		for (int i = 0; i < model->rowCount() - 1; i++)
		{
			textStream << model->data(model->index(i, 1)).toString() << "\r\n";
		}

		file.close();
		return true;
	}


	bool GcodeFileModel::isGcodeFile(string fileName)
	{
		//cout << "GcodeTableModel: isGcodeFile" << fileName;
		return fileName.endsWith(".txt", Qt::CaseInsensitive)
				|| fileName.endsWith(".nc", Qt::CaseInsensitive)
				|| fileName.endsWith(".ncc", Qt::CaseInsensitive)
				|| fileName.endsWith(".ngc", Qt::CaseInsensitive)
				|| fileName.endsWith(".tap", Qt::CaseInsensitive);
	}


	bool GcodeFileModel::hasFileChanged()
	{
		cout << "GcodeTableModel: hasFileChanged";
		return mFileChanged;
	}
}
