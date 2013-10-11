/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <windows.h>
#include "strdup_windows.h"
#endif
#include "mgetl.h"
#include "filesmanagement.h"
#include "mopen.h"
#include "MALLOC.h"
#include "BOOL.h"
#include "strsubst.h"
#include "charEncoding.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
#define LINE_MAX 4096
#define CR '\r'
#define LF '\n'
#define EMPTYSTR ""
/*--------------------------------------------------------------------------*/
static char *removeEOL(char *_inString);
static char *convertAnsiToUtf(char *_inString);
static char *getNextLine(FILE *stream);
/*--------------------------------------------------------------------------*/
static const unsigned char UTF8BOM_BYTEORDER_MARK[] = {0xEF, 0xBB, 0xBF, 0x00};
/*--------------------------------------------------------------------------*/
char **mgetl(int fd, int nbLinesIn, int *nbLinesOut, int *ierr)
{
    char **strLines = NULL;
    FILE *fa = NULL;

    *ierr = MGETL_ERROR;
    *nbLinesOut = 0;

    if (fd == STDIN_ID)
    {
        fa = stdin;
    }
    else
    {
        fa = GetFileOpenedInScilab(fd);
    }

    if (fa)
    {
        char *Line = NULL;
        int nbLines = 0;

        if (nbLinesIn < 0)
        {
            strLines = (char **)MALLOC(sizeof(char *));
            if (strLines == NULL)
            {
                *nbLinesOut = 0;
                *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                return NULL;
            }

            Line = getNextLine(fa);
            if (Line)
            {
                /* UTF-8 BOM */
                if (strncmp(Line, (const char*)UTF8BOM_BYTEORDER_MARK, strlen((const char*)UTF8BOM_BYTEORDER_MARK)) == 0)
                {
                    /* we skip first characters */
                    char *tmpLine = strsub(Line, (const char*)UTF8BOM_BYTEORDER_MARK, "");
                    FREE(Line);
                    Line = tmpLine;
                }
            }

            while ( Line != NULL )
            {
                nbLines++;
                strLines = (char **)REALLOC(strLines, nbLines * sizeof(char *));
                if (strLines == NULL)
                {
                    FREE(Line);
                    *nbLinesOut = 0;
                    *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                    return NULL;
                }

                strLines[nbLines - 1] = convertAnsiToUtf(removeEOL(Line));

                FREE(Line);

                if (strLines[nbLines - 1] == NULL)
                {
                    *nbLinesOut = 0;
                    *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                    freeArrayOfString(strLines, nbLines);
                    return NULL;
                }

                Line = getNextLine(fa);
            }

            *nbLinesOut = nbLines;
            *ierr = MGETL_NO_ERROR;
        }
        else
        {
            if (nbLinesIn == 0)
            {
                *ierr = MGETL_EOF;
                *nbLinesOut = 0;
                if (strLines)
                {
                    FREE(strLines);
                }
                strLines = NULL;
            }
            else
            {
                BOOL bContinue = TRUE;
                BOOL bEOF = FALSE;
                strLines = (char **)MALLOC(sizeof(char *) * nbLinesIn);

                if (strLines == NULL)
                {
                    *nbLinesOut = 0;
                    *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                    return NULL;
                }

                do
                {
                    if (nbLines < nbLinesIn)
                    {
                        if ((double) ftell(fa) == 0)
                        {
                            Line = getNextLine(fa);
                            /* UTF-8 BOM */
                            if (Line && (strncmp(Line, (const char*)UTF8BOM_BYTEORDER_MARK, strlen((const char*)UTF8BOM_BYTEORDER_MARK)) == 0))
                            {
                                /* we skip first characters */
                                char *tmpLine = strsub(Line, (const char*)UTF8BOM_BYTEORDER_MARK, "");
                                FREE(Line);
                                Line = tmpLine;
                            }
                        }
                        else
                        {
                            Line = getNextLine(fa);
                        }

                        if (Line != NULL)
                        {
                            nbLines++;
                            strLines[nbLines - 1] = convertAnsiToUtf(removeEOL(Line));

                            if (Line)
                            {
                                FREE(Line);
                                Line = NULL;
                            }

                            if (strLines[nbLines - 1] == NULL)
                            {
                                freeArrayOfString(strLines, nbLines);
                                *nbLinesOut = 0;
                                *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                                return NULL;
                            }
                        }
                        else
                        {
                            /* EOF */
                            if (feof(fa))
                            {
                                bEOF = TRUE;
                            }
                            bContinue = FALSE;
                        }
                    }
                    else
                    {
                        bContinue = FALSE;
                    }
                }
                while (bContinue);

                if (Line)
                {
                    FREE(Line);
                    Line = NULL;
                }

                *nbLinesOut = nbLines;
                if (bEOF)
                {
                    *ierr = MGETL_EOF;
                }
                else
                {
                    *ierr = MGETL_NO_ERROR;
                }
            }
        }
        if (Line)
        {
            FREE(Line);
            Line = NULL;
        }
    }

    return strLines;
}
/*--------------------------------------------------------------------------*/
static char *removeEOL(char *_inString)
{
    if (_inString)
    {
        char *pos = strchr(_inString, LF);
        if (pos)
        {
            *pos = 0;
        }

        pos = strchr(_inString, CR);
        if (pos)
        {
            *pos = 0;
        }
    }
    return _inString;
}
/*--------------------------------------------------------------------------*/
/*
* convert ansi to Utf
*/
static char *convertAnsiToUtf(char *_inString)
{
    char *outString = NULL;
    if (_inString)
    {
#ifdef _MSC_VER
        if (IsValidUTF8(_inString))
        {
            outString = strdup(_inString);
        }
        else
        {
            /* conversion ANSI to UTF */
            int Len = 0;
            int newLen = 0;
            BSTR bstrCode = NULL;

            Len = MultiByteToWideChar(CP_ACP, 0, _inString, lstrlen(_inString), NULL, NULL);
            bstrCode = SysAllocStringLen(NULL, Len);
            if (bstrCode)
            {
                MultiByteToWideChar(CP_ACP, 0, _inString, lstrlen(_inString), bstrCode, Len);
                newLen = WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, outString, 0, NULL, NULL);
                outString = (char*) MALLOC(newLen + 1);
                if (outString)
                {
                    WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, outString, newLen, NULL, NULL);
                }
                else
                {
                    outString = strdup(_inString);
                }
                SysFreeString(bstrCode);
                bstrCode = NULL;
            }
            else
            {
                outString = strdup(_inString);
            }
        }
#else
        if (IsValidUTF8(_inString))
        {
            outString = strdup(_inString);
        }
        else
        {
            int len = (int)strlen(_inString);
            int i = 0;

            outString = (char*)MALLOC(((len * 3) + 1) * sizeof(char));
            if (outString == NULL)
            {
                return NULL;
            }
            strcpy(outString, EMPTYSTR);

            for (i = 0; i < len; i++)
            {
                unsigned char *outUtfChar = NULL;
                unsigned char inAnsiChar = 0;

                if (_inString[i] < 0)
                {
                    inAnsiChar = 256 + _inString[i];
                }
                else
                {
                    inAnsiChar = _inString[i];
                }

                if (inAnsiChar < 128)
                {
                    outUtfChar = (char *)CALLOC(2, sizeof(char));
                    if (outUtfChar)
                    {
                        outUtfChar[0] = inAnsiChar;
                        outUtfChar[1] = 0;
                    }
                }
                else
                {
                    outUtfChar = (char *)CALLOC(3, sizeof(char));
                    if (outUtfChar)
                    {
                        outUtfChar[0] = (inAnsiChar >> 6) | 0xC0;
                        outUtfChar[1] = (inAnsiChar & 0x3F) | 0x80;
                        outUtfChar[2] = 0;
                    }
                }

                if (outUtfChar)
                {
                    strcat(outString, outUtfChar);
                    FREE(outUtfChar);
                    outUtfChar = NULL;
                }
            }
        }
#endif
    }
    return outString;
}
/*--------------------------------------------------------------------------*/
static char *getNextLine(FILE *stream)
{
    char *bufferLine = NULL;
    if (stream != NULL)
    {
        char *pCurrentLine = NULL;
        char tmp[LINE_MAX] = EMPTYSTR;

        size_t size = 1;

        while (fgets (tmp, LINE_MAX, stream) != NULL)
        {
            size += LINE_MAX;
            pCurrentLine = (char*)REALLOC (bufferLine, sizeof (char) * size);
            if (pCurrentLine != NULL)
            {
                if (bufferLine == NULL)
                {
                    pCurrentLine[0] = '\0';
                }
                bufferLine = pCurrentLine;
                pCurrentLine = NULL;
                strcat (bufferLine, tmp);

                if (bufferLine[strlen(bufferLine) - 1] == '\n')
                {
                    bufferLine[strlen(bufferLine) - 1] = '\0';
                    break;
                }
            }
            else
            {
                FREE (bufferLine);
                bufferLine = NULL;
            }
        }
    }
    return bufferLine;
}
/*--------------------------------------------------------------------------*/
