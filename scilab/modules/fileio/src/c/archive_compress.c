/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Kartik Gupta
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
int archive_compress(char *archive_name, char **file_list, int size, char *arg_list, int *error)
{
    int i = 0;
    int j = strlen(arg_list);
    i = 0;
    int bzip2 = 0, gzip = 0, zip = 0;
    while (i < (j - 1))
    {
        if (arg_list[i] == 'j' || arg_list[i] == 'y')
        {
            bzip2 = 1;
        }
        else if (arg_list[i] == 'Z')
        {
            zip = 1;
        }
        else if (arg_list[i] == 'z')
        {
            gzip = 1;
        }
        i++;
    }
    static char buff[16384];
    struct archive *a;
    struct archive *disk;
    struct archive_entry *entry;
    ssize_t len;
    int fd;

    a = archive_write_new();
    if (bzip2 == 1)
    {
        archive_write_add_filter_bzip2(a);
    }
    else if (gzip == 1)
    {
        archive_write_add_filter_gzip(a);
    }
    else if (zip == 1)
    {
        archive_write_add_filter_compress(a);
    }
    else
    {
        archive_write_add_filter_none(a);
    }

    archive_write_set_format_ustar(a);
    archive_write_open_filename(a, archive_name);
    disk = archive_read_disk_new();
    archive_read_disk_set_standard_lookup(disk);
    i = 0;
    while (i < size)
    {
        struct archive *disk = archive_read_disk_new();
        int r;
        r = archive_read_disk_open(disk, file_list[i]);
        if (r != ARCHIVE_OK)
        {
            *error = 1;
            return -1;
        }
        while (1)
        {

            entry = archive_entry_new();
            r = archive_read_next_header2(disk, entry);
            if (r == ARCHIVE_EOF)
            {
                break;
            }
            if (r != ARCHIVE_OK)
            {
                *error = 2;
                return -1;
            }
            archive_read_disk_descend(disk);
            char pathname[1024];
            strcpy(pathname, archive_entry_pathname(entry));
#ifdef _WIN32
            if (pathname[1] == ":" && pathname[2] == 92)
            {
                int l = 0;
                while (l < (strlen(archive_entry_pathname(entry)) - 2))
                {
                    pathname[l] = pathname[l + 3];
                    l++;
                }
            }
#else
            if (pathname[0] == 47)
            {
                int l = 0;
                while (l < (strlen(archive_entry_pathname(entry))))
                {
                    pathname[l] = pathname[l + 1];
                    l++;
                }
            }
#endif
            archive_entry_set_pathname(entry, pathname);
            r = archive_write_header(a, entry);
            if (r < ARCHIVE_OK)
            {
                *error = 3;
                return -1;
            }
            if (r == ARCHIVE_FATAL)
            {
                *error = 3;
                return -1;
            }
            if (r > ARCHIVE_FAILED)
            {
                fd = open(archive_entry_sourcepath(entry), O_RDONLY);
                len = read(fd, buff, sizeof(buff));
                while (len > 0)
                {
                    archive_write_data(a, buff, len);
                    len = read(fd, buff, sizeof(buff));
                }
                close(fd);
            }
            archive_entry_free(entry);
        }
        archive_read_close(disk);
        archive_read_free(disk);
        i++;
    }
    archive_write_close(a);
    archive_write_free(a);


    return 0;


}
