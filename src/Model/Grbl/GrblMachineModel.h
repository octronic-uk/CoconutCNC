/*
 * SerialPortModel.h
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

#include <map>
#include <thread>
#include <sstream>
#include <glm/vec3.hpp>

#include "GrblMachineState.h"
#include "GrblConfigurationModel.h"
#include "GCodeFileModel.h"
#include "GCodeCommand.h"
#include "GrblResponse.h"
#include "../../Common/Time.h"

using std::stringstream;
using std::thread;
using std::deque;
using std::map;
using std::string;
using glm::vec3;

namespace Coconut
{
    class AppState;
	class GrblMachineModel
	{
	public:
		GrblMachineModel(AppState* state);
		~GrblMachineModel();

		void ClearState();
        void StartWorkThread();
        void JoinWorkThread();
        void WorkFunction();

		int  GrblBufferLengthInUse();
		bool SendNextCommand();

		void ClearGrblCommandBuffer();

		vec3  GetMachinePosition();
		float GetMachinePositionX();
		float GetMachinePositionY();
		float GetMachinePositionZ();

		vec3  GetWorkPosition();
		float GetWorkPositionX();
		float GetWorkPositionY();
		float GetWorkPositionZ();

		bool GetProgramRunning() const;
		void SetProgramRunning(bool programRunning);

		void SendProgram();
		void SendManualGCodeCommand(const GCodeCommand& cmd);
		void WriteManualGCodeCommands();

		void UpdateRapidOverride(float rate);


        GrblConfigurationModel& GetConfigurationModel();
        bool IsWorkThreadRunning();
        string GetStateAsString();
        GrblMachineState GetState();

        float GetFeedRate() const;
        void SetFeedRate(int);
        int GetSpindleSpeed() const;
        void SetSpindleSpeed(int);
        int GetToolNumber() const;

		float GetPercentCompleted();
        float GetPercentGrblBufferUsed();

    protected: // Member Functions
        void AppendCommandToConsole(const GCodeCommand& command);
        void AppendResponseToConsole(const GrblResponse& command);

		GCodeCommand FeedOverride(const GCodeCommand& command, double overridePercent);

		bool IsSpaceInGrblBuffer(const GCodeCommand& cmd);

		void UpdateWorkPosition();
		void UpdateStatus(GrblResponse response);
		void UpdateOverrides(const GrblResponse& data);
		void UpdateWorkCoordinateOffset(const GrblResponse& data);
		void UpdateMachinePosition(const GrblResponse& data);
		void UpdateFeedRateAndSpindleSpeed(const GrblResponse& response);

		void ProcessResponse(const GrblResponse& data);
		void ParseError(const GrblResponse& error);
		void ParseGrblVersion(const GrblResponse& response);
		void ParseConfigurationResponse(GrblResponse response);
		void ParseAlarmResponse(const GrblResponse& response);
        void RequestStatus();
        void ReadFromGrbl();

		static string StateToString(GrblMachineState state);
		const static map<int,string> ERROR_STRINGS;
		const static map<int,string> ALARM_STRINGS;

	private: // Members
		const static int BUFFER_LENGTH_LIMIT;
        AppState* mAppState;
        // Reading from serial
        vector<string> mLinesRead;
        stringstream mCurrentLine;

        // Local State
		float mBufferUsedPercentage;
        int mProcessedCommandsCount;
		int mCommandQueueInitialSize;
        bool mStatusRequested;
		bool mProgramRunning;
        bool mGotStartupMessage;
        int mProgramIndex;
        long mLastCommandSendTime;

        // Grbl State
		vector<GCodeCommand> mManualCommandQueue;
		vector<GCodeCommand> mGrblCommandBuffer;
        GrblConfigurationModel mConfigurationModel;
		GrblMachineState mState;
		GrblMachineState mLastState;
		vec3 mMachinePosition;
		vec3 mWorkPosition;
		vec3 mWorkCoordinateOffset;
		float mFeedOverride;
		float mSpindleOverride;
		float mRapidOverride;
        int mSpindleSpeed;
        int mFeedRate;
        int mToolNumber;
		int mErrorCode;
		string mErrorString;
		string mGrblVersion;

        thread mWorkThread;
        bool mWorkThreadRunning;
        long mStatusRecievedTime;
	};
}
