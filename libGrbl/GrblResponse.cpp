/*
 * GrblResponse.cpp
 *
 * Created: 03 2018 by Ashley
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
#include "GrblResponse.h"

namespace Coconut
{

	// Default Constructor
	GrblResponse::GrblResponse() :
		mData(string()),
		mType(GrblResponseType::None) {}

	// Response Constructor
	GrblResponse::GrblResponse(string response) : mData(response)
	{
		IdentifyType();
	}

	// Copy Constructor
	GrblResponse::GrblResponse(const GrblResponse& other)
		: mData(other.mData), mType(other.mType) {}

	GrblResponseType GrblResponse::GetType() const
	{
	   return mType;
	}

	void GrblResponse::IdentifyType()
	{
		if (mData.find("Grbl")  == 0)
		{
			mType = GrblResponseType::Startup;
			return;
		}
		if (mData[0] == '<' && mData[mData.size()-1] == '>')
		{
			mType = GrblResponseType::Status;
			return;
		}
		if (mData.find("ok") != string::npos)
		{
			mType = GrblResponseType::Ok;
			return;
		}
		if (mData.find("error") != string::npos)
		{
			mType = GrblResponseType::Error;
			return;
		}
		if (mData.find("ALARM") != string::npos)
		{
			mType = GrblResponseType::Alarm;
			return;
		}
		if (mData.find("to unlock") != string::npos)
		{
			mType = GrblResponseType::Locked;
			return;
		}
		if (mData.find("HLP") != string::npos)
		{
			mType = GrblResponseType::Help;
			return;
		}
		if (mData.find("Unlocked") != string::npos)
		{
			mType = GrblResponseType::Unlocked;
			return;
		}
		if (mData.find("[Enabled]") != string::npos)
		{
			mType = GrblResponseType::Enabled;
			return;
		}
		if (mData.find("[Disabled]") != string::npos)
		{
			mType = GrblResponseType::Disabled;
			return;
		}
		if (mData.find("PRB") != string::npos)
		{
			mType = GrblResponseType::Probe;
			return;
		}
		if (mData[0] == '$' == 0 && mData.find("=") != string::npos)
		{
			mType = GrblResponseType::Configuration;
			return;
		}
		mType = GrblResponseType::ParserState;
	}

	string GrblResponse::GetData() const
	{
		return mData;
	}

}
