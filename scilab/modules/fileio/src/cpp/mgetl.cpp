/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
* Copyright (C) 2019 - ESI - Antoine ELIAS
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

#include <fstream>
#include <iostream>

extern "C"
{
#include "charEncoding.h"
#include "freeArrayOfString.h"
#include "mgetl.h"
#include "sci_malloc.h"
#include "sciprint.h"
}
#include "filemanager.hxx"

#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#include <Windows.h>
#endif

static const unsigned char UTF8_BOM[] = {0xEF, 0xBB, 0xBF, 0x00};

//remove \r
inline void rtrim(char* s)
{
    size_t n = strlen(s);
    if (n && s[n - 1] == '\r')
    {
        s[n - 1] = 0;
    }
}

int mgetl(int iFileID, int iLineCount, wchar_t*** pwstLines)
{
    *pwstLines = NULL;

    // get file descriptor
    types::File* pFile = FileManager::getFile(iFileID);
    FILE* fd;
    if (pFile != NULL)
    {
        fd = pFile->getFiledesc();
    }
    else
    {
        return -1;
    }

    if (iLineCount == 0)
    {
        return 0;
    }

    // check file is not empty
    if (ftell(fd) == 0)
    {
        char cValues[4] = {0x00, 0x00, 0x00, 0x00};
        if (fgets(cValues, 4 * sizeof(char), fd) != NULL)
        {
            // skip BOM
            if (strcmp(cValues, (const char*)UTF8_BOM) != 0)
            {
                rewind(fd);
            }
        }
    }

    int orig = ftell(fd);

#ifndef _MSC_VER
    //must reopen the file
    std::wstring wname = pFile->getFilename();
    char* name = wide_string_to_UTF8(wname.data());
    std::ifstream ifs(name);
    FREE(name);
#else
    std::ifstream ifs(fd);
#endif
    //seek to same position
    ifs.seekg(orig);

#define MAX_READ_LEN 262144
    char str[MAX_READ_LEN];
    std::vector<wchar_t*> lst;

    while ((iLineCount == -1 || lst.size() < iLineCount) && ifs.getline(str, MAX_READ_LEN))
    {
        rtrim(str);
        lst.push_back(to_wide_string(str));
    }

    int nbLinesOut = (int)lst.size();
    if (nbLinesOut == 0)
    {
        return 0;
    }

    *pwstLines = (wchar_t**)MALLOC(nbLinesOut * sizeof(wchar_t*));

    if (*pwstLines == NULL)
    {
        return -1;
    }

    for (int i = 0; i < nbLinesOut; ++i)
    {
        (*pwstLines)[i] = lst[i];
    }

#ifndef _MSC_VER
    auto pos = ifs.tellg();
    if (pos == -1)
    {
        fseek(fd, 0, SEEK_END);
    }
    else
    {
        fseek(fd, pos, SEEK_SET);
    }

    ifs.close();
#endif

    return nbLinesOut;
}
