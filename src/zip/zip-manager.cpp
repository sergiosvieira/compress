#include "zip/zip-manager.h"

/** C++ **/
#include <cassert>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#ifdef _WIN32
#include <io.h>
#include "windows/dirent.h"
#define SLASH "\\"
#else
#include <unistd.h>
#define SLASH "/"
#include <dirent.h>
#endif
#include <cstring>
#include <cassert>
#include <iostream>

/** LibArchive **/
#include <archive_entry.h>


/** Public Methods **/
void ZipManager::compress(StringVectorRef a_filePaths,
                          const std::string &a_outputZipFilename,
                          ErrorCode* a_errorCode,
                          ProgressCallback a_progress)
{
    #ifndef _WIN32
    std::locale::global(std::locale(""));
    #endif
    ZipData *zipData = new ZipData();
    struct archive *larchive = nullptr;
    struct archive_entry *entry = nullptr;
    struct stat st;
    char buffer[8192] = {};
    int length = 0;
    int fileDescription = 0;
    assert((larchive = archive_write_new()) != nullptr);
    zipData->name = a_outputZipFilename.c_str();
    assert(archive_write_set_format_zip(larchive) == ARCHIVE_OK);
    assert(archive_write_zip_set_compression_deflate(larchive) == ARCHIVE_OK);
    assert(archive_write_set_options(larchive, "hdrcharset=UTF-8") == ARCHIVE_OK);
    assert(archive_write_open(larchive,
                              zipData,
                              openCallback,
                              writeCallback,
                              closeCallback) == ARCHIVE_OK);
    for (const std::string& file: a_filePaths)
    {
        stat(file.c_str(), &st);
        entry = archive_entry_new();
        assert(entry != nullptr);
        archive_entry_copy_stat(entry, &st);
        archive_entry_set_pathname(entry, file.c_str());
        archive_write_header(larchive, entry);
        fileDescription = open(file.c_str(), O_RDONLY);
        if (fileDescription != -1)
        {
            length = read(fileDescription, buffer, sizeof(buffer));
            while (length > 0)
            {
                archive_write_data(larchive, buffer, length);
                length = read(fileDescription, buffer, sizeof(buffer));
            }
        }
        archive_entry_free(entry);
    }
    archive_write_finish(larchive);
}

void ZipManager::uncompressAll(const std::string &a_filename)
{

}

int ZipManager::openCallback(archive *a_archive,
                             void *a_clientData)
{
    ZipData *zipData = (ZipData*)a_clientData;
    zipData->fd = open(zipData->name,
                       O_WRONLY | O_CREAT,
                       0644);
    return (zipData->fd >= 0)
           ? ARCHIVE_OK
           : ARCHIVE_FATAL;
}

int ZipManager::closeCallback(archive *a_archive,
                              void *a_clientData)
{
    ZipData *zipData = (ZipData*)a_clientData;
    if (zipData->fd > 0)
    {
        close(zipData->fd);
    }
    return 0;
}

ssize_t ZipManager::writeCallback(archive *a_archive,
                                  void *a_clientData,
                                  const void *a_buffer,
                                  size_t a_length)
{
    ZipData* zipData = (ZipData*)a_clientData;
    return write(zipData->fd, a_buffer, a_length);
}

/** Private Methods **/
