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

using glm::vec3;

namespace Coconut
{

	GrblMachineModel::GrblMachineModel()
		: mState(GrblMachineState::Unknown),
		  mLastState(GrblMachineState::Unknown),
		  mMachinePosition(vec3(0.0)),
		  mWorkPosition(vec3(0.0,0.0,0.0)),
		  mWorkCoordinateOffset(vec3(0.0,0.0,0.0)),
		  mSettingsModelHandle(nullptr),
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
		SetupSerialPort();
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
		CloseSerialPort();
	}

	void GrblMachineModel::SetupSerialPort()
	{
		// Setup serial port
        /*
		mSerialPort.setParity(QSerialPort::NoParity);
		mSerialPort.setDataBits(QSerialPort::Data8);
		mSerialPort.setFlowControl(QSerialPort::NoFlowControl);
		mSerialPort.setStopBits(QSerialPort::OneStop);
		connect(&mSerialPort, SIGNAL(readyRead()),this, SLOT(onSerialPortReadyRead()),Qt::DirectConnection);
		connect(&mSerialPort, SIGNAL(bytesWritten(qint64)), this, SLOT(onSerialBytesWritten(qint64)),Qt::DirectConnection);
		connect(&mSerialPort, SIGNAL(error(QSerialPort::SerialPortError)),this, SLOT(onSerialPortError(QSerialPort::SerialPortError)));
        */
	}

	void GrblMachineModel::OnConnect()
	{
		if (!mSerialPort.isOpen())
		{
			openPort();
		}
	}

	void GrblMachineModel::UpdateMachinePosition(const GrblResponse& response)
	{
		// Update machine coordinates
		string data = response.GetData();
		static QRegExp machinePositionExpression("MPos:([^,]*),([^,]*),([^,|]*)");
		if (machinePositionExpression.indexIn(data) != -1)
		{
			mMachinePosition.setX(machinePositionExpression.cap(1).toFloat());
			mMachinePosition.setY(machinePositionExpression.cap(2).toFloat());
			mMachinePosition.setZ(machinePositionExpression.cap(3).toFloat());
			emit updateMachinePositionSignal(mMachinePosition);
		}
	}

	void GrblMachineModel::updateFeedRateAndSpindleSpeed(const GrblResponse& response)
	{
		static QRegExp feedSpeedRegex("\\|FS:(\\d+),(\\d+)\\|");
		if (feedSpeedRegex.indexIn(response.getData()) > -1)
		{
			mFeedRate = feedSpeedRegex.cap(1).toInt();
			mSpindleSpeed = feedSpeedRegex.cap(2).toInt();
			emit feedRateChangedSignal(mFeedRate);
			emit spindleSpeedChangedSignal(mSpindleSpeed);
		}
	}

	void GrblMachineModel::updateWorkCoordinateOffset(const GrblResponse& resp)
	{
		// Update work coordinates
		static QRegExp workCoordExpression("WCO:([^,]*),([^,]*),([^,^>]*)");
		if (workCoordExpression.indexIn(resp.getData()) != -1)
		{
			mWorkCoordinateOffset.setX(workCoordExpression.cap(1).toFloat());
			mWorkCoordinateOffset.setY(workCoordExpression.cap(2).toFloat());
			mWorkCoordinateOffset.setZ(workCoordExpression.cap(3).toFloat());
			emit updateWCOSignal(mWorkCoordinateOffset);
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
	void GrblMachineModel::updateOverrides(const GrblResponse& data)
	{
		static QRegExp overridesRegex("Ov:(\\d+),(\\d+),(\\d+)");

		if (overridesRegex.indexIn(data.getData()) > -1)
		{
		   mFeedOverride = overridesRegex.cap(1).toFloat();
		   mRapidOverride = overridesRegex.cap(2).toFloat();
		   mSpindleOverride = overridesRegex.cap(3).toFloat();

		   /*qDebug() << "GrblMachineController: Got overrides"
					<< "F" << mFeedOverride
					<< "R" << mRapidOverride
					<< "S" << mSpindleOverride;
					*/

		   emit updateFeedOverrideSignal(mFeedOverride);
		   emit updateRapidOverrideSignal(mRapidOverride);
		   emit updateSpindleOverrideSignal(mSpindleOverride);
		}
	}

	void GrblMachineModel::updateWorkPosition()
	{
		mWorkPosition.setX(mMachinePosition.x() - mWorkCoordinateOffset.x());
		mWorkPosition.setY(mMachinePosition.y() - mWorkCoordinateOffset.y());
		mWorkPosition.setZ(mMachinePosition.z() - mWorkCoordinateOffset.z());
		emit updateWorkPositionSignal(mWorkPosition);
	}

	void GrblMachineModel::parseGrblVersion(const GrblResponse& response)
	{
		static QRegExp grblVersionRegex("Grbl ([\\w.]+)");

		if (grblVersionRegex.indexIn(response.getData().trimmed()) != -1)
		{
			mGrblVersion = grblVersionRegex.cap(1);
			//qDebug() << "GrblMachineModel: Grbl Version" << mGrblVersion;
		}
	}

	void GrblMachineModel::processResponse(const GrblResponse& response)
	{
		qDebug() << "GrblMachineModel: Process Response" << response.getData();

		mLastState = mState;
		GCodeCommand* next = nullptr;

		switch (response.getType())
		{
			case GrblResponseType::Alarm:
				parseAlarmResponse(response);
				emit appendResponseToConsoleSignal(response);
				break;
			case GrblResponseType::Startup:
				parseGrblVersion(response);
				mStatusRequested = false;
				mWaitingForStatus = false;
				mState = GrblMachineState::Locked;
				emit appendResponseToConsoleSignal(response);
				break;
			case GrblResponseType::Locked:
				mState = GrblMachineState::Locked;
				emit appendResponseToConsoleSignal(response);
				break;
			case GrblResponseType::Unlocked:
				mError = false;
				mState = GrblMachineState::Unlocked;
				onGcodeCommandManualSend(GCodeCommand::GetFirmwareConfigurationCommand());
				emit appendResponseToConsoleSignal(response);
				break;
			case GrblResponseType::Status:
				if (!mError)
				{
					updateStatus(response);
					updateMachinePosition(response);
					updateWorkCoordinateOffset(response);
					updateOverrides(response);
					updateFeedRateAndSpindleSpeed(response);
					updateWorkPosition();
					//qDebug() << "GrblMachineModel: Got status!";
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
					startProgramSendTimer();
				}
				else if (!mCommandBuffer.isEmpty())
				{
					next = mCommandBuffer.takeFirst();
					/*qDebug() << "GrblMachineModel: Popping command"
							 << "id" << next->getID()
							 << "| line" << next->getLine()
							 << "| cmd" << next->getCommand()
							 << "| in queue" << mCommandQueue.count()
							 << "| in buffer" << mCommandBuffer.count();
							 */

					next->setResponse(response);
					next->setState(GcodeCommandState::Processed);
					mCountProcessedCommands++;
					emit updateProgramTableStatusSignal(next);
					emit setCompletionProgressSignal(getProcessedPercent());

					if (next->isM30Command())
					{
						emit setCompletionProgressSignal(100);
						emit jobCompletedSignal();
						mProgramRunning = false;

					}
					//emit appendResponseToConsoleSignal(response);
				}

				// DO NOT ELSE IF THIS.
				// We Need to check again to see if that was the last
				// command in the Queue/Buffer and the program has finished.
				if (mProgramRunning && mCommandQueue.isEmpty() && mCommandBuffer.isEmpty())
				{
					//qDebug() << "GrblMachineModel: Program Finished";
					emit setCompletionProgressSignal(100);
					emit jobCompletedSignal();
					mProgramRunning = false;
				}

				emit appendResponseToConsoleSignal(response);

				break;
			case GrblResponseType::Error:
				parseError(response);
				mError = true;
				mWaitingForStatus = false;
				mStatusRequested = false;
				clearCommandQueue();
				clearCommandBuffer();
				mState = GrblMachineState::Error;
				emit errorSignal(mErrorString);
				emit appendResponseToConsoleSignal(response);
				break;
			case GrblResponseType::Configuration:
				parseConfigurationResponse(response);
				break;
		};

		if (mState != mLastState)
		{
			emit machineStateUpdatedSignal(mState);
		}
	}

	void GrblMachineModel::parseError(const GrblResponse& error)
	{
		if (!mError)
		{
			QString trimmedError = error.getData().trimmed();
			static QRegExp errorNumRegex("error:(\\d+)$");
			static QRegExp errorStrRegex("error:(.+)$");

			if (errorNumRegex.indexIn(trimmedError) != -1)
			{
				mErrorCode = errorNumRegex.cap(1).toInt();
				mErrorString = ERROR_STRINGS.at(mErrorCode);
			}
			else if (errorStrRegex.indexIn(trimmedError) != -1)
			{
				mErrorString = errorStrRegex.cap(1);
			}
		}
	}

	int GrblMachineModel::getProcessedPercent()
	{
		if (mCommandQueue.count() > 0)
			return
			static_cast<int>(
				(
					static_cast<double>(mCountProcessedCommands)
					/ static_cast<double>(mCommandQueueInitialSize)
				)
				* 100
			);
		else
			return 0;
	}

	void GrblMachineModel::onSerialPortReadyRead()
	{
		//qDebug() << "GrblMachineModel: onSerialPortReadyRead";
		while (mSerialPort.canReadLine())
		{
			QString data = mSerialPort.readLine().trimmed();
			//qDebug() << "GrblMachineModel: Got line from serial port" << data;
			GrblResponse response(data);
			processResponse(response);
		}
	}

	void GrblMachineModel::onSerialPortError(QSerialPort::SerialPortError error)
	{
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
			emit serialPortErrorSignal(tr("Serial port error ") + QString::number(error) + ": " + mSerialPort.errorString());
		}
	}

	void GrblMachineModel::onSerialPortNameChanged(QString port)
	{
	   setPortName(port);
	   closePort();
	}

	void GrblMachineModel::onSerialPortBaudRateChanged(int baud)
	{
	   setBaudRate(baud);
	   closePort();
	}

	void GrblMachineModel::onProgramSendTimerTimeout()
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

			else if (mSerialPort.isOpen() && mBytesWaiting == 0 && !mError)
			{
				qDebug() << "GrblMachineModel: Requesting status";
				mBytesWaiting += mSerialPort.write(GCodeCommand::StatusUpdateCommand()->getCommand().toLatin1());
				mStatusRequested = true;
				mWaitingForStatus = true;
			}
		}
		else if (!mWaitingForStatus)
		{
			sendNextCommandFromQueue();
		}
	}

	void GrblMachineModel::onStatusTimerTimeout()
	{
		if (!mWaitingForStatus)
		{
			mStatusRequested = true;
		}
	}

	void GrblMachineModel::setSettingsModelHandle(SqlSettingsModel* settingsModelHandle)
	{
		mSettingsModelHandle = settingsModelHandle;
	}

	bool GrblMachineModel::openPort()
	{
		if (mSerialPort.open(QIODevice::ReadWrite))
		{
			emit statusBarUpdateSignal(QString("Connected on Serial Port %1 @ %2 baud").arg(getPortName()).arg(getBaudRate()));
			emit machineConnectedSigal(true);
			return true;
		}
		else
		{
			emit machineConnectedSigal(false);
			emit statusBarUpdateSignal(QString("Unable to open port %1").arg(getPortName()));
		}
		return false;
	}

	bool GrblMachineModel::closePort()
	{
		mSerialPort.close();
		return true;
	}

	void GrblMachineModel::initialise()
	{
		mCommandBuffer.clear();
		mCommandQueue.clear();
		mCommandQueueInitialSize = -1;
		mFeedOverride = 100;
		mSpindleOverride = 100;
		mRapidOverride = 100;
	}

	bool GrblMachineModel::sendNextCommandFromQueue()
	{
		if (mCommandQueue.isEmpty())
		{
			return false;
		}

		if (!mSerialPort.isOpen())
		{
			qDebug() << "GrblMachineModel: sendCommand FAILED -> SerialPort is not open";
			//mProgramSendTimer.stop();
			return false;
		}

		if (mToolChangeWaiting)
		{
			qDebug() << "GrblMachineModel: Tool change waiting...";
			return false;
		}

		// Serial port has been flushed
		if (mBytesWaiting == 0)
		{
			while (!mError && !mCommandQueue.isEmpty())
			{
				 GCodeCommand* command = mCommandQueue.first();

				 if (command->isMarker() ||  // Is Marker or
					(command->getCommand().isEmpty() &&  // No Command
					 command->getRawCommand() == 0))
				 {
					 command = mCommandQueue.takeFirst();
					 command->setState(GcodeCommandState::Skipped);
					 continue;
				 }

				if (!isSpaceInBuffer(command))
				{
					/*
					qDebug() << "GrblMachineController: Buffer full, waiting..."
							 << command->getCommand();
							 */
					break;
				}

				// Take the command off the queue for processing
				command = mCommandQueue.takeFirst();

				// Don't send this becaue GRBL doesn't respond/care and buffer get
				// misaligned
				if (command->isToolChangeCommand())
				{
					mToolChangeWaiting = true;
					emit toolChangeSignal(command->getToolNumber());
					command->setState(GcodeCommandState::Processed);
					break;
				}
				else
				{
					mCommandBuffer.append(command);
					qDebug() << "GrblMachineModel: Writing" << command->getCommand();
					mBytesWaiting += mSerialPort.write(command->getCommand().toLatin1());
					command->setState(GcodeCommandState::Sent);
				}
				emit appendCommandToConsoleSignal(command);
			}
			return true;
		}
		return false;
	}

	void GrblMachineModel::onSerialBytesWritten(qint64 bytes)
	{
		mBytesWaiting -= bytes;
		//qDebug() << "GrblMachineModel: Serial bytes Written:" << bytes << "/ Remaining:" << mBytesWaiting;
	}

	bool GrblMachineModel::getProgramRunning() const
	{
		return mProgramRunning;
	}

	void GrblMachineModel::setProgramRunning(bool programRunning)
	{
		mProgramRunning = programRunning;
	}

	void GrblMachineModel::onSendProgram(GcodeFileModel* gcodeFile)
	{
		//qDebug() << "GrblMachineModel: onSendProgram()";

		clearCommandBuffer();
		clearCommandQueue();
		mCountProcessedCommands = 0;

		int index = 0;
		for (GCodeCommand* next : gcodeFile->getData())
		{
			queueCommand(next);
			index++;
		}

		mCommandQueueInitialSize = mCommandQueue.count();
		mProgramRunning = true;
	}

	void GrblMachineModel::onSendProgramFromLine(GcodeFileModel* gcodeFile, long id)
	{
		//qDebug() << "GrblMachineModel: onSendProgramFromLine @ id" << id;

		clearCommandBuffer();
		clearCommandQueue();
		mCountProcessedCommands = 0;

		int index = gcodeFile->getCommandByID(id)->getLine();
		int size = gcodeFile->getData().count();

		for (; index < size; index++)
		{
			queueCommand(gcodeFile->getData()[index]);
		}

		mCommandQueueInitialSize = mCommandQueue.count();
		mProgramRunning = true;
	}

	void GrblMachineModel::startStatusTimer()
	{
		if (!mStatusTimer.isActive())
		{
			mStatusTimer.start(mStatusInterval);
		}
	}

	void GrblMachineModel::startProgramSendTimer()
	{
		if (!mProgramSendTimer.isActive())
		{
			qDebug() << "GrblMachineController: Starting status timer";
			mProgramSendTimer.start(mProgramSendInterval);
		}
	}

	void GrblMachineModel::stopProgramSendTimer()
	{
		qDebug() << "GrblMachineController: Stopping program send timer";
		mProgramSendTimer.stop();
		clearCommandBuffer();
		clearCommandQueue();
	}

	void GrblMachineModel::stopStatusTimer()
	{
		qDebug() << "GrblMachineController: Stopping status timer";
		mStatusTimer.stop();
	}

	void GrblMachineModel::updateStatus(GrblResponse response)
	{
		/*qDebug() << "GrblMachineModel: Updating status from response"
				 << response.getData();
				 */

		static QRegExp statusRegex("<([^|<]\\w+)[:\\|]");

		if (statusRegex.indexIn(response.getData()) >=0)
		{
			QString stateStr = statusRegex.cap(1);
			//qDebug() << "GrblMachineModel: parsing state from" << stateStr;
			mLastState = mState;
			mState = stateFromString(stateStr);
			if (mState != mLastState)
			{
				emit machineStateUpdatedSignal(mState);
			}
		}
	}

	void GrblMachineModel::parseConfigurationResponse(GrblResponse response)
	{
	   static QRegExp configRegex("\\$(\\d+)=(\\S*)");
	   int param = -1;
	   QString value = "";

	   if (configRegex.indexIn(response.getData()) > -1)
	   {
		  param = configRegex.cap(1).toInt();
		  value = configRegex.cap(2);
		  /*qDebug() << "GrblMachineModel: Got configuration parameter"
				   << param << "-->" << value;
				   */
		  mFirmwareConfiguration[param] = value;
		  emit firmwareConfigurationReadSignal(param,value);
	   }
	}

	void GrblMachineModel::parseAlarmResponse(const GrblResponse& response)
	{
		static QRegExp alarmRegex("ALARM:(\\d+)");
		if (alarmRegex.indexIn(response.getData()) > -1)
		{
		   int alarmNum = alarmRegex.cap(1).toInt();
		   emit alarmSignal(ALARM_STRINGS.at(alarmNum));
		   return;
		}
		emit alarmSignal("Unknown Alarm");
	}

	void GrblMachineModel::clearCommandQueue()
	{
		mCommandQueue.clear();
		mCountProcessedCommands = 0;
		mCommandQueueInitialSize = 0;
	}

	void GrblMachineModel::clearCommandBuffer()
	{
		mCommandBuffer.clear();
	}

	bool GrblMachineModel::isPortOpen()
	{
		return mSerialPort.isOpen();
	}


	int GrblMachineModel::commandsQueueLength()
	{
		return mCommandQueue.count();
	}

	QString GrblMachineModel::getPortName()
	{
		return mSerialPort.portName();
	}

	void GrblMachineModel::setPortName(QString name)
	{
		mSerialPort.setPortName(name);
	}

	void GrblMachineModel::setBaudRate(int baud)
	{
		mSerialPort.setBaudRate(baud);
	}

	int GrblMachineModel::getBaudRate()
	{
		return mSerialPort.baudRate();
	}


	void GrblMachineModel::onGcodeCommandManualSend(GCodeCommand* command)
	{
		if (mSerialPort.isOpen())
		{
			if (command->getRawCommand() > 0)
			{
				qDebug() << "GrblMachineController: Manual Raw Gcode Send 0x"
						 << QString::number(command->getRawCommand(),16).toUpper();
				char c = command->getRawCommand();
				mBytesWaiting += mSerialPort.write(&c,1);
			}
			else
			{
				qDebug() << "GrblMachineController: Manual ASCII Gcode Send" << command->getCommand();
				mBytesWaiting += mSerialPort.write(command->getCommand().toLatin1());
			}
			emit appendCommandToConsoleSignal(command);
		}
	}

	void GrblMachineModel::onUpdateRapidOverride(float rate)
	{

	}

	void GrblMachineModel::onToolChangeCompleted()
	{
		mToolChangeWaiting = false;
	}

	void GrblMachineModel::onUpdateSpindleOverride(float speed)
	{

	}

	void GrblMachineModel::onUpdateFeedOverride(float rate)
	{

	}

	void GrblMachineModel::queueCommand(GCodeCommand* command)
	{
		mCommandQueue.append(command);
	}

	int GrblMachineModel::bufferLengthInUse()
	{
		int length = 0;

		//qDebug() << "GrblMachineModel: Commands in buffer:";
		for (GCodeCommand* gc : mCommandBuffer)
		{
			/*qDebug() << "\t" << gc->getLine()
					 << "|" << gc->getCommand()
					 << "|" << gc->getArgs();
					 */

			length += gc->getCommandLength();
		}

		//qDebug() << "GrblMachineModel: Buffer in use:" << length;
		return length;
	}

	bool GrblMachineModel::isSpaceInBuffer(GCodeCommand* cmd)
	{
		int bufferLeft = BUFFER_LENGTH_LIMIT - bufferLengthInUse();
		int bufferUsed = BUFFER_LENGTH_LIMIT - bufferLeft;
		/*qDebug() << "GrblMachineModel: Buffer Left" << bufferLeft
				 << "cmd length" << cmd->getCommandLength();
				 */

		emit setBufferProgressSignal(((float)bufferUsed/BUFFER_LENGTH_LIMIT)*100);
		return bufferLeft >= cmd->getCommandLength();
	}

	GCodeCommand GrblMachineModel::feedOverride(GCodeCommand* command, double overridePercent)
	{
		return GCodeParser::overrideSpeed(command,overridePercent);
	}

	vec3 GrblMachineModel::getMachinePosition()
	{
		return mMachinePosition;
	}

	float GrblMachineModel::getMachinePositionX()
	{
		return mMachinePosition.x();
	}

	float GrblMachineModel::getMachinePositionY()
	{
		return mMachinePosition.y();
	}

	float GrblMachineModel::getMachinePositionZ()
	{
		return mMachinePosition.z();
	}

	vec3 GrblMachineModel::getWorkPosition()
	{
		return mWorkPosition;
	}

	float GrblMachineModel::getWorkPositionX()
	{
		return mWorkPosition.x();
	}

	float GrblMachineModel::getWorkPositionY()
	{
		return mWorkPosition.y();
	}

	float GrblMachineModel::getWorkPositionZ()
	{
		return mWorkPosition.z();
	}

	QString GrblMachineModel::stateToString(GrblMachineState state)
	{
		switch (state)
		{
			case GrblMachineState::Idle:
				return QString("Idle");
			case GrblMachineState::Alarm:
				return QString("Alarm");
			case GrblMachineState::Run:
				return QString("Running");
			case GrblMachineState::Home:
				return QString("Home");
			case GrblMachineState::Hold:
				return QString("Hold");
			case GrblMachineState::Queue:
				return QString("Queue");
			case GrblMachineState::Check:
				return QString("Check");
			case GrblMachineState::Door:
				return QString("Door");
			case GrblMachineState::Locked:
				return QString("Locked");
			case GrblMachineState::Unlocked:
				return QString("Unlocked");
			case GrblMachineState::Jog:
				return QString("Jog");
			case GrblMachineState::Unknown:
				break;
		}

		return QString("Unknown");
	}

	const int GrblMachineModel::BUFFER_LENGTH_LIMIT = 127;

	const map<int,QString> GrblMachineModel::ALARM_STRINGS =
	{
		{
			1,
			"Hard limit triggered.\n"
			"Machine position is likely lost due to sudden and immediate halt. Re-homing is highly recommended."
		},
		{
			2,
			"G-code motion target exceeds machine travel.\n"
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

	const map<int,QString> GrblMachineModel::ERROR_STRINGS =
	{
		{
			1,
			QString("G-code words consist of a letter and a value. Letter was not found.")
		},
		{
			2,
			QString("Numeric value format is not valid or missing an expected value.")
		},
		{
			3,
			QString("Grbl '$' system command was not recognized or supported.")
		},
		{
			4,
			QString("Negative value received for an expected positive value.")
		},
		{
			5,
			QString("Homing cycle is not enabled via settings.")
		},
		{
			6,
			QString("Minimum step pulse time must be greater than 3usec")
		},
		{
			7,
			QString("EEPROM read failed. Reset and restored to default values.")
		},
		{
			8,
			QString("Grbl '$' command cannot be used unless Grbl is IDLE. Ensures smooth operation during a job.")
		},
		{
			9,
			QString("G-code locked out during alarm or jog state")
		},
		{
			10,
			QString("Soft limits cannot be enabled without homing also enabled.")
		},
		{
			11,
			QString("Max characters per line exceeded. Line was not processed and executed.")
		},
		{
			12,
			QString("(Compile Option) Grbl '$' setting value exceeds the maximum step rate supported.")
		},
		{
			13,
			QString("Safety door detected as opened and door state initiated.")
		},
		{
			14,
			QString("(Grbl-Mega Only) Build info or startup line exceeded EEPROM line length limit.")
		},
		{
			15,
			QString("Jog target exceeds machine travel. Command ignored.")
		},
		{
			16,
			QString("Jog command with no '=' or contains prohibited g-code.")
		},
		{
			17,
			QString("Laser mode requires PWM output.")
		},
		{
			20,
			QString("Unsupported or invalid g-code command found in block.")
		},
		{
			21,
			QString("More than one g-code command from same modal group found in block.")
		},
		{
			22,
			QString("Feed rate has not yet been set or is undefined.")
		},
		{
			23,
			QString("G-code command in block requires an integer value.")
		},
		{
			24,
			QString("Two G-code commands that both require the use of the XYZ axis words were detected in the block.")
		},
		{
			25,
			QString("A G-code word was repeated in the block.")
		},
		{
			26,
			QString("A G-code command implicitly or explicitly requires XYZ axis words in the block, but none were detected.")
		},
		{
			27,
			QString("N line number value is not within the valid range of 1 - 9,999,999.")
		},
		{
			28,
			QString("A G-code command was sent, but is missing some required P or L value words in the line.")
		},
		{
			29,
			QString("Grbl supports six work coordinate systems G54-G59. G59.1, G59.2, and G59.3 are not supported.")
		},
		{
			30,
			QString("The G53 G-code command requires either a G0 seek or G1 feed motion mode to be active. A different motion was active.")
		},
		{
			31,
			QString("There are unused axis words in the block and G80 motion mode cancel is active.")
		},
		{
			32,
			QString("A G2 or G3 arc was commanded but there are no XYZ axis words in the selected plane to trace the arc.")
		},
		{
			33,
			QString("The motion command has an invalid target. G2, G3, and G38.2 generates this error, if the arc is impossible to generate or if the probe target is the current position.")
		},
		{
			34,
			QString("A G2 or G3 arc, traced with the radius definition, had a mathematical error when computing the arc geometry. Try either breaking up the arc into semi-circles or quadrants, or redefine them with the arc offset definition.")
		},
		{
			35,
			QString("A G2 or G3 arc, traced with the offset definition, is missing the IJK offset word in the selected plane to trace the arc.")
		},
		{
			36,
			QString("There are unused, leftover G-code words that aren't used by any command in the block.")
		},
		{
			37,
			QString("The G43.1 dynamic tool length offset command cannot apply an offset to an axis other than its configured axis. The Grbl default axis is the Z-axis.")
		}
	};

}
