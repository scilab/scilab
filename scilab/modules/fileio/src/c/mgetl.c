/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "mgetl.h"
#include "filesmanagement.h"
#include "mopen.h"
#include "MALLOC.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
#define LINE_MAX 4096
#define CR 13
#define LF 10
#define EMPTYSTR ""
/*--------------------------------------------------------------------------*/
char **mgetl(int fd, int nbLinesIn, int *nbLinesOut, int *ierr)
{
    char **wcLines = NULL;
    FILE *fa = NULL;

    *ierr = MGETL_ERROR;
    *nbLinesOut = 0;

    fa = GetFileOpenedInScilab(fd);

    if (fa)
    {
        char Line[LINE_MAX * 2];
        int nbLines = 0;

        if (nbLinesIn < 0)
        {
            wcLines = (char **)MALLOC(sizeof(char *));
            if (wcLines == NULL)
            {
                *nbLinesOut = 0;
                *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                return NULL;
            }

            while ( fgets ( Line, sizeof(Line), fa ) != NULL )
            {
                int wcLen = (int)strlen(Line);
                nbLines++;
                wcLines = (char **)REALLOC(wcLines, nbLines * sizeof(char *));
                if (wcLines == NULL)
                {
                    *nbLinesOut = 0;
                    *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                    return NULL;
                }

                wcLines[nbLines - 1] = (char*)MALLOC(sizeof(char) * (wcLen + 1)); 
                if (wcLines[nbLines - 1] == NULL)
                {
                    *nbLinesOut = 0;
                    *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                    return NULL;
                }

                if (wcLen >= 1)
                {
                    /* remove EOL */
                    if ( (Line[wcLen - 1] == CR) || (Line[wcLen - 1] == LF) )
                    {
                        Line[wcLen - 1] = 0;
                    }
                }

                strcpy(wcLines[nbLines - 1], Line);
                strcpy(Line, EMPTYSTR);
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
                if (wcLines) 
                {
                    FREE(wcLines);
                }
                wcLines = NULL;
            }
            else
            {
                BOOL bContinue = TRUE;
                wcLines = (char **)MALLOC(sizeof(char *) * nbLinesIn);
                if (wcLines == NULL)
                {
                    *nbLinesOut = 0;
                    *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                    return NULL;
                }

                do 
                {
                    if (nbLines < nbLinesIn)
                    {
                        if ( fgets ( Line, sizeof(Line), fa ) != NULL)
                        {
                            int wcLen = (int)strlen(Line);
                            if (wcLen >= 1)
                            {
                                /* remove EOL */
                                if ( (Line[wcLen - 1] == CR) || (Line[wcLen - 1] == LF) )
                                {
                                    Line[wcLen - 1] = 0;
                                }
                            }
                            nbLines++;
                            wcLines[nbLines - 1] = (char*)MALLOC(sizeof(char) * (wcLen + 1)); 
                            if (wcLines[nbLines - 1] == NULL)
                            {
                                *nbLinesOut = 0;
                                *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                                return NULL;
                            }
                            strcpy(wcLines[nbLines - 1], Line);

                            strcpy(Line, EMPTYSTR);
                        }
                        else
                        {
                            bContinue = FALSE;
                        }
                    }
                    else
                    {
                        bContinue = FALSE;
                    }
                } while (bContinue);

                *nbLinesOut = nbLines;
                *ierr = MGETL_NO_ERROR;
            }
        }
    }
    return wcLines;
}
/*--------------------------------------------------------------------------*/
