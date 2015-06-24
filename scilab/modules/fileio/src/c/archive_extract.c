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

#include<sys/types.h>
#include<sys/stat.h>
#include<archive.h>
#include<archive_entry.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int archive_extract(char *filename, int perm_flag, int* error)
{
    *error = 0;
    int flags;
    flags = ARCHIVE_EXTRACT_TIME;
    if (perm_flag == 1)
    {
        flags |= ARCHIVE_EXTRACT_PERM;
        flags |= ARCHIVE_EXTRACT_ACL;
        flags |= ARCHIVE_EXTRACT_FFLAGS;
    }
    int do_extract = 1;
    struct archive *to_be_read;
    struct archive *to_write;
    struct archive_entry *entry;
    int r;

    to_be_read = archive_read_new();
    archive_read_support_format_all(to_be_read);
    archive_read_support_compression_all(to_be_read);
    to_write = archive_write_disk_new();
    archive_write_disk_set_options(to_write, flags);
    archive_write_disk_set_standard_lookup(to_write);

    if ((r = archive_read_open_filename(to_be_read, filename, 10240)))
    {
        *error = 1;
        return -1;
    }
    for (;;)
    {
        r = archive_read_next_header(to_be_read, &entry);
        if (r == ARCHIVE_EOF)
        {
            break;
        }
        if (r < ARCHIVE_OK)
        {
            *error = 2;
            return -1;
        }
        if (r < ARCHIVE_WARN)
        {
            *error = 2;
            return -1;
        }
        r = archive_write_header(to_write, entry);
        if (r < ARCHIVE_OK)
        {
            *error = 3;
            return -1;
        }
        else if (archive_entry_size(entry) > 0)
        {
            const void *buff;
            size_t size;
            off_t offset;

            while (1)
            {
                r = archive_read_data_block(to_be_read, &buff, &size, &offset);
                if (r == ARCHIVE_EOF)
                {
                    break;
                }
                if (r < ARCHIVE_OK)
                {
                    *error = 4;
                    return -1;
                }
                r = archive_write_data_block(to_write, buff, size, offset);
                if (r < ARCHIVE_OK)
                {
                    *error = 5;
                    return -1;
                }
            }

            if (r < ARCHIVE_OK)
            {
                *error = 5;
                return -1;
            }
            if (r < ARCHIVE_WARN)
            {
                *error = 5;
                return -1;
            }
            r = archive_write_finish_entry(to_write);
            if (r < ARCHIVE_OK)
            {
                *error = 6;
                return -1;
            }
            if (r < ARCHIVE_WARN)
            {
                *error = 6;
                return -1;
            }
        }
    }
    archive_read_close(to_be_read);
    archive_read_free(to_be_read);
    archive_write_close(to_write);
    archive_write_free(to_write);
    return 0;
}




