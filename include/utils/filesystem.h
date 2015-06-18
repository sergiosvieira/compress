#ifndef __FILE_SYSTEM__
#define __FILE_SYSTEM__

#ifdef _WIN32
    #include <io.h>
    #include "windows/dirent.h"
    #define SLASH "\\"
#else
    #include <unistd.h>
    #define SLASH "/"
    #include <dirent.h>
#endif

/** C++ **/
#include <vector>
#include <string>

class FileSystem
{
public:
    static void fileTree(std::vector<std::string>& a_files,
                         DIR* a_parent,
                         const char* a_name,
                         int a_level);
};

#endif /** __FILE_SYSTEM__ **/
