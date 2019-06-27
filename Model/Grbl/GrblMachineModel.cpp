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
 *
 * Notes
 *
 * [1] A "Grbl Machine" is the thing powered by a Grbl-equipped Arduino, i.e.
 * 		CNC router, Laser, etc..
 *
 * [2] This model is designed to be run in a separate thread to the UI thread
 *     as it relies on blocking for status updates.
 *
 * [3] Grbl Machine Flow
 *
 *
 */

#include <glm/vec3.hpp>

#include "GrblMachineModel.h"
#include "GrblMachineState.h"
#include "GCodeFileModel.h"

#include "../../AppState.h"
#include "../../Common/Logger.h"

#include <regex>
#include <chrono>

using std::regex;
using std::smatch;
using std::regex_match;
using std::this_thread::yield;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

namespace Coconut
{
	GrblMachineModel::GrblMachineModel(AppState* state)
		: mAppState(state),
          mConfigurationModel(state),
          mWorkThreadRunning(false),
          mLastTime(0),
          mCurrentTime(0)
	{
        debug("GrblMachineModel: Constructor");
        ClearState();
	}

	GrblMachineModel::~GrblMachineModel()
	{
        debug("GrblMachineModel: Destructor");
	}

    void GrblMachineModel::StartWorkThread()
    {
        info("GrblMachineModel: {}",__FUNCTION__);
        mWorkThreadRunning = true;
        mWorkThread = thread(&GrblMachineModel::WorkFunction,this);
        info("GrblMachineModel: Work thread has started");
    }

    void GrblMachineModel::WorkFunction()
    {
		while(mWorkThreadRunning)
        {
            mCurrentTime = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();

            SerialPortModel& sp = mAppState->GetSerialPortModel();

            if (sp.BytesToRead() > 0)
            {
				int bytes_read = 0;
				if ((bytes_read = sp.Read()) > 0)
				{
				   char* buffer = sp.GetReadBuffer();
				   info("GrblMachineModel: Read {} bytes : {}",bytes_read,buffer);

                   	while (*buffer)
                    {
                        if(*buffer == '\n')
                        {
                            string current = Util::trim_copy(mCurrentLine.str());
                            if (!current.empty())
                            {
								info("GrblMachineModel: Pushing back line {}",current);
								mLinesRead.push_back(current);
                                mCurrentLine.str(std::string());
                            }
                        }
                        else
                        {
                        	mCurrentLine << *buffer;
                        }
                        buffer++;
                    }
				}
            }

            for (string line : mLinesRead)
			{
				if(!line.empty())
				{
					GrblResponse response(line);
					ProcessResponse(response);
				}
			}

			mLinesRead.clear();

            if (GetTimeDelta() > mStatusInterval)
            {
				RequestStatus();
            }

            SendNextPacket();

            yield();
            mLastTime = mCurrentTime;
        }
    }

    void GrblMachineModel::JoinWorkThread()
    {
        info("GrblMachineModel: {}",__FUNCTION__);
        mWorkThreadRunning = false;
        mWorkThread.join();
        info("GrblMachineModel: Work Thread has Joined");
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
		feed, rapids,and spindle speed, respectively.
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
        debug("GrblMachineModel: {}",__FUNCTION__);
		static regex grblVersionRegex("Grbl ([\\w.]+)");
        smatch m;
        string data = response.GetData();

		if (regex_search(data,m,grblVersionRegex))
		{
			mGrblVersion = m[1];
            info("GrblMachineModel: Grbl Version String \"{}\"",mGrblVersion);
		}
	}

	void GrblMachineModel::ProcessResponse(const GrblResponse& response)
	{
		debug("GrblMachineModel: Process Response {}" , response.GetData());

		mLastState = mState;
		GCodeCommand next;

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
				GCodeCommandManualSend(GCodeCommand::GetFirmwareConfigurationCommand());
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
				}
				else if (!mCommandBuffer.empty())
				{
					next = mCommandBuffer.at(0);
					debug("GrblMachineModel: Popping command: ",
						"id {} | line {} | cmd {} | in Queue {} | in buffer {}",
                        next.GetID(),
					    next.GetLine(),
					    next.GetCommand(),
					    mCommandQueue.size(),
						mCommandBuffer.size());

					next.SetResponse(response);
					next.SetState(GcodeCommandState::Processed);
					mCountProcessedCommands++;
					//emit updateProgramTableStatusSignal(next);
					//emit setCompletionProgressSignal(GetProcessedPercent());

					if (next.IsM30Command())
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
            default:
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

	void GrblMachineModel::SendNextPacket()
	{
	    info("GrblMachineModel: {}", __FUNCTION__);
		SerialPortModel& serial_port = mAppState->GetSerialPortModel();

		if (mError)
		{
			mStatusRequested = false;
			mWaitingForStatus = false;
		}

		if (mStatusRequested)
		{
			if (mWaitingForStatus && !mError)
			{
				info("GrblMachineModel: Still waiting for last status request to respond");
				return;
			}

			else if (serial_port.IsPortOpen() && mBytesWaiting == 0 && !mError)
			{
				info("GrblMachineModel: Requesting status");
				mBytesWaiting += serial_port.Write(GCodeCommand::StatusUpdateCommand().GetCommand());
                serial_port.Write("\n");
				mStatusRequested = true;
				mWaitingForStatus = true;
			}
		}
		else if (!mWaitingForStatus)
		{
			SendNextCommandFromQueue();
		}
	}

	void GrblMachineModel::RequestStatus()
	{
		if (!mWaitingForStatus)
		{
            info("GrblMachineModel: Requesting Status");
			mStatusRequested = true;
        }
    }

    bool GrblMachineModel::IsWorkThreadRunning()
    {
    	return mWorkThreadRunning;
    }


	void GrblMachineModel::ClearState()
	{
		mCommandBuffer.clear();
		mCommandQueue.clear();
        mCurrentLine.str(std::string());
        mLinesRead.clear();
		mCommandQueueInitialSize = -1;
        mState = GrblMachineState::Unknown;
		mLastState = GrblMachineState::Unknown;
		mMachinePosition = vec3(0.0);
		mWorkPosition = vec3(0.0,0.0,0.0);
		mWorkCoordinateOffset = vec3(0.0,0.0,0.0);
		mProgramSendInterval = 1000.0f/10.0f;
		mStatusInterval = 1000.0f/ 5.0f;
		mCountProcessedCommands = 0;
		mCommandQueueInitialSize = 0;
		mFeedOverride = 100;
		mSpindleOverride =100;
		mRapidOverride = 100;
		mError = false;
		mErrorCode = -1;
        mBufferUsedPercentage = 0.0f;
		mBytesWaiting = 0;
		mStatusRequested = false;
		mWaitingForStatus = false;
		mProgramRunning = false;
		mToolChangeWaiting = false;
		mFeedRate = 0;
		mSpindleSpeed = 0;
	}

	bool GrblMachineModel::SendNextCommandFromQueue()
	{
		if (mCommandQueue.empty())
		{
			return false;
		}

        SerialPortModel& serial_port = mAppState->GetSerialPortModel();

		if (!serial_port.IsPortOpen())
		{
			debug("GrblMachineModel: sendCommand FAILED -> SerialPort is not open");
			return false;
		}

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
				 GCodeCommand& command = mCommandQueue.at(0);

				 if (command.IsMarker() ||  // Is Marker or
					(command.GetCommand().empty() &&  // No Command
					 command.GetRawCommand() == 0))
				 {
					 command = mCommandQueue.at(0);
                     mCommandQueue.pop_front();
					 command.SetState(GcodeCommandState::Skipped);
					 continue;
				 }

				if (!IsSpaceInBuffer(command))
				{
					debug("GrblMachineController: Buffer full, waiting... {}", command.GetCommand());
					break;
				}

				// Take the command off the Queue for processing
				command = mCommandQueue.at(0);
                mCommandQueue.pop_front();

				// Don't send this becaue GRBL doesn't respond/care and buffer Get
				// misaligned
				if (command.IsToolChangeCommand())
				{
					mToolChangeWaiting = true;
					//emit toolChangeSignal(command->GetToolNumber());
					command.SetState(GcodeCommandState::Processed);
					break;
				}
				else
				{
					mCommandBuffer.push_back(command);
					debug("GrblMachineModel: Writing", command.GetCommand());
					//mBytesWaiting += mSerialPort.write(command->GetCommand().toLatin1());
					command.SetState(GcodeCommandState::Sent);
				}
				//emit appendCommandToConsoleSignal(command);
			}
			return true;
		}
		return false;
	}

	bool GrblMachineModel::GetProgramRunning() const
	{
		return mProgramRunning;
	}

	void GrblMachineModel::SetProgramRunning(bool programRunning)
	{
		mProgramRunning = programRunning;
	}

	void GrblMachineModel::SendProgram()
	{
        GCodeFileModel& fileModel = mAppState->GetGCodeFileModel();
		debug("GrblMachineModel: onSendProgram()");

		ClearCommandBuffer();
		ClearCommandQueue();
		mCountProcessedCommands = 0;

		int index = 0;
		for (GCodeCommand& next : fileModel.GetData())
		{
			QueueCommand(next);
			index++;
		}

		mCommandQueueInitialSize = mCommandQueue.size();
		mProgramRunning = true;
	}

	void GrblMachineModel::SendProgramFromLine(long id)
	{
        GCodeFileModel& gcodeFile = mAppState->GetGCodeFileModel();

		info("GrblMachineModel: SendProgramFromLine {} " ,id);

		ClearCommandBuffer();
		ClearCommandQueue();
		mCountProcessedCommands = 0;

		int index = gcodeFile.GetCommandByID(id).GetLine();
		int size = gcodeFile.GetData().size();

		for (; index < size; index++)
		{
			QueueCommand(gcodeFile.GetData()[index]);
		}

		mCommandQueueInitialSize = mCommandQueue.size();
		mProgramRunning = true;
	}

	void GrblMachineModel::UpdateStatus(GrblResponse response)
	{
		debug("GrblMachineModel: Updating status from response {}", response.GetData());

		static regex statusRegex("<([^|<]\\w+)[:\\|]");
        smatch m;
        string data = response.GetData();


		if (regex_match(data,m,statusRegex))
		{
			string stateStr = m[1];
			debug("GrblMachineModel: parsing state from {}", stateStr);
			mLastState = mState;
			mState = stateFromString(stateStr);
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
          mConfigurationModel.SetValue(param,value);
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
		   return;
		}
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

	void GrblMachineModel::GCodeCommandManualSend(const GCodeCommand& command)
	{
        SerialPortModel& serial_port = mAppState->GetSerialPortModel();
		if (serial_port.IsPortOpen())
		{
			if (command.GetRawCommand() > 0)
			{
				debug("GrblMachineController: Manual Raw Gcode Send 0x{}", command.GetRawCommand());
				char c = command.GetRawCommand();
				mBytesWaiting += serial_port.Write(&c);
			}
			else
			{
				debug("GrblMachineController: Manual ASCII Gcode Send {}" ,command.GetCommand());
				mBytesWaiting += serial_port.Write(command.GetCommand());
			}
			//appendCommandToConsoleSignal(command);
		}
	}

	void GrblMachineModel::UpdateRapidOverride(float rate)
	{

	}

	void GrblMachineModel::ToolChangeCompleted()
	{
		mToolChangeWaiting = false;
	}

	void GrblMachineModel::QueueCommand(const GCodeCommand& command)
	{
		mCommandQueue.push_back(command);
	}

	int GrblMachineModel::BufferLengthInUse()
	{
		int length = 0;

		debug("GrblMachineModel: Commands in buffer:");

		for (GCodeCommand& gc : mCommandBuffer)
		{
			debug("\t{}|{}|Args: ", gc.GetLine(), gc.GetCommand());
            for (string arg : gc.GetArgs())
            {
                debug("{}",arg);
            }
			length += gc.GetCommandLength();
		}

		debug("GrblMachineModel: Buffer in use: {}", length)
                ;
		return length;
	}

	bool GrblMachineModel::IsSpaceInBuffer(const GCodeCommand& cmd)
	{
		int bufferLeft = BUFFER_LENGTH_LIMIT - BufferLengthInUse();
		int bufferUsed = BUFFER_LENGTH_LIMIT - bufferLeft;
        debug("GrblMachineModel: Buffer Left {} cmd length {}", bufferLeft, cmd.GetCommandLength());
		mBufferUsedPercentage = ((float)bufferUsed/BUFFER_LENGTH_LIMIT) * 100.0f;
		return bufferLeft >= cmd.GetCommandLength();
	}

	GCodeCommand GrblMachineModel::FeedOverride(const GCodeCommand& command, double overridePercent)
	{
		return GCodeParser::OverrideSpeed(command,overridePercent);
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


	GrblConfigurationModel& GrblMachineModel::GetConfigurationModel()
    {
        return mConfigurationModel;
    }


    long GrblMachineModel::GetTimeDelta()
    {
        /* cap off time deltas greater than 1 sec */
        long delta = mCurrentTime - mLastTime;
        return delta > 1000 ? 1000 : delta;
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
