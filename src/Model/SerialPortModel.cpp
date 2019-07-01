#include "SerialPortModel.h"
#include "../Common/Logger.h"

namespace Coconut
{
	SerialPortModel::SerialPortModel(AppState* state)
        : mAppState(state),
          mBaudRate(115200),
          mTimeout(1000)
	{
        debug("SerialPortModel: Constructor");
		ProbeSerialPortNames();
	}

    SerialPortModel::~SerialPortModel()
    {
        debug("SerialPortModel: Destructor");
    }

    void SerialPortModel::SetPortName(const string& port_name)
    {
        mPortName = port_name;
    }

    bool SerialPortModel::OpenSerialPort()
    {
       if (mPort.isOpen())
       {
           error("SerialPortModel: Open mPort is already open");
           return false;
       }

       info("SerialPortModel: {} :{} ",__FUNCTION__,mPortName);
	   

	   mPort.setBytesize(serial::bytesize_t::eightbits);
       mPort.setStopbits(serial::stopbits_t::stopbits_one);
       mPort.setParity(serial::parity_t::parity_none);
	   mPort.setFlowcontrol(serial::flowcontrol_none);
	   mPort.setTimeout(serial::Timeout::simpleTimeout(mTimeout));
       mPort.setBaudrate(mBaudRate);
       mPort.setPort(mPortName);

	   mPort.open();
       
       if (mPort.isOpen())
       {
       	info("SerialPortModel: Successfuly Opened serial port");
       	return true;
       }

       error("SerialPortModel: Unable to open port");
       return false;
    }


    bool SerialPortModel::CloseSerialPort()
    {
       if (!mPort.isOpen())
       {
           error("SerialPortModel: Close - mPort is not open");
           return false;
       }

       info("SerialPortModel: {} : {}",__FUNCTION__,mPortName);
       mPort.close();

       info("SerialPortModel: Successfuly Closed serial port");
       return true;
    }

    bool SerialPortModel::IsPortOpen()
    {
       return mPort.isOpen();
    }

    string SerialPortModel::Read()
    {
        if (!mPort.isOpen())
        {
            error("SerialPortModel: Cannot read mPort is null");
            return "";
        }
        string data = mPort.read();
        debug("SerialPortModel: {} : {}",__FUNCTION__,data);
        return data;
    }

    string SerialPortModel::ReadLine()
    {
        if (!mPort.isOpen())
        {
            error("SerialPortModel: Cannot read mPort is null");
            return "";
        }
        memset(mReadBuffer,0,BUFSIZ);
        string data = mPort.readline();
        debug("SerialPortModel: {} : {}",__FUNCTION__,data);
        return data;
    }

    int SerialPortModel::Write(const string& data)
    {
        if (!mPort.isOpen())
        {
            error("SerialPortModel: Cannot write, mPort is null");
            return -1;
        }
        int bw = mPort.write((const uint8_t*)data.c_str(),data.size());
	
        debug("SerialPortModel: {} : wrote {} bytes {}",__FUNCTION__,bw,data);
        return bw;
    }

    int SerialPortModel::GetTimeout() const
    {
        return mTimeout;
    }

    void SerialPortModel::SetTimeout(int timeout)
    {
        mTimeout = timeout;
    }

    char* SerialPortModel::GetReadBuffer()
    {
       return mReadBuffer;
    }

    void SerialPortModel::SetBaudRate(int br)
    {
        info("SerialPortModel: {} : {}",__FUNCTION__,br);
        mBaudRate = br;
    }

    vector<string>& SerialPortModel::GetSerialPortNames()
	{
        return mSerialPortNames;
    }

    void SerialPortModel::ProbeSerialPortNames()
    {
        mSerialPortNames.clear();
        vector<serial::PortInfo> devices_found = serial::list_ports();
		vector<serial::PortInfo>::iterator iter = devices_found.begin();
		while( iter != devices_found.end() )
		{
			serial::PortInfo device = *iter++;
			mSerialPortNames.push_back(device.port.c_str());
		}
	}

    const vector<string> SerialPortModel::BaudRateNames =
    {
       "110",
       "300",
       "600",
       "1200",
       "2400",
       "4800",
       "9600",
       "14400",
       "19200",
       "38400",
       "57600",
       "115200",
       "128000",
       "256000"
    };
}
