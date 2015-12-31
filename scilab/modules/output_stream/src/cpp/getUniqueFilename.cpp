/*--------------------------------------------------------------------------*/
/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <fstream>
#include <iostream>
#include <sstream>
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
#include <unistd.h>
#endif
extern "C"
{
#include "machine.h"
#include "PATH_MAX.h"
#include "sci_malloc.h"
#include "getFullFilename.h"
#include "splitpath.h"
#include "FileExist.h"
#include "charEncoding.h"
}
/*--------------------------------------------------------------------------*/
static int getFileSize(const std::string& _filename);
/*--------------------------------------------------------------------------*/
std::string getUniqueFilename(const std::string& _filename)
{
    char* s = getFullFilename(_filename.data());
    std::string fullfilename(s);
    FREE(s);
    std::string newfilename;

    if (FileExist(fullfilename.data()))
    {
        char drive[PATH_MAX];
        char directory[PATH_MAX];
        char name[PATH_MAX];
        char ext[PATH_MAX];
        unsigned int id = (unsigned int)(-1);

        splitpath(fullfilename.data(), FALSE, drive, directory, name, ext);
        std::string prefixFilename(drive);
        prefixFilename += directory;
        prefixFilename += name;

        do
        {
            id++;
            newfilename = prefixFilename + "_" + std::to_string(id) + ext;
        } while (FileExist(newfilename.data()) && getFileSize(newfilename) != 0);
    }
    else
    {
        newfilename = fullfilename;
    }
    return newfilename;
}
/*--------------------------------------------------------------------------*/
static int getFileSize(const std::string& _filename)
{
    std::ifstream file(_filename.c_str());
    file.seekg(std::ios::end);
    if(file.is_open())
    {
        int size = (int)file.tellg();
        file.close();
        return size;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
