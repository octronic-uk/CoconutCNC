#include "SerialPortModel.h"
#include "../../Common/Logger.h"

namespace Coconut
{
	SerialPortModel::SerialPortModel(AppState* state)
        : mAppState(state),
          mPort(nullptr),
          mBaudRate(115200),
          mTimeout(1000),
          mIsPortOpen(false)

	{
        debug("SerialPortModel: Constructor");
		ProbeSerialPortNames();
	}

    SerialPortModel::~SerialPortModel()
    {
        debug("SerialPortModel: Destructor");
        if (mPort)
        {
            sp_free_port(mPort);
        }
    }

    bool SerialPortModel::SetSerialPort(const string& port_name)
    {
        info("SerialPortModel: {} : {}",__FUNCTION__, port_name);
		if (sp_get_port_by_name(port_name.c_str(),&mPort) != SP_OK)
        {
            return false;
        }
        return true;
    }

    bool SerialPortModel::OpenSerialPort()
    {
       if (!mPort)  {
           error("SerialPortModel: Open - mPort is nullptr");
           return false;
       }

       char* name = sp_get_port_name(mPort);
       info("SerialPortModel: {} :{} ",__FUNCTION__,name);

       if (sp_open(mPort,SP_MODE_READ_WRITE) != SP_OK)
       {
           error("SerialPortModel: Error opening port {} ",sp_last_error_message());
           mIsPortOpen = false;
           return false;
       }

       sp_set_bits(mPort,8);
       sp_set_stopbits(mPort,1);
       sp_set_parity(mPort,SP_PARITY_NONE);
       sp_set_baudrate(mPort,  mBaudRate);

       info("SerialPortModel: Successfuly Opened serial port");
       mIsPortOpen = true;
       return true;
    }


    bool SerialPortModel::CloseSerialPort()
    {
       if (!mPort)
       {
           error("SerialPortModel: Close - mPort is nullptr");
           return false;
       }
       char* name = sp_get_port_name(mPort);

       info("SerialPortModel: {} : {}",__FUNCTION__,name);
       mIsPortOpen = false;

       if (sp_close(mPort) != SP_OK)
       {
           error("SerialPortModel: Error closing port {}", sp_last_error_message());
           return false;
       }
       info("SerialPortModel: Successfuly Closed serial port");
       return true;
    }

    bool SerialPortModel::IsPortOpen()
    {
       return mIsPortOpen;
    }

    int SerialPortModel::BytesToRead()
    {
        if (!mPort) return -1;
		int btr =  sp_input_waiting(mPort);
        debug("SerialPortModel: {} : {}",__FUNCTION__,btr);
        return btr;
    }

    int SerialPortModel::BytesToWrite()
    {
		if (!mPort) return -1;
        int btw = sp_output_waiting(mPort);
        debug("SerialPortModel: {} : {}",__FUNCTION__,btw);
        return btw;
    }

    void SerialPortModel::FlushRead()
    {
        debug("SerialPortModel: {}",__FUNCTION__);
		sp_flush(mPort,sp_buffer::SP_BUF_INPUT);
    }

    void SerialPortModel::FlushWrite()
    {
        debug("SerialPortModel: {}",__FUNCTION__);
		sp_flush(mPort,sp_buffer::SP_BUF_OUTPUT);
    }

    int SerialPortModel::Read()
    {
        if (!mPort)
        {
            error("SerialPortModel: Cannot read mPort is null");
            return -1;
        }
        memset(mReadBuffer,0,BUFSIZ);
        int br = sp_nonblocking_read(mPort,mReadBuffer,BUFSIZ);
        debug("SerialPortModel: {} : {}",__FUNCTION__,br);
        return br;
    }

    int SerialPortModel::Write(const string& data)
    {
        if (!mPort)
        {
            error("SerialPortModel: Cannot write, mPort is null");
            return -1;
        }
        int bw = sp_nonblocking_write(mPort,data.c_str(),data.size());
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
        sp_port** list;
		if (sp_list_ports(&list) == SP_OK)
        {
            int i=0;
            while (list[i])
            {
            	sp_port* port = list[i];

				if (port)
                {
                    char* name = sp_get_port_name(port);
                    mSerialPortNames.push_back(string(name));
                }
                i++;
            }
        }
        sp_free_port_list(list);
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
