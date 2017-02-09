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

extern "C"
{
#include "mgetl.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
#include "sci_malloc.h"
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

static const unsigned char UTF8_BOM[] = { 0xEF, 0xBB, 0xBF, 0x00 };

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
        char cValues[4] = { 0x00, 0x00, 0x00, 0x00 };
        if (fgets(cValues, 4 * sizeof(char), fd) != NULL)
        {
            // skip BOM
            if (strcmp(cValues, (const char*) UTF8_BOM) != 0)
            {
                rewind(fd);
            }
        }
    }

    if (iLineCount > 0)
    {
        *pwstLines = (wchar_t**)MALLOC(iLineCount * sizeof(wchar_t*));
        if (pwstLines == NULL)
        {
            return -1;
        }
    }

    // allocate initial reading buffer, it will grow depending on line size
    int iBufferSize = BUFFER_SIZE;
    char *pstBuffer = (char *) malloc(iBufferSize * sizeof(char));
    if (pstBuffer == NULL)
    {
        freeArrayOfWideString(*pwstLines, iLineCount);
        *pwstLines = NULL;
        return -1;
    }

    int iReadLineCount = 0;
    while (fgets(pstBuffer, iBufferSize * sizeof(char), fd) != NULL)
    {
        iReadLineCount++;
        *pwstLines = (wchar_t**)REALLOC(*pwstLines, iReadLineCount * sizeof(wchar_t*));
        if (*pwstLines == NULL)
        {
            free(pstBuffer);
            return -1;
        }

        // is the line complete in the buffer (= zero terminal found) ?
        int len = strnlen(pstBuffer, iBufferSize);
        int totalLen = len;
        if (len >= iBufferSize - 1)
        {
            // no, there is another data to read for this line
            // allocate a temporary buffer for reading it
            char *pstBufferTmp = (char*) malloc(BUFFER_SIZE * sizeof(char));
            if (pstBufferTmp == NULL)
            {
                freeArrayOfWideString(*pwstLines, iReadLineCount);
                *pwstLines = NULL;
                free(pstBuffer);
                return -1;
            }
            char *pstNewBuffer = pstBuffer;
            // loop until line is complete
            do
            {
                // reallocate a new bigger buffer for the line
                iBufferSize += BUFFER_SIZE;
                pstNewBuffer = (char*) REALLOC(pstNewBuffer, iBufferSize * sizeof(char));
                if (pstNewBuffer == NULL)
                {
                    freeArrayOfWideString(*pwstLines, iReadLineCount);
                    *pwstLines = NULL;
                    free(pstBufferTmp);
                    return -1;
                }

                // read the remaining data and copy it in the new buffer
                if (fgets(pstBufferTmp, BUFFER_SIZE * sizeof(char), fd) != NULL)
                {
                    len = strnlen(pstBufferTmp, BUFFER_SIZE);
                    totalLen += len;
                    strncat(pstNewBuffer, pstBufferTmp, BUFFER_SIZE);
                }
                else
                {
                    break;
                }
            }
            while (len >= BUFFER_SIZE - 1);

            free(pstBufferTmp);
            // the bigger buffer becomes the current buffer
            pstBuffer = pstNewBuffer;
        }
        if ((totalLen > 0) && (pstBuffer[totalLen - 1] == '\n'))
        {
            pstBuffer[totalLen - 1] = '\0';
            if ((totalLen > 1) && (pstBuffer[totalLen - 2] == '\r'))
            {
                pstBuffer[totalLen - 2] = '\0';
            }
        }
        // add the line in the array
        (*pwstLines)[iReadLineCount - 1] = to_wide_string(pstBuffer);

        if ((iLineCount > 0) && (iReadLineCount >= iLineCount))
        {
            break;
        }
    }

    free(pstBuffer);
    return iReadLineCount;
}

