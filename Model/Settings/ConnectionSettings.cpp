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
#include "../../Common/Logger.h"

namespace Coconut
{
	ConnectionSettings::ConnectionSettings()
		: mSerialPort("Not Set"),
		  mSerialBaudRate(115200),
		  mTimeout(1000)
	{
		debug("ConnectionSettings: Constructing");
	}

	ConnectionSettings::~ConnectionSettings()
    {
		debug("ConnectionSettings: Destructing");
    }

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

	int ConnectionSettings::GetTimeout() const
	{
		return mTimeout;
	}

    int* ConnectionSettings::GetTimeoutPtr()
	{
		return &mTimeout;
	}

	void ConnectionSettings::SetTimeout(int timeout)
	{
        mTimeout=timeout;
	}

    json ConnectionSettings::ToJson()
    {
        info("ConnectionSettings: {}",__FUNCTION__);
		json j;
		j[SERIAL_PORT_NAME] = GetSerialPort();
		j[SERIAL_BAUD_RATE] = GetSerialBaudRate();
		j[SERIAL_TIMEOUT] = GetTimeout();
		return j;
    }

    bool ConnectionSettings::FromJson(const json& j)
    {
        info("ConnectionSettings: {}",__FUNCTION__);

  		if(j.find(SERIAL_PORT_NAME) != j.end() && j[SERIAL_PORT_NAME].is_string())
        {
        	SetSerialPort(j[SERIAL_PORT_NAME]);
        }

		if(j.find(SERIAL_BAUD_RATE) != j.end() && j[SERIAL_BAUD_RATE].is_number())
        {
        	SetSerialBaudRate(j[SERIAL_BAUD_RATE]);
        }

		if(j.find(SERIAL_TIMEOUT) != j.end() && j[SERIAL_TIMEOUT].is_number())
        {
        	SetTimeout(j[SERIAL_TIMEOUT]);
    	}

        return true;
    }

    // Serial
	const string ConnectionSettings::SERIAL_PORT_NAME = "port_name";
	const string ConnectionSettings::SERIAL_BAUD_RATE = "baud_rate";
    const string ConnectionSettings::SERIAL_TIMEOUT = "timeout";

}
