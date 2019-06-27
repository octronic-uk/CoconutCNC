#pragma once

#include <vector>
#include <libserialport.h>

#include "../Common/Logger.h"

using std::vector;
using std::string;

namespace Coconut
{
    class AppState;

    class SerialPortModel
    {
    public:
        SerialPortModel(AppState* state);
        ~SerialPortModel();

        void ProbeSerialPortNames();
        vector<string>& GetSerialPortNames();
    	bool SetSerialPort(const string& port_name);
        void SetBaudRate(int baud);

        bool OpenSerialPort();
        bool CloseSerialPort();
        bool IsPortOpen();

        int BytesToRead();
        int BytesToWrite();

        int Read();
        int Write(const string& data);

        static const vector<string> BaudRateNames;

        int GetTimeout() const;
        void SetTimeout(int timeout);

        char* GetReadBuffer();

    private:
        AppState* mAppState;
        vector<string> mSerialPortNames;
        char mReadBuffer[BUFSIZ];
        sp_port* mPort;
        bool mIsPortOpen;

        int mTimeout;
        int mBaudRate;
    };
}
