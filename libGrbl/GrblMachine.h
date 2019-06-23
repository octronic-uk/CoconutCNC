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

#include <glm/vec3.hpp>
#include <map>
#include "GrblMachineState.h"
#include "GcodeFileModel.h"
#include "GCodeCommand.h"
#include "GrblResponse.h"


using std::string;
using glm::vec3;

namespace Coconut
{

	class GrblMachineModel :
	{
	public:
		GrblMachineModel();
		~GrblMachineModel();

		void Init();

		bool OpenSerialPort();
		bool CloseSerialPort();
		bool IsSerialPortOpen();
		int  GetSerialBaudRate();
		string GetSerialPortName();
		void setPortName(string name);
		void setBaudRate(int baud);
		void SerialPortErrorSignal(string errorMessage);
		void SetupSerialPort();

		int  BufferLengthInUse();
		bool SendNextCommandFromQueue();
		int  CommandsQueueLength();

		void RestoreOffsets(GrblMachineState& state);
		void StoreOffsets(GrblMachineState& state);
		void RestoreParserState();
		void ClearCommandQueue();
		void ClearCommandBuffer();

		vec3  GetMachinePosition();
		float GetMachinePositionX();
		float GetMachinePositionY();
		float GetMachinePositionZ();

		vec3  GetWorkPosition();
		float GetWorkPositionX();
		float GetWorkPositionY();
		float GetWorkPositionZ();

		void   QueueCommand(GCodeCommand* command);
		void   SetSettingsModelHandle(SqlSettingsModel* settingsModelHandle);

		bool GetProgramRunning() const;
		void SetProgramRunning(bool programRunning);

	signals:
		void UpdateProgramTableStatusSignal(GCodeCommand* state);
		void UpdateFeedOverrideSignal(float);
		void UpdateRapidOverrideSignal(float);
		void UpdateSpindleOverrideSignal(float);
		void UpdateMachinePositionSignal(const vec3);
		void UpdateWorkPositionSignal(const vec3);
		void UpdateWCOSignal(const vec3);
		void StatusBarUpdateSignal(string status);

		void ToolPositionChangedSignal(vec3);
		void MachineStateUpdatedSignal(const GrblMachineState&);
		void JobCompletedSignal();
		void AlarmSignal(string);

		void AppendResponseToConsoleSignal(const GrblResponse&);
		void AppendCommandToConsoleSignal(GCodeCommand*);
		void CommandResponseSignal(const GrblResponse&);
		void SetCompletionProgressSignal(int);
		void SetBufferProgressSignal(int);
		void ErrorSignal(string);
		void MachineConnectedSigal(bool);

		void FirmwareConfigurationReadSignal(int,string);
		void ToolChangeSignal(int);

		void FeedRateChangedSignal(int);
		void SpindleSpeedChangedSignal(int);


	public slots:
		void OnSendProgram(GcodeFileModel* gcodeFile);
		void OnSendProgramFromLine(GcodeFileModel* gcodeFile, long fromId);
		void OnGcodeCommandManualSend(GCodeCommand*);

		void OnUpdateSpindleOverride(float speed);
		void OnUpdateFeedOverride(float rate);
		void OnUpdateRapidOverride(float rate);
		void OnToolChangeCompleted();

	private slots:
		void OnConnect();
		void OnSerialPortReadyRead();
		void OnSerialPortError(QSerialPort::SerialPortError);
		void OnSerialPortNameChanged(string);
		void OnSerialPortBaudRateChanged(int);
		void OnProgramSendTimerTimeout();
		void OnStatusTimerTimeout();
		void OnSerialBytesWritten(qint64 bytes);

	private: // Members
		const static int BUFFER_LENGTH_LIMIT;
		QSerialPort mSerialPort;
		vector<GCodeCommand*> mCommandBuffer;
		vector<GCodeCommand*> mCommandQueue;
		GrblMachineState mState;
		GrblMachineState mLastState;
		vec3 mMachinePosition;
		vec3 mWorkPosition;
		vec3 mWorkCoordinateOffset;
		SqlSettingsModel* mSettingsModelHandle;
		QTimer mProgramSendTimer;
		QTimer mStatusTimer;
		int mProgramSendInterval;
		int mStatusInterval;
		int mCountProcessedCommands;
		int mCommandQueueInitialSize;
		float mFeedOverride;
		float mSpindleOverride;
		float mRapidOverride;
		bool mError;
		int mErrorCode;
		string mErrorString;
		string mGrblVersion;
		uint64_t mBytesWaiting;
		bool mStatusRequested;
		bool mWaitingForStatus;
		map<int,string> mFirmwareConfiguration;
		bool mProgramRunning;
		bool mToolChangeWaiting;
		int mFeedRate;
		int mSpindleSpeed;

	private: // Member Functions
		GCodeCommand FeedOverride(GCodeCommand* command, double overridePercent);
		GCodeCommand GetNextCommand(GcodeFileModel& gcodeFile);

		bool IsSpaceInBuffer(GCodeCommand* cmd);
		int  GetProcessedPercent();

		void StartProgramSendTimer();
		void StopProgramSendTimer();

		void StartStatusTimer();
		void StopStatusTimer();

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

		static string StateToString(GrblMachineState state);
		const static map<int,string> ERROR_STRINGS;
		const static map<int,string> ALARM_STRINGS;
	};
}
