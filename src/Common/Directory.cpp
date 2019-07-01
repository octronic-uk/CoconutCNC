#include "Directory.h"
#include "File.h"

#ifdef WIN32
#include <direct.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <regex>
#include <sstream>
#include "Logger.h"

using std::cmatch;
using std::stringstream;
using std::regex;

namespace Coconut
{

    Directory::Directory(string dir): mPath(dir)
    {

    }

    Directory::~Directory()
    {

    }

    vector<string> Directory::List (string regexStr)
    {
        auto usingRegex = !regexStr.empty();
        vector<string> directoryContents;
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(mPath.c_str())) != nullptr)
        {
            regex fileRegex(regexStr);
            cmatch match;

            while ((ent = readdir (dir)) != nullptr)
            {
                string fileName(ent->d_name);

                if (fileName[0] == '.') continue;

                if (usingRegex)
                {
                    if (regex_search(fileName.c_str(),match,fileRegex))
                    {
                        directoryContents.push_back(fileName);
                    }
                }
                else
                {
                    directoryContents.push_back(fileName);
                }
            }
            closedir (dir);
        }
        else
        {
            error( "Unable to open directory {}", mPath );
            return directoryContents;
        }
        return directoryContents;
    }

    vector<string> Directory::ListSubdirectories(string regexStr)
    {
        auto usingRegex = !regexStr.empty();
        vector<string> directoryContents;
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(mPath.c_str())) != nullptr)
        {
            regex fileRegex(regexStr);
            cmatch match;

            while ((ent = readdir (dir)) != nullptr)
            {
                string fileName(ent->d_name);
                if (fileName[0] == '.') continue;
                stringstream abs;
                abs << mPath << '/' << fileName;
                const char* absPath = abs.str().c_str();

                Directory subDir(absPath);
                if (!subDir.IsDirectory())
                {
                    continue;
                }

                if (usingRegex)
                {
                    if (regex_search(fileName.c_str(),match,fileRegex))
                    {
                        directoryContents.push_back(absPath);
                    }
                }
                else
                {
                    directoryContents.push_back(absPath);
                }
            }
            closedir (dir);
        }
        else
        {
            error("Unable to open directory {}", mPath );
            return directoryContents;
        }
        return directoryContents;

    }

    string Directory::GetPath() const
    {
        return mPath;
    }

    void Directory::SetPath(string path)
    {
        mPath = path;
    }

    string Directory::GetName()
    {
        auto nameStart = mPath.find_last_of('/');
        {
            if (nameStart != string::npos)
            {
                return mPath.substr(nameStart+1);
            }
        }
        return "";
    }

    bool Directory::Exists()
    {
        DIR* dir = opendir(mPath.c_str());
        if (dir)
        {
            closedir(dir);
            return true;
        }
        return false;
    }

    bool Directory::Create()
    {
#ifdef WIN32
        return mkdir(mPath.c_str()) == 0;
#else
        return mkdir(mPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
#endif
    }

    bool Directory::DeleteDirectory()
    {
        debug("Deleting directory {}",mPath);
        auto files = List();
        for (auto file : files)
        {
            if (file.compare(".") == 0) continue;
            if (file.compare("..") == 0) continue;

            string absPath = mPath+'/'+file;
            Directory d(absPath);
            if (d.IsDirectory())
            {
                d.DeleteDirectory();
            }
            else
            {
                File f(absPath);
                if (f.Exists())
                {
                    f.DeleteFile();
                }
            }
        }
        if (rmdir(mPath.c_str()) != 0)
        {
            error("Unable to delete directory {}",mPath);
            return false;
        }
        return true;
    }

    File Directory::GetFile(string fileName)
    {
        stringstream ss;
        ss << mPath << '/' << fileName;
        return File(ss.str());
    }

    bool Directory::IsDirectory()
    {
        bool result = false;
        struct stat sb;
        if (stat(mPath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
        {
            result = true;
        }
        error("{} Directory {}",result? "Is a" : "Not a", mPath);
        return result;
    }
}
