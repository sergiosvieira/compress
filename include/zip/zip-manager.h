#ifndef __ZIP_MANAGER__
#define __ZIP_MANAGER__

/** C++ **/
#include <vector>

/** Zipper **/
#include "error-codes.h"
#include "progress-callback.h"

/** LibArchive **/
#include <archive.h>

class ZipManager
{
    typedef const std::vector<std::string> &StringVectorRef;
    typedef struct
    {
        const char *name;
        int fd;
    } ZipData;
public:
    static void compress(StringVectorRef a_filePaths,
                         const std::string &a_outputZipFilename,
                         ErrorCode *a_errorCode = nullptr,
                         ProgressCallback a_progress = nullptr);
    static void uncompressAll(const std::string &a_filename);
private:
    static int openCallback(struct archive *a_archive,
                            void *a_clientData);
    static int closeCallback(struct archive *a_archive,
                             void *a_clientData);
    static __LA_SSIZE_T writeCallback(struct archive *a_archive,
                                      void *a_clientData,
                                      const void *a_buffer,
                                      size_t a_length);
};

#endif /** __ZIP_MANAGER__ **/
