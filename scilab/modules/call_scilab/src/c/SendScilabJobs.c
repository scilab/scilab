/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2009-2010 - DIGITEO - Allan CORNET
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
#include <stdio.h>
#include <string.h>
#include "call_scilab.h"
#include "sci_malloc.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "os_string.h"
#include "api_scilab.h"
#include "call_scilab_engine_state.h"
#include "InitScilab.h"

/*--------------------------------------------------------------------------*/
static BOOL RemoveCharsFromEOL(char *line, char CharToRemove);
static BOOL RemoveComments(char *line);
static BOOL CleanBuffers(char *bufCommands, char **LOCALJOBS, int numberjobs);
static BOOL SetLastJob(char *JOB);
static char *lastjob = NULL;

/*--------------------------------------------------------------------------*/
/* see call_scilab.h more information */
/*--------------------------------------------------------------------------*/
int SendScilabJob(char *job)
{
    SciErr sciErr;
    int retCode = -1;
    char *command = NULL;

#define COMMAND_EXECSTR  "Err_Job = execstr(TMP_EXEC_STRING,\"errcatch\");"
#define COMMAND_CLEAR "clear TMP_EXEC_STRING;clear Err_Job;"

    if (getCallScilabEngineState() == CALL_SCILAB_ENGINE_STOP)
    {
        fprintf(stderr, "Error: SendScilabJob call_scilab engine not started.\n");
        return retCode;
    }

    command = os_strdup(job);

    if (command)
    {
        double Err_Job = 0.;
        int m = 0, n = 0;
        ScilabEngineInfo* pSEI = InitScilabEngineInfo();

        SetLastJob(command);

        /* Creation of a temp variable in Scilab which contains the command */
        sciErr = createNamedMatrixOfString(NULL, "TMP_EXEC_STRING", 1, 1, (char const * const*) &command);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            /* Problem */
            fprintf(stderr, "Error: SendScilabJob (1) call_scilab failed to create the temporary variable 'TMP_EXEC_STRING'.\n");
            retCode = -1;

            if (command)
            {
                FREE(command);
                command = NULL;
            }

            FREE(pSEI);
            return retCode;
        }

        /* Run the command within an execstr */
        pSEI->pstExec = COMMAND_EXECSTR;
        ExecExternalCommand(pSEI);

        sciErr = getNamedVarDimension(NULL, "Err_Job", &m, &n);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            fprintf(stderr, "Error: SendScilabJob (2) call_scilab failed to detect the temporary variable 'Err_Job' size.\n");
            retCode = -2;

            if (command)
            {
                FREE(command);
                command = NULL;
            }

            FREE(pSEI);
            return retCode;
        }

        if ((m != 1) && (n != 1))
        {
            fprintf(stderr, "Error: SendScilabJob (3) call_scilab detected a badly formated 'Err_Job' variable. Size [1,1] expected.\n");
            retCode = -3;

            if (command)
            {
                FREE(command);
                command = NULL;
            }

            FREE(pSEI);
            return retCode;
        }

        sciErr = readNamedMatrixOfDouble(NULL, "Err_Job", &m, &n, &Err_Job);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            fprintf(stderr, "Error: SendScilabJob (4) call_scilab failed to read the temporary variable 'Err_Job'.\n");
            retCode = -4;

            if (command)
            {
                FREE(command);
                command = NULL;
            }

            FREE(pSEI);
            return retCode;
        }

        if (command)
        {
            FREE(command);
            command = NULL;
        }

        retCode = (int)Err_Job;

        /* clear prev. Err , TMP_EXEC_STRING scilab variables */
        pSEI->pstExec = COMMAND_CLEAR;
        ExecExternalCommand(pSEI);
        FREE(pSEI);
    }
    else
    {
        fprintf(stderr, "Error: SendScilabJob (5) call_scilab failed to create the 'command' variable (MALLOC).\n");
        retCode = -4;
    }

    return retCode;
}

/*--------------------------------------------------------------------------*/
static BOOL SetLastJob(char *JOB)
{
    if (lastjob)
    {
        FREE(lastjob);
        lastjob = NULL;
    }

    if (JOB)
    {
        lastjob = os_strdup(JOB);
        if (lastjob)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*--------------------------------------------------------------------------*/
BOOL GetLastJob(char *JOB, int nbcharsJOB)
{
    if (JOB)
    {
        if ((int)strlen(lastjob) < nbcharsJOB)
        {
            strcpy(JOB, lastjob);
        }
        else
        {
            strncpy(JOB, lastjob, nbcharsJOB);
        }
        return TRUE;
    }
    return FALSE;
}

/*--------------------------------------------------------------------------*/
int SendScilabJobs(char **jobs, int numberjobs)
{
#define BUFFERSECURITYSIZE 64

    int retcode = -10;

    if (jobs)
    {
        int i = 0;
        int nbcharsjobs = 0;
        char *bufCommands = NULL;
        char **LOCALJOBS = NULL;

        int jobsloop = 0;

        LOCALJOBS = (char **)MALLOC(sizeof(char *) * numberjobs);

        if (LOCALJOBS)
        {
            for (i = 0; i < numberjobs; i++)
            {
                if (jobs[i])
                {
                    nbcharsjobs = nbcharsjobs + (int)strlen(jobs[i]);
                    LOCALJOBS[i] = (char *)MALLOC(sizeof(char) * (strlen(jobs[i]) + BUFFERSECURITYSIZE));
                    if (LOCALJOBS[i])
                    {
                        strcpy(LOCALJOBS[i], jobs[i]);
                    }
                    else
                    {
                        CleanBuffers(bufCommands, LOCALJOBS, numberjobs);
                        fprintf(stderr, "Error: SendScilabJobs (1) 'LOCALJOBS[%d] MALLOC'.\n", i);
                        return retcode;
                    }
                }
                else
                {
                    fprintf(stderr, "Error: SendScilabJobs (2) 'jobs[%d] == NULL'.\n", i);
                    FREE(LOCALJOBS);
                    return retcode;
                }
            }

            bufCommands = (char *)MALLOC(sizeof(char) * (nbcharsjobs + numberjobs + BUFFERSECURITYSIZE));

            if (bufCommands)
            {
                strcpy(bufCommands, "");

                for (jobsloop = 0; jobsloop < numberjobs; jobsloop++)
                {
                    if (jobs[jobsloop])
                    {
                        char *currentline = NULL;
                        BOOL AddSemiColon;

                        if (jobsloop == 0)
                        {
                            AddSemiColon = FALSE;
                        }
                        else
                        {
                            AddSemiColon = TRUE;
                        }

DOTDOTLOOP:
                        currentline = LOCALJOBS[jobsloop];

                        RemoveCharsFromEOL(currentline, '\n');
                        RemoveComments(currentline);
                        RemoveCharsFromEOL(currentline, ' ');

                        if (RemoveCharsFromEOL(currentline, '.'))
                        {
                            RemoveCharsFromEOL(currentline, ' ');
                            strcat(bufCommands, currentline);
                            jobsloop++;
                            AddSemiColon = FALSE;
                            goto DOTDOTLOOP;
                        }
                        else
                        {
                            if (!AddSemiColon)
                            {
                                strcat(currentline, ";");
                            }
                            else
                            {
                                strcat(bufCommands, ";");
                            }

                            strcat(bufCommands, currentline);
                        }
                    }
                }

                retcode = SendScilabJob(bufCommands);
                CleanBuffers(bufCommands, LOCALJOBS, numberjobs);
            }
            else
            {
                CleanBuffers(bufCommands, LOCALJOBS, numberjobs);
                fprintf(stderr, "Error: SendScilabJobs (3) 'bufCommands MALLOC'.\n");
                return retcode;
            }
        }
        else
        {
            CleanBuffers(bufCommands, LOCALJOBS, numberjobs);
            fprintf(stderr, "Error: SendScilabJobs (4) 'LOCALJOBS == NULL'.\n");
            return retcode;
        }
    }
    else
    {
        fprintf(stderr, "Error: SendScilabJobs (5) 'jobs == NULL'.\n");
        retcode = -10;
    }

    return retcode;
}

/*--------------------------------------------------------------------------*/
static BOOL RemoveCharsFromEOL(char *line, char CharToRemove)
{
    int l = 0;
    BOOL bOK = FALSE;
    int len = 0;

    len = (int)strlen(line);
    for (l = (len - 1); l > 0; l--)
    {
        if (line[l] == CharToRemove)
        {
            line[l] = '\0';
            bOK = TRUE;
        }
        else
        {
            break;
        }
    }
    return bOK;
}

/*--------------------------------------------------------------------------*/
static BOOL RemoveComments(char *line)
{
    int l = 0;
    int len = 0;
    int idx = -1;

    len = (int)strlen(line);
    for (l = len - 1; l > 0; l--)
    {
        if (line[l] == '/')
        {
            if (l - 1 >= 0)
            {
                if (line[l - 1] == '/')
                {
                    idx = l - 1;
                    l = l - 2;
                }
            }
        }
    }

    if (idx >= 0)
    {
        line[idx] = '\0';
    }

    return FALSE;
}

/*--------------------------------------------------------------------------*/
static BOOL CleanBuffers(char *bufCommands, char **LOCALJOBS, int numberjobs)
{
    if (bufCommands)
    {
        FREE(bufCommands);
        bufCommands = NULL;
    }
    freeArrayOfString(LOCALJOBS, numberjobs);
    return TRUE;
}

/*--------------------------------------------------------------------------*/
