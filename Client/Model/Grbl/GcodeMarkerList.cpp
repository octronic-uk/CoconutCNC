/*
 * GCodeMarkerList.cpp
 *
 * Created: 14 2018 by Ashley
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

#include "GCodeMarkerList.h"
#include "GCodeCommand.h"
#include <regex>
#include <algorithm>

using std::regex;
using std::replace;

namespace Coconut
{
	GCodeMarkerList::GCodeMarkerList()
	{

	}

	GCodeMarkerList::~GCodeMarkerList()
	{

	}

	int GCodeMarkerList::rowCount() const
	{
		return mData.size();
	}

	string GCodeMarkerList::data(int index) const
	{
	   return removeCommentChars(mData.at(index)->getMarker());
	}

	string GCodeMarkerList::removeCommentChars(string marker) const
	{
	   marker = regex_replace(marker, regex("\\)"), "");
       marker = regex_replace(marker, regex("\\("), "");
       marker = regex_replace(marker, regex(";")  , "");
       return marker;
	}

	void GCodeMarkerList::setMarkers(deque<GCodeCommand*> data)
	{
	   mData = data;
	   // TODO - Leak Spin
	   GCodeCommand *start, *end;
	   start = new GCodeCommand("",0);
	   start->setMarker("Program Start");
	   start->setState(GcodeCommandState::Marker);
	   end = new GCodeCommand("",INT_MAX);
	   end->setMarker("Program End");
	   end->setState(GcodeCommandState::Marker);

	   mData.push_front(start);
	   mData.push_back(end);
	}

	GCodeCommand* GCodeMarkerList::at(int index)
	{
		return mData.at(index);
	}

	void GCodeMarkerList::clear()
	{
	   mData.clear();
	}
}
