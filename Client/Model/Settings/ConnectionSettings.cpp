/*
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
#include "ConnectionSettings.h"
#include "../../Logger.h"

namespace Coconut
{
	ConnectionSettings::ConnectionSettings()
		: mSerialPort("Not Set"),
		  mSerialBaudRate(115200),
		  mIgnoreErrorMessages(false),
		  mSetParserState(false),
		  mArcApproximation(false),
		  mArcApproximationLength(0.0),
		  mArcApproximationDegrees(0.0)
	{
		debug("ConnectionSettings: Constructing");
	}

	ConnectionSettings::~ConnectionSettings() {}

	string ConnectionSettings::GetSerialPort() const
	{
		return mSerialPort;
	}

	void ConnectionSettings::SetSerialPort(const string& serialPort)
	{
		mSerialPort = serialPort;
	}

	int ConnectionSettings::GetSerialBaudRate() const
	{
		return mSerialBaudRate;
	}

	void ConnectionSettings::SetSerialBaudRate(int serialBaudRate)
	{
		mSerialBaudRate = serialBaudRate;
	}

	bool ConnectionSettings::GetIgnoreErrorMessages() const
	{
		return mIgnoreErrorMessages;
	}

	void ConnectionSettings::SetIgnoreErrorMessages(bool ignoreErrorMessages)
	{
		mIgnoreErrorMessages = ignoreErrorMessages;
	}

	bool ConnectionSettings::GetSetParserState() const
	{
		return mSetParserState;
	}

	void ConnectionSettings::SetSetParserState(bool SetParserState)
	{
		mSetParserState = SetParserState;
	}

	bool ConnectionSettings::GetArcApproximation() const
	{
		return mArcApproximation;
	}

	void ConnectionSettings::SetArcApproximation(bool arcApproximation)
	{
		mArcApproximation = arcApproximation;
	}

	float ConnectionSettings::GetArcApproximationLength() const
	{
		return mArcApproximationLength;
	}

	void ConnectionSettings::SetArcApproximationLength(float arcApproximationLength)
	{
		mArcApproximationLength = arcApproximationLength;
	}

	float ConnectionSettings::GetArcApproximationDegrees() const
	{
		return mArcApproximationDegrees;
	}

	void ConnectionSettings::SetArcApproximationDegrees(float arcApproximationDegrees)
	{
        mArcApproximationDegrees = arcApproximationDegrees;
    }

    json ConnectionSettings::ToJson()
    {
		json j;
		j[SERIAL_PORT_NAME] = GetSerialPort();
		j[SERIAL_BAUD_RATE] = GetSerialBaudRate();
		j[SERIAL_IGNORE_ERROR] = GetIgnoreErrorMessages();
		j[SERIAL_SET_PARSER_STATE] = GetSetParserState();
		j[SERIAL_ARC_APPROX] = GetArcApproximation();
		j[SERIAL_ARC_APPROX_LEN] = GetArcApproximationLength();
		j[SERIAL_ARC_APPROX_DEG] = GetArcApproximationDegrees();
		return j;
    }

    bool ConnectionSettings::FromJson(const json& j)
    {
  		if(!j[SERIAL_PORT_NAME].is_string()) return false;
        SetSerialPort(j[SERIAL_PORT_NAME]);

		if(!j[SERIAL_BAUD_RATE].is_number()) return false;
        SetSerialBaudRate(j[SERIAL_BAUD_RATE]);

		if(!j[SERIAL_IGNORE_ERROR].is_boolean()) return false;
        SetIgnoreErrorMessages(j[SERIAL_IGNORE_ERROR]);

		if(!j[SERIAL_SET_PARSER_STATE].is_boolean()) return false;
        SetSetParserState(j[SERIAL_SET_PARSER_STATE]);

		if(!j[SERIAL_ARC_APPROX].is_boolean()) return false;
        SetArcApproximation(j[SERIAL_ARC_APPROX]);

		if(!j[SERIAL_ARC_APPROX_LEN].is_number()) return false;
        SetArcApproximationLength(j[SERIAL_ARC_APPROX_LEN]);

		if(!j[SERIAL_ARC_APPROX_DEG].is_number()) return false;
        SetArcApproximationDegrees(j[SERIAL_ARC_APPROX_DEG]);

        return true;
    }

    // Serial
	const string ConnectionSettings::SERIAL_PORT_NAME = "port_name";
	const string ConnectionSettings::SERIAL_BAUD_RATE = "baud_rate";
    const string ConnectionSettings::SERIAL_IGNORE_ERROR = "ignore_errors";
	const string ConnectionSettings::SERIAL_SET_PARSER_STATE = "set_parser_state";
	const string ConnectionSettings::SERIAL_ARC_APPROX = "arc_approx";
	const string ConnectionSettings::SERIAL_ARC_APPROX_LEN = "arc_approx_len";
	const string ConnectionSettings::SERIAL_ARC_APPROX_DEG = "arc_approx_deg";

}
