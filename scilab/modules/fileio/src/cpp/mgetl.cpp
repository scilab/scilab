/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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

#include <iostream>
#include <fstream>

extern "C"
{
#include "mgetl.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
#include "sci_malloc.h"
#include "sciprint.h"
}
#include "filemanager.hxx"

#include <string.h>
#include <stdio.h>

#ifdef _MSC_VER
#include <Windows.h>
#endif

#ifdef BUFFER_SIZE
#undef BUFFER_SIZE
#endif
#define BUFFER_SIZE 4096

static const unsigned char UTF8_BOM[] = {0xEF, 0xBB, 0xBF, 0x00};

int mgetl(int iFileID, int iLineCount, wchar_t ***pwstLines)
{
    *pwstLines = NULL;

    // get file descriptor
    types::File* pFile = FileManager::getFile(iFileID);
    FILE *fd;
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

    std::list<std::string> lst;

    bool lineReach = false;
    std::string previous;
    size_t offset = 0;
    while (lst.size() < iLineCount && ifs.eof() == false)
    {
        int delimiter_size = 1;
        size_t sp = previous.size();
#define MAX_READ_LEN 262144
        char buf[MAX_READ_LEN + 1] = {0};
        ifs.read(buf, MAX_READ_LEN);
        size_t s = strlen(buf);
        //extract lines
        char* ptr = buf;
        for (int i = 0; i < s; ++i)
        {
            if (buf[i] == '\n')
            {
                //delimit line
                buf[i] = '\0';
                if (i > 0 && buf[i - 1] == '\r')
                {
                    buf[i - 1] = '\0';
                    delimiter_size = 2;
                }

                //add line to list
                if (sp)
                {
                    previous += ptr;
                    lst.push_back(previous);
#ifdef _MSC_VER
                    offset += previous.size() + 2;
#else
                    offset += previous.size() + delimiter_size;
#endif
                    previous.clear();
                }
                else
                {
                    lst.emplace_back(ptr);
#ifdef _MSC_VER
                    offset += strlen(ptr) + 2;
#else
                    offset += strlen(ptr) + delimiter_size;
#endif
                }

                //move ptr to first next line char
                ptr = buf + i + 1;

                if (iLineCount != -1 && lst.size() >= iLineCount)
                {
                    //rewind
#ifndef _MSC_VER
                    auto t = ifs.tellg();
#else
                    std::fpos_t t = ifs.tellg().seekpos();
#endif
                    if (t <= 0)
                    {
                        ifs.clear();
                    }

                    ifs.seekg(orig + offset, std::ios::beg);
                    lineReach = true;
                    break;
                }
            }
        }

        if (ptr == buf)
        {
            //long line
            previous += buf;
        }
        else if (lineReach == false)
        {
            int offset = (int)(buf + s - ptr);
            if (offset)
            {
                if (!ifs.eof())
                {
                    //some data stay in buf, rewind file to begin of this data and read it again
                    ifs.seekg(-offset, std::ios::cur);
                }
                else
                {
                    //some data stay in buf but oef is reached, add ptr data in list
                    std::string str(ptr);
                    lst.push_back(str);
                }
            }
        }
    }

    if (previous.size())
    {
        lst.push_back(previous);
        previous.clear();
    }

    int nbLinesOut = (int)lst.size();
    if (nbLinesOut == 0)
    {
        return 0;
    }

    *pwstLines = (wchar_t**)MALLOC(nbLinesOut * sizeof(wchar_t**));
    if (*pwstLines == NULL)
    {
        return -1;
    }

    for (int i = 0; i < nbLinesOut; ++i)
    {
        std::string s = lst.front();
        (*pwstLines)[i] = to_wide_string(s.data());
        lst.pop_front();
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
