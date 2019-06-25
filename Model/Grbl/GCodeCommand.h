/*
 * GcodeCommand.h
 *
 * Created: 02 2018 by Ashley
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
 *
 *
 *
 * Every string Grbl receives is assumed to be a G-code block/line for it to
 * execute, except for some special system commands Grbl uses for configuration,
 * provide feedback to the user on what and how it's doing, or perform some task
 * such as a homing cycle. To see a list of these system commands, type $
 * followed by an enter, and Grbl will respond with:
 *
 * $$ (view Grbl Settings)
 * $# (view # parameters)
 * $G (view parser state)
 * $I (view build info)
 * $N (view startup blocks)
 * $x=value (save Grbl Setting)
 * $Nx=line (save startup block)
 * $C (check gcode mode)
 * $X (kill alarm lock)
 * $H (run homing cycle)
 * ~ (cycle start)
 * ! (feed hold)
 * ? (current status)
 * ctrl-x (reSet Grbl)
 *
 */

#pragma once

#include <string>
#include <vector>
#include "GrblResponse.h"

using std::string;
using std::vector;

namespace Coconut
{
	enum GcodeCommandState
	{
		None,
		Marker,
		InQueue,
		Sent,
		Processed,
		Skipped
	};


	class GCodeCommand
	{
	public:
		GCodeCommand(
			string cmd = "",
			int tableIndex = -1,
			bool showInConsole = true,
			int consoleIndex = -1,
			GcodeCommandState state = GcodeCommandState::InQueue
		);

		GCodeCommand(const GCodeCommand& other);
		GCodeCommand(const GCodeCommand* other);
		GCodeCommand(unsigned char rawCmd);

		static long ID;

		static GCodeCommand* JogCancelCommand();
		static GCodeCommand* AbsoluteXCommand(float);
		static GCodeCommand* AbsoluteYCommand(float);
		static GCodeCommand* AbsoluteZCommand(float);
		static GCodeCommand* ControlXCommand();
		static GCodeCommand* UnlockCommand();
		static GCodeCommand* StatusUpdateCommand();
		static GCodeCommand* ReSetCommand();
		static GCodeCommand* SpindleCounterClockwiseCommand();
		static GCodeCommand* SpindleClockwiseCommand();
		static GCodeCommand* SpindleStopCommand();
		static GCodeCommand* HomingCommand();
		static GCodeCommand* ZeroXYCommand();
		static GCodeCommand* GetGcodeParserParamsCommand();
		static GCodeCommand* ZeroZCommand();
		static GCodeCommand* CyclePauseResumeCommand();
		static GCodeCommand* FeedHoldCommand();
		static GCodeCommand* GetFirmwareConfigurationCommand();
		static GCodeCommand* SetFirmwareConfigurationCommand(int param, string value);
		static GCodeCommand* SetSafePositionCommand();
		static GCodeCommand* GoToSafePositionCommand();
		static GCodeCommand* GoToXYOriginCommand();
		static GCodeCommand* GoToZOriginCommand();

		static GCodeCommand* FeedOvDefault();
		static GCodeCommand* FeedOvPlusOne();
		static GCodeCommand* FeedOvPlusTen();
		static GCodeCommand* FeedOvMinusOne();
		static GCodeCommand* FeedOvMinusTen();

		static GCodeCommand* RapidOvDefault();
		static GCodeCommand* RapidOv50Percent();
		static GCodeCommand* RapidOv25Percent();

		static GCodeCommand* SpindleOvDefault();
		static GCodeCommand* SpindleOvPlusOne();
		static GCodeCommand* SpindleOvPlusTen();
		static GCodeCommand* SpindleOvMinusOne();
		static GCodeCommand* SpindleOvMinusTen();


		static GCodeCommand* JogCommand
		(
			double x, double y, double z,
			int feedRate, bool inches, bool machineCoordinates
		);

		bool operator==(const GCodeCommand& other);

		bool IsEmpty();

		int GetCommandLength() const;

		string GetCommand() const;
		void SetCommand(const string& command);

		int GetTableIndex() const;
		void SetTableIndex(int tableIndex);

		bool GetShowInConsole() const;
		void SetShowInConsole(bool showInConsole);

		int GetConsoleIndex() const;
		void SetConsoleIndex(int consoleIndex);

		GrblResponse GetResponse() const;
		void SetResponse(const GrblResponse& response);

		vector<string> GetArgs() const;
		void SetArgs(const vector<string>& args);

		GcodeCommandState GetState() const;
		void SetState(GcodeCommandState state);

		int GetLine() const;
		void SetLine(int line);

		bool isRawCommand();

		long GetID();
		bool HasID(long id);
		unsigned char GetRawCommand();

		string GetMarker() const;
		bool IsMarker() const;
		void SetMarker(const string marker);

		bool IsToolChangeCommand();
		string RemoveM6();
		int GetToolNumber();

		bool IsM30Command();

	private:
		static long nextID();
		unsigned char mRawCommand;
		string mCommand;
		GrblResponse mResponse;
		GcodeCommandState mState;
		vector<string> mArgs;
		long mID;
		int mLine;
		int mTableIndex;
		int mConsoleIndex;
		bool mShowInConsole;
		string mMarker;
	};

}
