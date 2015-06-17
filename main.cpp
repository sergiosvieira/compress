#include <sys/stat.h>
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>

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

ssize_t
mywrite(struct archive *a, void *client_data, const void *buff, size_t n)
{
  struct mydata *mydata = (struct mydata*)client_data;


  return (write(mydata->fd, buff, n));
}

void
write_archive(const char *outname, const char **filename)
{
  struct mydata *mydata = (struct mydata*)malloc(sizeof(struct mydata));
  struct archive *a;
  struct archive_entry *entry;
  struct stat st;
  char buff[8192];
  int len;
  int fd;


  a = archive_write_new();
  mydata->name = outname;
  archive_write_zip_set_compression_store(a);
  archive_write_set_format_zip(a);
  archive_write_open(a, mydata, myopen, mywrite, myclose);
  while (*filename) {
    stat(*filename, &st);
    entry = archive_entry_new();
    archive_entry_copy_stat(entry, &st);
    archive_entry_set_pathname(entry, *filename);
    archive_write_header(a, entry);
    fd = open(*filename, O_RDONLY);
    len = read(fd, buff, sizeof(buff));
    while ( len > 0 ) {
        archive_write_data(a, buff, len);
        len = read(fd, buff, sizeof(buff));
    }
    archive_entry_free(entry);
    filename++;
  }
  archive_write_finish(a);
}

void listDir(DIR* a_parent, const char* a_dirName, int a_level)
{
    DIR* dir = nullptr;
    dir = opendir(a_dirName);
    if (dir != nullptr)
    {
        struct dirent* entity = nullptr;
        while((entity = readdir(dir)) != nullptr)
        {
            if (entity->d_type == 4)
            {
                if (strcmp(entity->d_name, ".") != 0
                    && strcmp(entity->d_name, "..") != 0)
                {
                    for (int i = 0; i < a_level; ++i)
                    {
                        printf("  ");
                    }
                    printf("[%s]\n", entity->d_name);
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
                    strcat(fullName, "/");
                    strcat(fullName, entity->d_name);
                    listDir(dir, fullName, a_level + 1);
                }
            }
            else
            {
                for (int i = 0; i < a_level; ++i)
                {
                    printf("  ");
                }
                printf("%s\n", entity->d_name);
            }
        }
    }
}

int main(int argc, const char **argv)
{
    if (argv[1] != nullptr)
    {
        printf("[%s]\n", argv[1]);
        listDir(nullptr, argv[1], 1);
    }
    //write_archive("saida.zip", argv);
    return 0;
}
