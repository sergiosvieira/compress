#include "utils/filesystem.h"

/** C++ **/
#include <cstring> // strcmp
#include <vector>

void FileSystem::fileTree(std::vector<std::string> &a_files,
                          DIR *a_parent,
                          const char *a_name,
                          int a_level)
{
    DIR* dir = nullptr;
    dir = opendir(a_name);
    if (dir != nullptr)
    {
        struct dirent* entity = nullptr;
        while((entity = readdir(dir)) != nullptr)
        {
            if (entity->d_type == 4
                || entity->d_type == 16384)
            {
                if (strcmp(entity->d_name, ".") != 0
                    && strcmp(entity->d_name, "..") != 0)
                {
                    char fullName[256] = {};
                    if (a_parent != nullptr)
                    {
                        struct dirent* parentEntity = readdir(a_parent);
                        strcpy(fullName, parentEntity->d_name);
                    }
                    else
                    {
                        strcpy(fullName, a_name);
                    }
                    strcat(fullName, SLASH);
                    strcat(fullName, entity->d_name);
                    fileTree(a_files, dir, fullName, a_level + 1);
                    a_files.push_back(fullName);
                }
            }
            else
            {
                char fullName[256] = {};
                strcpy(fullName, a_name);
                strcat(fullName, SLASH);
                strcat(fullName, entity->d_name);
                a_files.push_back(fullName);
            }
        }
    }
}
