/*--------------------------------------------------------------------------*/
/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
static void wcsplitpath(const wchar_t* path, wchar_t* drv, wchar_t* dir, wchar_t* name, wchar_t* ext);
static int getFileSize(const std::wstring& _wfilename);
/*--------------------------------------------------------------------------*/
std::wstring getUniqueFilename(const std::wstring& _wfilename)
{
    wchar_t* ws = getFullFilenameW(_wfilename.data());
    std::wstring wfullfilename(ws);
    FREE(ws);
    std::wstring newfilename;

    if (FileExistW(wfullfilename.data()))
    {
        wchar_t wcdrive[PATH_MAX];
        wchar_t wcdirectory[PATH_MAX];
        wchar_t wcname[PATH_MAX];
        wchar_t wcext[PATH_MAX];
        unsigned int id = (unsigned int)(-1);

        splitpathW(wfullfilename.data(), FALSE, wcdrive, wcdirectory, wcname, wcext);
        std::wstring prefixFilename(wcdrive);
        prefixFilename += wcdirectory;
        prefixFilename += wcname;

        do
        {
            id++;
            newfilename = prefixFilename + L"_" + std::to_wstring(id) + wcext;
        }
        while (FileExistW(newfilename.data()) && getFileSize(newfilename) != 0);
    }
    else
    {
        newfilename = wfullfilename;
    }
    return newfilename;
}
/*--------------------------------------------------------------------------*/
static int getFileSize(const std::wstring& _wfilename)
{
#ifdef _MSC_VER
    std::wifstream file(_wfilename.c_str());
#else
    char *_filename = wide_string_to_UTF8((wchar_t*)_wfilename.c_str());
    if (_filename == NULL)
    {
        return false;
    }
    std::ifstream file(_filename);
#endif
    file.seekg(std::ios::end);
    if (file.is_open())
    {
        int size = (int)file.tellg();
        file.close();
        return size;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
