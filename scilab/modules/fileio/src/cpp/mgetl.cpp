/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "filemanager.hxx"

extern "C"
{
#include "mgetl.h"
#include "filesmanagement.h"
#include "mopen.h"
#include "MALLOC.h"
#include "os_wcsdup.h"
#include "os_strdup.h"
}
/*--------------------------------------------------------------------------*/
#define LINE_MAX 4096
#define CR L'\r'
#define LF L'\n'
#define EMPTYSTRW L""
#define EMPTYSTR ""
/*--------------------------------------------------------------------------*/
static wchar_t *removeEOL(wchar_t *_inString);
static char *convertAnsiToUtf(char *_inString);
static wchar_t* getLine(wchar_t* _pstLine, int _iLineSize, File* _pFile);
/*--------------------------------------------------------------------------*/
wchar_t **mgetl(int fd, int nbLinesIn, int *nbLinesOut, int *ierr)
{
    wchar_t **strLines = NULL;
    File* pFile     = NULL;

    *ierr = MGETL_ERROR;
    *nbLinesOut = 0;

    pFile = FileManager::getFile(fd);

    if (pFile)
    {
        wchar_t Line[LINE_MAX * 2];
        int nbLines = 0;

        if (nbLinesIn < 0)
        {
            strLines = (wchar_t **)MALLOC(sizeof(wchar_t *));
            if (strLines == NULL)
            {
                *nbLinesOut = 0;
                *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                return NULL;
            }

            while ( getLine ( Line, sizeof(Line), pFile ) != NULL )
            {
                nbLines++;
                strLines = (wchar_t **)REALLOC(strLines, nbLines * sizeof(wchar_t *));
                if (strLines == NULL)
                {
                    *nbLinesOut = 0;
                    *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                    return NULL;
                }

                strLines[nbLines - 1] = os_wcsdup(removeEOL(Line)); 
                if (strLines[nbLines - 1] == NULL)
                {
                    *nbLinesOut = 0;
                    *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                    return NULL;
                }
                wcscpy(Line, EMPTYSTRW);
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
                strLines = (wchar_t **)MALLOC(sizeof(wchar_t *) * nbLinesIn);
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
                        if ( getLine ( Line, sizeof(Line), pFile) != NULL)
                        {
                            nbLines++;
                            strLines[nbLines - 1] = os_wcsdup(removeEOL(Line));
                            if (strLines[nbLines - 1] == NULL)
                            {
                                *nbLinesOut = 0;
                                *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                                return NULL;
                            }
                            wcscpy(Line, EMPTYSTRW);
                        }
                        else
                        {
                            /* EOF */
                            if (feof(pFile->getFiledesc()))
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
                } while (bContinue);

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
    }
    return strLines;
}
/*--------------------------------------------------------------------------*/
wchar_t* getLine(wchar_t* _pstLine, int _iLineSize, File* _pFile)
{
    if(static_cast<int>(_pFile->getFileModeAsDouble()) % 2 == 1)
    {//binary mode
        return fgetws(_pstLine, _iLineSize, _pFile->getFiledesc());
    }
    else
    {
        char* pstTemp = (char*)MALLOC(sizeof(char*) * _iLineSize);
        if(fgets(pstTemp, _iLineSize, _pFile->getFiledesc()) == NULL)
        {
            FREE(pstTemp);
            return NULL;
        }
        char* pstUtf = convertAnsiToUtf(pstTemp);
        wchar_t* pstTempWide = to_wide_string(pstUtf);
        wcscpy(_pstLine, pstTempWide);
        FREE(pstUtf);
        FREE(pstTemp);
        FREE(pstTempWide);
        return _pstLine;
    }
}
/*--------------------------------------------------------------------------*/
wchar_t *removeEOL(wchar_t *_inString)
{
    if (_inString)
    {
        wchar_t *pos = wcschr(_inString, LF);
        if(pos)
        {
            *pos = 0;
        }

        pos = wcschr(_inString, CR);
        if(pos)
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
char *convertAnsiToUtf(char *_inString)
{
    char *outString = NULL;
    if (_inString)
    {
#ifdef _MSC_VER
        if (IsValidUTF8(_inString))
        {
            outString = os_strdup(_inString);
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
                outString = (char*) MALLOC(sizeof(char) * (newLen + 1));
                if (outString)
                {
                    WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, outString, newLen, NULL, NULL); 
                }
                else
                {
                    outString = os_strdup(_inString);
                }
                SysFreeString(bstrCode);
                bstrCode = NULL;
            }
            else
            {
                outString = os_strdup(_inString);
            }
        }
#else
        if (IsValidUTF8(_inString))
        {
            outString = os_strdup(_inString);
        }
        else
        {
            int len = (int)strlen(_inString);
            int i = 0;

            outString = (char*)MALLOC(((len*3) + 1) * sizeof(char));
            if (outString == NULL)
            {
                return NULL;
            }

            strcpy(outString, EMPTYSTR);

            for (i = 0; i < len; i++)
            {
                char *outUtfChar = NULL;
                unsigned char inAnsiChar = 0;

                if (_inString[i] < 0)
		{
			 inAnsiChar = 256 + _inString[i];
		}
                else
		{
			 inAnsiChar = _inString[i];
		}

                if(inAnsiChar < 128)
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

