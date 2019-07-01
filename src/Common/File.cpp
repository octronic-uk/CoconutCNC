/*
* FileReader
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "File.h"

#include <fstream>
#include <sstream>
#include <cstdio>

#include "Logger.h"

#ifdef DeleteFile
#undef DeleteFile
#endif

using std::istreambuf_iterator;
using std::ios;
using std::stringstream;
using std::ifstream;

namespace Coconut
{
    File::File(string path)
    {
        mPath = path;
    }

    File::~File
    ()
    {
        debug("FileReader: Destroying reader for {}" , mPath );
    }

    string File::GetDirectory() const
    {
        auto endOfPath = mPath.find_last_of('/');
        if (endOfPath == string::npos)
        {
            endOfPath = mPath.find_last_of('\\');
        }
        if (endOfPath == string::npos)
        {
            return "";
        }
        return mPath.substr(0,endOfPath);
    }

    string File::GetPath() const
    {
        return mPath;
    }

    string File::ReadString() const
    {
        ifstream inputStream;
        stringstream stringStream;

        inputStream.open(mPath.c_str(), ifstream::in);
        if (inputStream.is_open())
        {
            string line;
            while ( getline (inputStream,line) )
            {
                stringStream << line << '\n';
            }
            auto data = stringStream.str();
            inputStream.close();
            return data;
        }
        else
        {
            return "";
        }
    }

    vector<string> File::ReadAsLines() const
    {
        vector<string> lines;
        ifstream inputStream;

        inputStream.open(mPath.c_str(), ifstream::in);
        if (inputStream.is_open())
        {
            string line;
            while ( getline (inputStream,line) )
            {
                lines.push_back(line);
            }
            inputStream.close();
            return lines;
        }
        else
        {
            return lines;
        }
    }

    vector<char> File::ReadBinary() const
    {
        vector<char> data;
        ifstream inputStream;
        inputStream.open(mPath.c_str(), ios::binary );
        data = vector<char>
        (
            (istreambuf_iterator<char>(inputStream)),
            (istreambuf_iterator<char>())
        );
        inputStream.close();
        return data;
    }

    bool File::Create() const
    {
        string data = ".";
        auto file = fopen(mPath.c_str(),"wb");
        auto bytesWritten = fwrite(&data[0],sizeof(char),data.size(),file);
        fclose(file);
        return bytesWritten == data.size();
    }

    bool File::WriteBinary (const vector<char>& data) const
    {
        auto file = fopen(mPath.c_str(),"wb");
        auto bytesWritten = fwrite(&data[0],sizeof(char),data.size(),file);
        fclose(file);
        return bytesWritten == data.size();
    }

    bool File::WriteString (const std::string& data) const
    {
        auto file = fopen(mPath.c_str(),"wb");
        auto bytesWritten = fwrite(&data[0],sizeof(char),data.size(),file);
        fclose(file);
        return bytesWritten == data.size();
    }

    bool File::DeleteFile() const
    {
        debug("Deleting file {}",mPath);
        if(remove(mPath.c_str()) != 0)
        {
            error("Error deleting file {}",mPath );
            perror("File check error");
            return false;
        }
        return true;
    }

    bool File::Exists() const
    {
        FILE* file = fopen(mPath.c_str(),"rb");
        if (file)
        {
            fclose(file);
            return true;
        }
        perror("File check error");
        return false;
    }

    string File::NameWithExtension() const
    {

        auto endOfPath = mPath.find_last_of('/');
        auto fileName = mPath.substr(endOfPath+1);
        debug("Got file name with extension {}",fileName);
        return fileName;
    }

    string File::NameWithoutExtension() const
    {
        auto name = NameWithExtension();
        auto extStart = name.find_last_of(".");
        auto nameOnly = name.substr(0,extStart);
        debug("Got file name without extension {}",nameOnly);
        return nameOnly;
    }

    string File::Extension() const
    {
        auto name = NameWithExtension();
        auto extStart = name.find_last_of(".");
        if (extStart != string::npos)
        {
            auto ext = name.substr(extStart+1);
            debug("Got file extension {}",ext);
            return ext;
        }
        return "";
    }


    void File::SetPath(string path)
    {
        mPath = path;
    }
}
