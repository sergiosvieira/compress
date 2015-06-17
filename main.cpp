#include <sys/stat.h>
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <stdlib.h>
//#include <unistd.h>
#ifdef _WIN32
#include <io.h>
#include "dirent.h"
#define SLASH "\\"
#else
#include <unistd.h>
#define SLASH "/"
#include <dirent>
#endif
#include <cstring>
#include <vector>
#include <iostream>
#include <cassert>

struct mydata {
        const char *name;
        int fd;
};

int
myopen(struct archive *a, void *client_data)
{
  struct mydata *mydata = (struct mydata*)client_data;


  mydata->fd = open(mydata->name, O_WRONLY | O_CREAT, 0644);
  if (mydata->fd >= 0)
    return (ARCHIVE_OK);
  else
    return (ARCHIVE_FATAL);
}

int
myclose(struct archive *a, void *client_data)
{
    struct mydata *mydata = (struct mydata*)client_data;
    if (mydata->fd > 0)
    {
        close(mydata->fd);
    }
    return (0);
}

/**
ssize_t
mywrite(struct archive *a, void *client_data, const void *buff, size_t n)
**/


__LA_SSIZE_T mywrite(struct archive * a,
			    void *client_data,
			    const void *buffer, size_t length)
{
  struct mydata *mydata = (struct mydata*)client_data;


  return (write(mydata->fd, buffer, length));
}

void
write_archive(const char *outname, const std::vector<std::string>& a_vector)
{
    struct mydata *mydata = (struct mydata*)malloc(sizeof(struct mydata));
    struct archive *a = nullptr;
    struct archive_entry *entry = nullptr;
    struct stat st;
    char buff[8192];
    int len;
    int fd;
    assert((a = archive_write_new()) != nullptr);
    mydata->name = outname;
    //int result = archive_write_zip_set_compression_store(a);
	assert(archive_write_set_format_zip(a) == ARCHIVE_OK);
	assert(archive_write_zip_set_compression_deflate(a) == ARCHIVE_OK);	
	assert(archive_write_set_options(a, "hdrcharset=UTF-8") == ARCHIVE_OK);	
    assert(archive_write_open(a, mydata, myopen, mywrite, myclose) == ARCHIVE_OK);

    for (const std::string& file: a_vector)
    {
        stat(file.c_str(), &st);
        entry = archive_entry_new();
		assert(entry != nullptr);
        archive_entry_copy_stat(entry, &st);
        archive_entry_set_pathname(entry, file.c_str());
		assert(archive_write_header(a, entry) == ARCHIVE_OK);
		//assert(archive_write_add_filter_compress(a) == ARCHIVE_OK);
		//assert(archive_write_add_filter_uuencode(a) == ARCHIVE_OK);
        fd = open(file.c_str(), O_RDONLY);
		if (fd != -1)
		{
			len = read(fd, buff, sizeof(buff));
			while (len > 0)
			{
				__LA_SSIZE_T length = archive_write_data(a, buff, len);
				len = read(fd, buff, sizeof(buff));
			}
		}
        archive_entry_free(entry);
    }
    archive_write_finish(a);
}

void listDir(std::vector<std::string>& a_vector, DIR* a_parent, const char* a_dirName, int a_level)
{
    DIR* dir = nullptr;
    dir = opendir(a_dirName);
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
                    for (int i = 0; i < a_level; ++i)
                    {
                        printf("  ");
                    }
                    char fullName[256] = {};
                    if (a_parent != nullptr)
                    {
                        struct dirent* parentEntity = readdir(a_parent);
                        strcpy(fullName, parentEntity->d_name);
                    }
                    else
                    {
                        strcpy(fullName, a_dirName);
                    }
                    strcat(fullName, SLASH);
                    strcat(fullName, entity->d_name);
                    //printf("[%s]\n", fullName);
                    listDir(a_vector, dir, fullName, a_level + 1);
                    a_vector.push_back(fullName);
                }
            }
            else
            {
                for (int i = 0; i < a_level; ++i)
                {
                    printf("  ");
                }
                char fullName[256] = {};
                strcpy(fullName, a_dirName);
                strcat(fullName, SLASH);
                strcat(fullName, entity->d_name);
                //printf("%s\n", fullName);
                a_vector.push_back(fullName);
            }
        }
    }
}

int main(int argc, const char **argv)
{
    std::vector<std::string> files;
    if (argv[1] != nullptr)
    {
        listDir(files, nullptr, argv[1], 1);
    }
    for (const std::string& file: files)
    {
        std::cout << file.c_str() << "\n";
    }
    write_archive("saida.zip", files);
    return 0;
}
