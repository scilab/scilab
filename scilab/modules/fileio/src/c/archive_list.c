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

#include<archive.h>
#include<stdio.h>
#include<stdlib.h>
#include<archive_entry.h>
#include<string.h>
#include "MALLOC.h"

char** archive_list(char *filename, int *size, int *error)
{
    struct archive *a;
    struct archive_entry *entry;
    int r;
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    r = archive_read_open_filename(a, filename, 10240);    //Opening the archive to read the headers
    char **file_list;


    if (r != ARCHIVE_OK) //If the archive could not be opened
    {
        *error = 1;
        return file_list;
    }
    *size = 0;


    while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
    {   //Iterating over the archive header to get filenames
        *size = *size + 1;
        if((*size) == 1)
        {
            file_list = (char**)MALLOC(sizeof(char*)*(*size));
        }
        else
        {
            file_list = (char**)REALLOC(file_list,sizeof(char*)*(*size));
        }
        file_list[(*size)-1] = (char*)MALLOC(sizeof(char)*1024);
        file_list[(*size)-1] = NULL;
        file_list[(*size)-1] = strdup(archive_entry_pathname(entry));
        archive_read_data_skip(a);
    }


    r = archive_read_free(a); 
    if (r != ARCHIVE_OK) //If the archive struct could not be freed properly
    {
        *error = 2;
        return file_list;
    }


    return file_list;
}
