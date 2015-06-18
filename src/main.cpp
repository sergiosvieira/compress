/** C++ **/
#include <iostream>

/** ZipManager **/
#include "zip/zip-manager.h"

/** Utils **/
#include "utils/filesystem.h"

int main(int argc, const char **argv)
{
    std::vector<std::string> files;
    if (argv[1] != nullptr)
    {
        FileSystem::fileTree(files, nullptr, argv[1], 1);
    }
    ZipManager::compress(files, "saida.zip");
    return 0;
}
