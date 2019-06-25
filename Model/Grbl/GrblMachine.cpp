/*
 * GrblMachineModel.cpp
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

#include <glm/vec3.hpp>

#include "GrblMachine.h"
#include "GrblMachineState.h"
#include <regex>
#include "../../Common/Logger.h"

using glm::vec3;

using std::regex;
using std::smatch;
using std::regex_match;

namespace Coconut
{

	GrblMachineModel::GrblMachineModel()
		: mState(GrblMachineState::Unknown),
		  mLastState(GrblMachineState::Unknown),
		  mMachinePosition(vec3(0.0)),
		  mWorkPosition(vec3(0.0,0.0,0.0)),
		  mWorkCoordinateOffset(vec3(0.0,0.0,0.0)),
		  mProgramSendInterval(1000/10),
		  mStatusInterval(1000/5),
		  mCountProcessedCommands(0),
		  mCommandQueueInitialSize(0),
		  mFeedOverride(100),
		  mSpindleOverride(100),
		  mRapidOverride(100),
		  mError(false),
		  mErrorCode(-1),
		  mBytesWaiting(0),
		  mStatusRequested(false),
		  mWaitingForStatus(false),
		  mProgramRunning(false),
		  mToolChangeWaiting(false),
		  mFeedRate(0),
		  mSpindleSpeed(0)
	{
        /*
		// Setup timer
		connect(
			&mProgramSendTimer, SIGNAL(timeout()),
			this, SLOT(onProgramSendTimerTimeout())
		);
		connect(
			&mStatusTimer, SIGNAL(timeout()),
			this, SLOT(onStatusTimerTimeout())
		);

		startProgramSendTimer();
		startStatusTimer();
        */
	}

	GrblMachineModel::~GrblMachineModel()
	{
	}

	void GrblMachineModel::OnConnect()
	{
	}

	void GrblMachineModel::UpdateMachinePosition(const GrblResponse& response)
	{
		// Update machine coordinates
		string data = response.GetData();
        smatch m;
		static regex machinePositionExpression("MPos:([^,]*),([^,]*),([^,|]*)");

        if (regex_match(data,m,machinePositionExpression))
        {
            mMachinePosition.x = stod(m[1]);
			mMachinePosition.y = stod(m[2]);
            mMachinePosition.z = stod(m[3]);
        }
	}

	void GrblMachineModel::UpdateFeedRateAndSpindleSpeed(const GrblResponse& response)
	{
		static regex feedSpeedRegex("\\|FS:(\\d+),(\\d+)\\|");
        smatch m;
        string data = response.GetData();
        if (regex_match(data,m,feedSpeedRegex))
        {
          mFeedRate = stod(m[1]);
          mSpindleSpeed = stod(m[1]);
        }
	}

	void GrblMachineModel::UpdateWorkCoordinateOffset(const GrblResponse& resp)
	{
		// Update work coordinates
		static regex workCoordExpression("WCO:([^,]*),([^,]*),([^,^>]*)");
        smatch m;
        string data = resp.GetData();
		if (regex_match(data,m,workCoordExpression))
		{
			mWorkCoordinateOffset.x = stod(m[1]);
			mWorkCoordinateOffset.y = stod(m[2]);
			mWorkCoordinateOffset.z = stod(m[3]);
		}
	}

	/*
		Ov:100,100,100
		indicates current override values in percent of programmed values for
		feed,
		rapids,
		and spindle speed,
		respectively.
	*/
	void GrblMachineModel::UpdateOverrides(const GrblResponse& resp)
	{
		static regex overridesRegex("Ov:(\\d+),(\\d+),(\\d+)");
        smatch m;
        string data = resp.GetData();

		if (regex_match(data,m,overridesRegex))
		{
		   mFeedOverride = stod(m[1]);
		   mRapidOverride = stod(m[2]);
		   mSpindleOverride = stod(m[3]);

		   debug(
		   	"GrblMachineController: Got overrides: F{} R{} S{}",
		   	mFeedOverride, mRapidOverride, mSpindleOverride);
		}
	}

	void GrblMachineModel::UpdateWorkPosition()
	{
		mWorkPosition.x = (mMachinePosition.x - mWorkCoordinateOffset.x);
		mWorkPosition.y = (mMachinePosition.y - mWorkCoordinateOffset.y);
		mWorkPosition.z = (mMachinePosition.z - mWorkCoordinateOffset.z);
	}

	void GrblMachineModel::ParseGrblVersion(const GrblResponse& response)
	{
		static regex grblVersionRegex("Grbl ([\\w.]+)");
        smatch m;
        string data = response.GetData();

		if (regex_search(data,m,grblVersionRegex))
		{
			mGrblVersion = m[1];
		}
	}

	void GrblMachineModel::ProcessResponse(const GrblResponse& response)
	{
		debug("GrblMachineModel: Process Response {}" , response.GetData());

		mLastState = mState;
		GCodeCommand* next = nullptr;

		switch (response.GetType())
		{
			case GrblResponseType::Alarm:
				ParseAlarmResponse(response);
				break;
			case GrblResponseType::Startup:
				ParseGrblVersion(response);
				mStatusRequested = false;
				mWaitingForStatus = false;
				mState = GrblMachineState::Locked;
				break;
			case GrblResponseType::Locked:
				mState = GrblMachineState::Locked;
				//emit appendResponseToConsoleSignal(response);
				break;
			case GrblResponseType::Unlocked:
				mError = false;
				mState = GrblMachineState::Unlocked;
				OnGcodeCommandManualSend(GCodeCommand::GetFirmwareConfigurationCommand());
				break;
			case GrblResponseType::Status:
				if (!mError)
				{
					UpdateStatus(response);
					UpdateMachinePosition(response);
					UpdateWorkCoordinateOffset(response);
					UpdateOverrides(response);
					UpdateFeedRateAndSpindleSpeed(response);
					UpdateWorkPosition();
					debug("GrblMachineModel: Got status!");
					mStatusRequested = false;
					mWaitingForStatus = false;
				}
				break;
			case GrblResponseType::Ok:
				if (mError)
				{
					mState = GrblMachineState::Unlocked;
					mError = false;
					mProgramRunning = false;
					mStatusRequested = false;
					mWaitingForStatus = false;
					StartProgramSendTimer();
				}
				else if (!mCommandBuffer.empty())
				{
					next = mCommandBuffer.at(0);
					debug("GrblMachineModel: Popping command: ",
						"id {} | line {} | cmd {} | in Queue {} | in buffer {}",
                        next->GetID(),
					    next->GetLine(),
					    next->GetCommand(),
					    mCommandQueue.size(),
						mCommandBuffer.size());

					next->SetResponse(response);
					next->SetState(GcodeCommandState::Processed);
					mCountProcessedCommands++;
					//emit updateProgramTableStatusSignal(next);
					//emit setCompletionProgressSignal(GetProcessedPercent());

					if (next->IsM30Command())
					{
						//emit setCompletionProgressSignal(100);
						//emit jobCompletedSignal();
						mProgramRunning = false;

					}
				}

				// DO NOT ELSE IF THIS.
				// We Need to check again to see if that was the last
				// command in the Queue/Buffer and the program has finished.
				if (mProgramRunning && mCommandQueue.empty() && mCommandBuffer.empty())
				{
					//qDebug() << "GrblMachineModel: Program Finished";
					//emit setCompletionProgressSignal(100);
					//emit jobCompletedSignal();
					mProgramRunning = false;
				}

				//emit appendResponseToConsoleSignal(response);

				break;
			case GrblResponseType::Error:
				ParseError(response);
				mError = true;
				mWaitingForStatus = false;
				mStatusRequested = false;
				ClearCommandQueue();
				ClearCommandBuffer();
				mState = GrblMachineState::Error;
				//emit errorSignal(mErrorString);
				//emit appendResponseToConsoleSignal(response);
				break;
			case GrblResponseType::Configuration:
				ParseConfigurationResponse(response);
				break;
		};

		if (mState != mLastState)
		{
			//emit machineStateUpdatedSignal(mState);
		}
	}

	void GrblMachineModel::ParseError(const GrblResponse& error)
	{
		if (!mError)
		{
			string trimmedError = Util::trim_copy(error.GetData());
			static regex errorNumRegex("error:(\\d+)$");
			static regex errorStrRegex("error:(.+)$");
            smatch num_m, str_m;

			if (regex_match(trimmedError, num_m, errorNumRegex))
			{
				mErrorCode = stoi(num_m[1]);
				mErrorString = ERROR_STRINGS.at(mErrorCode);
			}
			else if (regex_match(trimmedError,str_m, errorStrRegex))
			{
				mErrorString = str_m[1];
			}
		}
	}

	int GrblMachineModel::GetProcessedPercent()
	{
		if (mCommandQueue.size() > 0)
        {
			return static_cast<int>((static_cast<double>(mCountProcessedCommands)/ static_cast<double>(mCommandQueueInitialSize))* 100);
        }
		else
        {
			return 0;
        }
	}

	void GrblMachineModel::OnSerialPortReadyRead()
	{
		//qDebug() << "GrblMachineModel: onSerialPortReadyRead";
        /*
		while (mSerialPort.canReadLine())
		{
			string data = mSerialPort.readLine().trimmed();
			//qDebug() << "GrblMachineModel: Got line from serial port" << data;
			GrblResponse response(data);
			processResponse(response);
		}
        */
	}

	void GrblMachineModel::OnSerialPortError(/*QSerialPort::SerialPortError error*/)
	{
        /*
		static QSerialPort::SerialPortError previousError;

		if (error == QSerialPort::TimeoutError)
		{
			//qDebug() << "GrblMachineModel: serial wait timeout";
			return;
		}

		if (error != QSerialPort::NoError && error != previousError)
		{
			//qDebug() << "GrblMachineModel: onSerialPortError" << error;
			previousError = error;
			if (mSerialPort.isOpen())
			{
				mSerialPort.close();
			}
			//emit serialPortErrorSignal(tr("Serial port error ") + string::number(error) + ": " + mSerialPort.errorString());
		}
        */
	}

	void GrblMachineModel::OnSerialPortNameChanged(string port)
	{
        /*
	   setPortName(port);
	   closePort();
       */
	}

	void GrblMachineModel::OnSerialPortBaudRateChanged(int baud)
	{
        /*
	   setBaudRate(baud);
	   closePort();
       */
	}

	void GrblMachineModel::OnProgramSendTimerTimeout()
	{
	//    qDebug () << "GrblMachineModel: onProgramSendTimerTimout";

		if (mError)
		{
			mStatusRequested = false;
			mWaitingForStatus = false;
		}

		if (mStatusRequested)
		{
			if (mWaitingForStatus && !mError)
			{
				//qDebug() << "GrblMachineModel: Still waiting for last status request to respond";
				return;
			}

            /*
			else if (mSerialPort.isOpen() && mBytesWaiting == 0 && !mError)
			{
				qDebug() << "GrblMachineModel: Requesting status";
				mBytesWaiting += mSerialPort.write(GCodeCommand::StatusUpdateCommand()->GetCommand().toLatin1());
				mStatusRequested = true;
				mWaitingForStatus = true;
			}
            */
		}
		else if (!mWaitingForStatus)
		{
			SendNextCommandFromQueue();
		}
	}

	void GrblMachineModel::OnStatusTimerTimeout()
	{
		if (!mWaitingForStatus)
		{
			mStatusRequested = true;
		}
	}


	void GrblMachineModel::Init()
	{
		mCommandBuffer.clear();
		mCommandQueue.clear();
		mCommandQueueInitialSize = -1;
		mFeedOverride = 100;
		mSpindleOverride = 100;
		mRapidOverride = 100;
	}

	bool GrblMachineModel::SendNextCommandFromQueue()
	{
		if (mCommandQueue.empty())
		{
			return false;
		}

        /*
		if (!mSerialPort.isOpen())
		{
			qDebug() << "GrblMachineModel: sendCommand FAILED -> SerialPort is not open";
			//mProgramSendTimer.stop();
			return false;
		}
        */

		if (mToolChangeWaiting)
		{
			debug("GrblMachineModel: Tool change waiting...");
			return false;
		}

		// Serial port has been flushed
		if (mBytesWaiting == 0)
		{
			while (!mError && !mCommandQueue.empty())
			{
				 GCodeCommand* command = mCommandQueue.at(0);

				 if (command->IsMarker() ||  // Is Marker or
					(command->GetCommand().empty() &&  // No Command
					 command->GetRawCommand() == 0))
				 {
					 command = mCommandQueue.at(0);
                     mCommandQueue.pop_front();
					 command->SetState(GcodeCommandState::Skipped);
					 continue;
				 }

				if (!IsSpaceInBuffer(command))
				{
					debug("GrblMachineController: Buffer full, waiting... {}", command->GetCommand());
					break;
				}

				// Take the command off the Queue for processing
				command = mCommandQueue.at(0);
                mCommandQueue.pop_front();

				// Don't send this becaue GRBL doesn't respond/care and buffer Get
				// misaligned
				if (command->IsToolChangeCommand())
				{
					mToolChangeWaiting = true;
					//emit toolChangeSignal(command->GetToolNumber());
					command->SetState(GcodeCommandState::Processed);
					break;
				}
				else
				{
					mCommandBuffer.push_back(command);
					debug("GrblMachineModel: Writing", command->GetCommand());
					//mBytesWaiting += mSerialPort.write(command->GetCommand().toLatin1());
					command->SetState(GcodeCommandState::Sent);
				}
				//emit appendCommandToConsoleSignal(command);
			}
			return true;
		}
		return false;
	}

	void GrblMachineModel::OnSerialBytesWritten(uint64_t bytes)
	{
		mBytesWaiting -= bytes;
		//qDebug() << "GrblMachineModel: Serial bytes Written:" << bytes << "/ Remaining:" << mBytesWaiting;
	}

	bool GrblMachineModel::GetProgramRunning() const
	{
		return mProgramRunning;
	}

	void GrblMachineModel::SetProgramRunning(bool programRunning)
	{
		mProgramRunning = programRunning;
	}

	void GrblMachineModel::OnSendProgram(GCodeFileModel* gcodeFile)
	{
		debug("GrblMachineModel: onSendProgram()");

		ClearCommandBuffer();
		ClearCommandQueue();
		mCountProcessedCommands = 0;

		int index = 0;
		for (GCodeCommand* next : gcodeFile->GetData())
		{
			QueueCommand(next);
			index++;
		}

		mCommandQueueInitialSize = mCommandQueue.size();
		mProgramRunning = true;
	}

	void GrblMachineModel::OnSendProgramFromLine(GCodeFileModel* gcodeFile, long id)
	{
		//qDebug() << "GrblMachineModel: onSendProgramFromLine @ id" << id;

		ClearCommandBuffer();
		ClearCommandQueue();
		mCountProcessedCommands = 0;

		int index = gcodeFile->GetCommandByID(id)->GetLine();
		int size = gcodeFile->GetData().size();

		for (; index < size; index++)
		{
			QueueCommand(gcodeFile->GetData()[index]);
		}

		mCommandQueueInitialSize = mCommandQueue.size();
		mProgramRunning = true;
	}

	void GrblMachineModel::StartStatusTimer()
	{
		/*if (!mStatusTimer.isActive())
		{
			mStatusTimer.start(mStatusInterval);
		}
        */
	}

	void GrblMachineModel::StartProgramSendTimer()
	{
		/*if (!mProgramSendTimer.isActive())
		{
			qDebug() << "GrblMachineController: Starting status timer";
			mProgramSendTimer.start(mProgramSendInterval);
		}
        */
	}

	void GrblMachineModel::StopProgramSendTimer()
	{
		debug("GrblMachineController: Stopping program send timer");
		//mProgramSendTimer.stop();
		ClearCommandBuffer();
		ClearCommandQueue();
	}

	void GrblMachineModel::StopStatusTimer()
	{
		debug("GrblMachineController: Stopping status timer");
		//mStatusTimer.stop();
	}

	void GrblMachineModel::UpdateStatus(GrblResponse response)
	{
		/*qDebug() << "GrblMachineModel: Updating status from response"
				 << response.GetData();
				 */

		static regex statusRegex("<([^|<]\\w+)[:\\|]");
        smatch m;
        string data = response.GetData();


		if (regex_match(data,m,statusRegex))
		{
			string stateStr = m[1];
			//qDebug() << "GrblMachineModel: parsing state from" << stateStr;
			mLastState = mState;
			mState = stateFromString(stateStr);
			if (mState != mLastState)
			{
				//emit machineStateUpdatedSignal(mState);
			}
		}
	}

	void GrblMachineModel::ParseConfigurationResponse(GrblResponse response)
	{
	   static regex configRegex("\\$(\\d+)=(\\S*)");
       smatch m;
	   int param = -1;
       string data = response.GetData();
	   string value = "";

	   if (regex_match(data,m,configRegex))
	   {
		  param = stoi(m[1]);
		  value = m[2];
		  debug("GrblMachineModel: Got configuration parameter {}-->{}", param, value);
		  mFirmwareConfiguration[param] = value;
		  //emit firmwareConfigurationReadSignal(param,value);
	   }
	}

	void GrblMachineModel::ParseAlarmResponse(const GrblResponse& response)
	{
		static regex alarmRegex("ALARM:(\\d+)");
        smatch m;
        string data = response.GetData();
		if (regex_match(data, m, alarmRegex))
		{
		   int alarmNum = stoi(m[1]);
		   //emit alarmSignal(ALARM_STRINGS.at(alarmNum));
		   return;
		}
		//emit alarmSignal("Unknown Alarm");
	}

	void GrblMachineModel::ClearCommandQueue()
	{
		mCommandQueue.clear();
		mCountProcessedCommands = 0;
		mCommandQueueInitialSize = 0;
	}

	void GrblMachineModel::ClearCommandBuffer()
	{
		mCommandBuffer.clear();
	}

	int GrblMachineModel::CommandsQueueLength()
	{
		return mCommandQueue.size();
	}

	void GrblMachineModel::OnGcodeCommandManualSend(GCodeCommand* command)
	{
        /*
		if (mSerialPort.isOpen())
		{
			if (command->GetRawCommand() > 0)
			{
				qDebug() << "GrblMachineController: Manual Raw Gcode Send 0x"
						 << string::number(command->GetRawCommand(),16).toUpper();
				char c = command->GetRawCommand();
				mBytesWaiting += mSerialPort.write(&c,1);
			}
			else
			{
				qDebug() << "GrblMachineController: Manual ASCII Gcode Send" << command->GetCommand();
				mBytesWaiting += mSerialPort.write(command->GetCommand().toLatin1());
			}
			//emit appendCommandToConsoleSignal(command);
		}
        */
	}

	void GrblMachineModel::OnUpdateRapidOverride(float rate)
	{

	}

	void GrblMachineModel::OnToolChangeCompleted()
	{
		mToolChangeWaiting = false;
	}

	void GrblMachineModel::OnUpdateSpindleOverride(float speed)
	{

	}

	void GrblMachineModel::OnUpdateFeedOverride(float rate)
	{

	}

	void GrblMachineModel::QueueCommand(GCodeCommand* command)
	{
		mCommandQueue.push_back(command);
	}

	int GrblMachineModel::BufferLengthInUse()
	{
		int length = 0;

		//qDebug() << "GrblMachineModel: Commands in buffer:";
		for (GCodeCommand* gc : mCommandBuffer)
		{
			/*qDebug() << "\t" << gc->GetLine()
					 << "|" << gc->GetCommand()
					 << "|" << gc->GetArgs();
					 */

			length += gc->GetCommandLength();
		}

		//qDebug() << "GrblMachineModel: Buffer in use:" << length;
		return length;
	}

	bool GrblMachineModel::IsSpaceInBuffer(GCodeCommand* cmd)
	{
		int bufferLeft = BUFFER_LENGTH_LIMIT - BufferLengthInUse();
		int bufferUsed = BUFFER_LENGTH_LIMIT - bufferLeft;
		/*qDebug() << "GrblMachineModel: Buffer Left" << bufferLeft
				 << "cmd length" << cmd->GetCommandLength();
				 */

		//emit setBufferProgressSignal(((float)bufferUsed/BUFFER_LENGTH_LIMIT)*100);
		return bufferLeft >= cmd->GetCommandLength();
	}

	GCodeCommand GrblMachineModel::FeedOverride(GCodeCommand* command, double overridePercent)
	{
		return GCodeParser::overrideSpeed(command,overridePercent);
	}

	vec3 GrblMachineModel::GetMachinePosition()
	{
		return mMachinePosition;
	}

	float GrblMachineModel::GetMachinePositionX()
	{
		return mMachinePosition.x;
	}

	float GrblMachineModel::GetMachinePositionY()
	{
		return mMachinePosition.y;
	}

	float GrblMachineModel::GetMachinePositionZ()
	{
		return mMachinePosition.z;
	}

	vec3 GrblMachineModel::GetWorkPosition()
	{
		return mWorkPosition;
	}

	float GrblMachineModel::GetWorkPositionX()
	{
		return mWorkPosition.x;
	}

	float GrblMachineModel::GetWorkPositionY()
	{
		return mWorkPosition.y;
	}

	float GrblMachineModel::GetWorkPositionZ()
	{
		return mWorkPosition.z;
	}

	string GrblMachineModel::StateToString(GrblMachineState state)
	{
		switch (state)
		{
			case GrblMachineState::Idle:
				return string("Idle");
			case GrblMachineState::Alarm:
				return string("Alarm");
			case GrblMachineState::Run:
				return string("Running");
			case GrblMachineState::Home:
				return string("Home");
			case GrblMachineState::Hold:
				return string("Hold");
			case GrblMachineState::Queue:
				return string("Queue");
			case GrblMachineState::Check:
				return string("Check");
			case GrblMachineState::Door:
				return string("Door");
			case GrblMachineState::Locked:
				return string("Locked");
			case GrblMachineState::Unlocked:
				return string("Unlocked");
			case GrblMachineState::Jog:
				return string("Jog");
            case GrblMachineState::Error:
                return string("Error");
			case GrblMachineState::Unknown:
				break;
		}

		return string("Unknown");
	}

	const int GrblMachineModel::BUFFER_LENGTH_LIMIT = 127;

	const map<int,string> GrblMachineModel::ALARM_STRINGS =
	{
		{
			1,
			"Hard limit triggered.\n"
			"Machine position is likely lost due to sudden and immediate halt. Re-homing is highly recommended."
		},
		{
			2,
			"G-code motion tarGet exceeds machine travel.\n"
			"Machine position safely retained. Alarm may be unlocked."
		},
		{
			3,
			"Reset while in motion.\n"
			"Grbl cannot guarantee position. Lost steps are likely. Re-homing is highly recommended."
		},
		{
			4,
			"Probe fail.\n"
			"The probe is not in the expected initial state before starting probe cycle, where G38.2 and G38.3 is not triggered and G38.4 and G38.5 is triggered."
		},
		{
			5,
			"Probe fail.\n"
			"Probe did not contact the workpiece within the programmed travel for G38.2 and G38.4."
		},
		{
			6,
			"Homing fail.\n"
			"Reset during active homing cycle."
		},
		{
			7
			,"Homing fail.\n"
			"Safety door was opened during active homing cycle."
		},
		{
			8,
			"Homing fail.\n"
			"Cycle failed to clear limit switch when pulling off. Try increasing pull-off setting or check wiring."
		},
		{   9,
			"Homing fail.\n"
			"Could not find limit switch within search distance. Defined as 1.5 * max_travel on search and 5 * pulloff on locate phases."
		}
	};

	const map<int,string> GrblMachineModel::ERROR_STRINGS =
	{
		{
			1,
			string("G-code words consist of a letter and a value. Letter was not found.")
		},
		{
			2,
			string("Numeric value format is not valid or missing an expected value.")
		},
		{
			3,
			string("Grbl '$' system command was not recognized or supported.")
		},
		{
			4,
			string("Negative value received for an expected positive value.")
		},
		{
			5,
			string("Homing cycle is not enabled via settings.")
		},
		{
			6,
			string("Minimum step pulse time must be greater than 3usec")
		},
		{
			7,
			string("EEPROM read failed. Reset and restored to default values.")
		},
		{
			8,
			string("Grbl '$' command cannot be used unless Grbl is IDLE. Ensures smooth operation during a job.")
		},
		{
			9,
			string("G-code locked out during alarm or jog state")
		},
		{
			10,
			string("Soft limits cannot be enabled without homing also enabled.")
		},
		{
			11,
			string("Max characters per line exceeded. Line was not processed and executed.")
		},
		{
			12,
			string("(Compile Option) Grbl '$' setting value exceeds the maximum step rate supported.")
		},
		{
			13,
			string("Safety door detected as opened and door state initiated.")
		},
		{
			14,
			string("(Grbl-Mega Only) Build info or startup line exceeded EEPROM line length limit.")
		},
		{
			15,
			string("Jog tarGet exceeds machine travel. Command ignored.")
		},
		{
			16,
			string("Jog command with no '=' or contains prohibited g-code.")
		},
		{
			17,
			string("Laser mode requires PWM output.")
		},
		{
			20,
			string("Unsupported or invalid g-code command found in block.")
		},
		{
			21,
			string("More than one g-code command from same modal group found in block.")
		},
		{
			22,
			string("Feed rate has not yet been set or is undefined.")
		},
		{
			23,
			string("G-code command in block requires an integer value.")
		},
		{
			24,
			string("Two G-code commands that both require the use of the XYZ axis words were detected in the block.")
		},
		{
			25,
			string("A G-code word was repeated in the block.")
		},
		{
			26,
			string("A G-code command implicitly or explicitly requires XYZ axis words in the block, but none were detected.")
		},
		{
			27,
			string("N line number value is not within the valid range of 1 - 9,999,999.")
		},
		{
			28,
			string("A G-code command was sent, but is missing some required P or L value words in the line.")
		},
		{
			29,
			string("Grbl supports six work coordinate systems G54-G59. G59.1, G59.2, and G59.3 are not supported.")
		},
		{
			30,
			string("The G53 G-code command requires either a G0 seek or G1 feed motion mode to be active. A different motion was active.")
		},
		{
			31,
			string("There are unused axis words in the block and G80 motion mode cancel is active.")
		},
		{
			32,
			string("A G2 or G3 arc was commanded but there are no XYZ axis words in the selected plane to trace the arc.")
		},
		{
			33,
			string("The motion command has an invalid tarGet. G2, G3, and G38.2 generates this error, if the arc is impossible to generate or if the probe tarGet is the current position.")
		},
		{
			34,
			string("A G2 or G3 arc, traced with the radius definition, had a mathematical error when computing the arc geometry. Try either breaking up the arc into semi-circles or quadrants, or redefine them with the arc offset definition.")
		},
		{
			35,
			string("A G2 or G3 arc, traced with the offset definition, is missing the IJK offset word in the selected plane to trace the arc.")
		},
		{
			36,
			string("There are unused, leftover G-code words that aren't used by any command in the block.")
		},
		{
			37,
			string("The G43.1 dynamic tool length offset command cannot apply an offset to an axis other than its configured axis. The Grbl default axis is the Z-axis.")
		}
	};

}
