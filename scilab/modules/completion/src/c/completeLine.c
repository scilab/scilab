/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009-2010 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Vincent LEJEUNE
* Copyright (C) 2011 - DIGITEO - Allan CORNET
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
#include <string.h>
#include <ctype.h>
#include "completeLine.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "getPartLine.h"
#include "splitpath.h"
#include "PATH_MAX.h"
#include "stristr.h"

/*--------------------------------------------------------------------------*/
/*!  Get the position of the longest suffix of string that match with a prefix of find
*  @param[in] string  A string that has a suffix that match a prefix of find ; Assumed to be non null because of the first guard in completeLine
*  @param[in] find A string that has a prefix that match with a suffix of string
*  @return Position in string where suffix of string and prefix of find does match
*
*/
static int findMatchingPrefixSuffix(const char* string, const char* find, BOOL stringToAddIsPath)
{
    char* pointerOnString = NULL;
    char* pointerOnFindCopy = NULL;
    char* movingPointerOnFindCopy = NULL;
    char lastchar;
    size_t stringLength = 0;

    //get a working copy of find
    pointerOnFindCopy = os_strdup(find);
    //last character of string
    lastchar = *(string + strlen(string) - 1);
    stringLength = strlen(string);

    // Convert to upper-case
    {
        char* str;
        for (str = pointerOnFindCopy; *str != '\0'; str++)
        {
            *str = toupper(*str);
        }
    }

    //Tips : no infinite loop there, tmpfind string length is always reduced at each iteration
    movingPointerOnFindCopy = strrchr(pointerOnFindCopy, toupper(lastchar));

    while ( movingPointerOnFindCopy )
    {
        //find the last occurence of last char of string in tmpfind
        movingPointerOnFindCopy = strrchr(pointerOnFindCopy, toupper(lastchar));
        if (movingPointerOnFindCopy == NULL)
        {
            break;
        }
        // Cut tmpfind at this position
        movingPointerOnFindCopy[0] = '\0';
        //Check if the cutted tmpfind match with the suffix of string that has adequat length
        pointerOnString = (char*)(string + stringLength - 1 - strlen(pointerOnFindCopy));
        if ( !strnicmp(pointerOnFindCopy, pointerOnString, strlen(pointerOnFindCopy)) )
        {
            FREE(pointerOnFindCopy);
            pointerOnFindCopy = NULL;
            return (int)(pointerOnString - string);
        }

    }
    //if no return, no position is correct, return last char of string.
    FREE(pointerOnFindCopy);
    pointerOnFindCopy = NULL;
    return (int)stringLength;
}
/*--------------------------------------------------------------------------*/
char *completeLine(char *currentline, char *stringToAdd, char *filePattern,
                   char *defaultPattern, BOOL stringToAddIsPath, char *postCaretLine)
{
    char *new_line = NULL;
    int lengthNewLine = 0;

    char *stringToAddAtTheEnd = NULL;
    int lenstringToAddAtTheEnd = 0;

    char *res = NULL;

    int lencurrentline = 0;

    int iposInsert = 0;

    if (currentline == NULL)
    {
        return  os_strdup("");
    }
    lencurrentline = (int)strlen(currentline);

    if (postCaretLine == NULL)
    {
        stringToAddAtTheEnd = os_strdup("");
        lenstringToAddAtTheEnd = (int)strlen(stringToAddAtTheEnd);
    }
    else
    {
        stringToAddAtTheEnd = os_strdup(postCaretLine);
        lenstringToAddAtTheEnd = (int)strlen(stringToAddAtTheEnd);
    }

    if ( (stringToAdd == NULL)  || (strcmp(stringToAdd, "") == 0) )
    {
        lengthNewLine = lencurrentline + lenstringToAddAtTheEnd;
        new_line = (char*)MALLOC(sizeof(char) * (lengthNewLine + 1));
        if (new_line)
        {
            strcpy(new_line, currentline);
            strcat(new_line, stringToAddAtTheEnd);
        }

        FREE(stringToAddAtTheEnd);
        stringToAddAtTheEnd = NULL;

        return new_line;
    }

    if (stringToAddIsPath == FALSE)
    {
        char *filePatternBuf = NULL;
        BOOL bfilePatternBuf = FALSE;

        if (filePattern != NULL)
        {
            filePatternBuf = filePattern;
        }
        else
        {
            filePatternBuf = getFilePartLevel(currentline);
            bfilePatternBuf = TRUE;
        }

        if (filePatternBuf)
        {
            char* drv = (char*)MALLOC(sizeof(char) * (PATH_MAX + 1));
            char* dir = (char*)MALLOC(sizeof(char) * (PATH_MAX + 1));
            char* name = (char*)MALLOC(sizeof(char) * (PATH_MAX + 1));
            char* ext = (char*)MALLOC(sizeof(char) * (PATH_MAX + 1));

            splitpath(filePatternBuf, TRUE, drv, dir, name, ext);

            if (bfilePatternBuf)
            {
                FREE(filePatternBuf);
                filePatternBuf = NULL;
            }

            if ( strcmp(drv, "") || strcmp(dir, "") )
            {
                /* bug 4365 */
                /*cd SCI/modules/arnoldi/nonreg_tes */

                FREE(drv);
                drv = NULL;
                FREE(dir);
                dir = NULL;
                FREE(name);
                name = NULL;
                FREE(ext);
                ext = NULL;

                lengthNewLine = lencurrentline + lenstringToAddAtTheEnd;
                new_line = (char*)MALLOC(sizeof(char) * (lengthNewLine + 1));
                if (new_line)
                {
                    strcpy(new_line, currentline);
                    strcat(new_line, stringToAddAtTheEnd);
                }

                FREE(stringToAddAtTheEnd);
                stringToAddAtTheEnd = NULL;

                return new_line;
            }

            FREE(drv);
            drv = NULL;
            FREE(dir);
            dir = NULL;
            FREE(name);
            name = NULL;
            FREE(ext);
            ext = NULL;
        }
    }

    // Does the end of line (currentLine) matches the beginning of completed word (stringToAdd) ?
    iposInsert = findMatchingPrefixSuffix(currentline, stringToAdd, stringToAddIsPath);
    res = stristr(stringToAdd, &currentline[iposInsert]);

    if (res == NULL)
    {
        // No, find the last occurence of completed word word in line
        char* foundCompletedWordPtr = NULL;
        char* nextFoundCompletedWordPtr = stristr(currentline, stringToAdd);
        while (nextFoundCompletedWordPtr)
        {
            foundCompletedWordPtr = nextFoundCompletedWordPtr;
            nextFoundCompletedWordPtr =
                stristr(foundCompletedWordPtr + strlen(foundCompletedWordPtr), stringToAdd);
        }

        if (foundCompletedWordPtr)
        {
            iposInsert = (int) (foundCompletedWordPtr - currentline);
        }
    }

    // if it is a path, we add at the end
    if ((currentline[lencurrentline - 1] == '/') || (currentline[lencurrentline - 1] == '\\'))
    {
        iposInsert = lencurrentline;
    }

    lengthNewLine = (int)(strlen(currentline) + strlen(stringToAdd) + lenstringToAddAtTheEnd);
    new_line = (char*)MALLOC(sizeof(char) * (lengthNewLine + 1));
    if (new_line)
    {
        strcpy(new_line, currentline);
        new_line[iposInsert] = 0;

        strcat(new_line, stringToAdd);
        strcat(new_line, stringToAddAtTheEnd);
    }

    FREE(stringToAddAtTheEnd);
    stringToAddAtTheEnd = NULL;

    return new_line;
}
/*--------------------------------------------------------------------------*/
