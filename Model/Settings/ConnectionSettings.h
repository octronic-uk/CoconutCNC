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

#pragma once

#include <string>
#include "JsonSerialization.h"

using std::string;

namespace Coconut
{
	class ConnectionSettings : public JsonSerialization
	{
	public:
		ConnectionSettings();
		~ConnectionSettings();

		string GetSerialPort() const;
		void SetSerialPort(const string& serialPort);

		int GetSerialBaudRate() const;
		void SetSerialBaudRate(int serialBaudRate);

        int GetTimeout() const;
        int* GetTimeoutPtr();
        void SetTimeout(int timeout);

        // JsonSerialization interface
        json ToJson() override;
        bool FromJson(const json& j) override;

	private:
		string mSerialPort;
		int mSerialBaudRate;
        int mTimeout;

		const static string SERIAL_PORT_NAME;
		const static string SERIAL_BAUD_RATE;
		const static string SERIAL_TIMEOUT;

	};

}
