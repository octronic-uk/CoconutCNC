#pragma once

#include "File.h"

namespace Coconut
{

    class Directory
    {
    public:
        Directory(string dir);
        ~Directory();

        vector<string> List(string regex = "");
        vector<string> ListSubdirectories(string regex = "");

        string GetPath() const;
        void SetPath(string path);
        string GetName();

        bool Exists();
        bool Create();
        bool DeleteDirectory();
        File GetFile(string fileName);
        bool IsDirectory();

    private:
        string mPath;
    };

}
