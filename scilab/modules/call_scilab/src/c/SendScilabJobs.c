/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include <string.h>
#include "CallScilab.h"
#include "MALLOC.h"
#include "scirun.h"
#include "localization.h"
#include "stack-c.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "api_common.h"
#include "api_string.h"
#include "api_double.h"
/*--------------------------------------------------------------------------*/
static BOOL RemoveCharsFromEOL(char *line,char CharToRemove);
static BOOL RemoveComments(char *line);
static BOOL CleanBuffers(char *bufCommands,char **LOCALJOBS,int numberjobs);
static BOOL SetLastJob(char *JOB);
static char *lastjob = NULL;
/*--------------------------------------------------------------------------*/
/* see CallScilab.h more informations*/
/*--------------------------------------------------------------------------*/
int SendScilabJob(char *job)
{
	int retCode = -1;
	int lencommand = 0;
	char *command = NULL;

#define COMMAND_EXECSTR  "Err_Job = execstr(TMP_EXEC_STRING,\"errcatch\",\"n\");quit;"
#define COMMAND_CLEAR "clear TMP_EXEC_STRING;clear Err_Job;quit;"

	command = strdup(job);
	lencommand = (int)strlen(command);

	if (command)
	{ 
		double Err_Job = 0.;
		int m = 0, n = 0;

		/* clear prev. Err , TMP_EXEC_STRING scilab variables */
		C2F(scirun)(COMMAND_CLEAR,(long int)strlen(COMMAND_CLEAR));

		SetLastJob(command);

		/* Creation of a temp variable in Scilab which contains the command */

		if (createNamedMatrixOfString("TMP_EXEC_STRING", 1, 1, &command))
		{
			/* Problem */
			fprintf(stderr, "Error : SendScilabJob (1) 'TMP_EXEC_STRING'.\n");
			retCode = -1;

			if (command) {FREE(command);command=NULL;}
			lencommand = 0;

			return retCode;
		}

		/* Run the command within an execstr */
		C2F(scirun)(COMMAND_EXECSTR,(long int)strlen(COMMAND_EXECSTR));

		if (getNamedVarDimension("Err_Job", &m, &n))
		{
			fprintf(stderr,"Error : SendScilabJob (2) 'Err_Job'.\n");	
			retCode = -2;

			if (command) {FREE(command);command=NULL;}
			lencommand = 0;

			return retCode;
		}

		if ( (m != 1) && (n != 1) )
		{
			fprintf(stderr,"Error : SendScilabJob (3) 'Err_Job'.\n");	
			retCode = -3;

			if (command) {FREE(command);command=NULL;}
			lencommand = 0;

			return retCode;
		}

		if (readNamedMatrixOfDouble("Err_Job", &m, &n, &Err_Job))
		{
			fprintf(stderr,"Error : SendScilabJob (4) 'Err_Job'.\n");	
			retCode = -4;

			if (command) {FREE(command);command=NULL;}
			lencommand = 0;

			return retCode;
		}

		if (command) {FREE(command);command=NULL;}
		lencommand = 0;

		retCode = (int)Err_Job;

		/* clear prev. Err , TMP_EXEC_STRING scilab variables */
		C2F(scirun)(COMMAND_CLEAR,(long int)strlen(COMMAND_CLEAR));;
	}
	else
	{
		fprintf(stderr,"Error : SendScilabJob (4) 'command' MALLOC.\n");
		retCode = -4;
	}

	return retCode;
}
/*--------------------------------------------------------------------------*/
static BOOL SetLastJob(char *JOB)
{
	if (lastjob) { FREE(lastjob); lastjob = NULL;}

	if (JOB)
	{
		lastjob = strdup(JOB);
		if (lastjob)
		{
			return TRUE;
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL GetLastJob(char *JOB,int nbcharsJOB)
{
	if (JOB)
	{
		if ((int)strlen(lastjob) < nbcharsJOB)
		{
			strcpy(JOB, lastjob);
		}
		else strncpy(JOB, lastjob, nbcharsJOB);
		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
int SendScilabJobs(char **jobs,int numberjobs)
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

		LOCALJOBS = (char**)MALLOC(sizeof(char*) * numberjobs);

		if (LOCALJOBS)
		{
			for (i = 0; i < numberjobs; i++)
			{
				if (jobs[i])
				{
					nbcharsjobs = nbcharsjobs + (int)strlen(jobs[i]);
					LOCALJOBS[i] = (char*)MALLOC( sizeof(char)*(strlen(jobs[i]) + BUFFERSECURITYSIZE) );
					if (LOCALJOBS[i])
					{
						strcpy(LOCALJOBS[i], jobs[i]);
					}
					else
					{
						CleanBuffers(bufCommands, LOCALJOBS, numberjobs);
						fprintf(stderr,"Error : SendScilabJobs (1) 'LOCALJOBS[%d] MALLOC'.\n",i);	
						return retcode;
					}
				}
				else
				{
					fprintf(stderr,"Error : SendScilabJobs (2) 'jobs[%d] == NULL'.\n",i);	
					return retcode;
				}
			}

			bufCommands = (char*)MALLOC( sizeof(char)*(nbcharsjobs+numberjobs+BUFFERSECURITYSIZE) );

			if (bufCommands)
			{
				strcpy(bufCommands,"");

				for (jobsloop = 0 ; jobsloop < numberjobs ; jobsloop++)
				{
					if (jobs[jobsloop])
					{
						char *currentline = NULL;
						BOOL AddSemiColon;

						if (jobsloop == 0) AddSemiColon=FALSE;
						else  AddSemiColon=TRUE;

DOTDOTLOOP:
						currentline = LOCALJOBS[jobsloop];

						RemoveCharsFromEOL(currentline,'\n');
						RemoveComments(currentline);
						RemoveCharsFromEOL(currentline,' ');

						if (RemoveCharsFromEOL(currentline,'.'))
						{
							RemoveCharsFromEOL(currentline,' ');
							strcat(bufCommands,currentline);
							jobsloop++;
							AddSemiColon = FALSE;
							goto DOTDOTLOOP;
						}
						else
						{
							if (!AddSemiColon)
							{
								AddSemiColon = TRUE;
								strcat(currentline,";");
							}
							else
							{
								strcat(bufCommands,";");
							}

							strcat(bufCommands,currentline);
						}
					}
				}

				retcode = SendScilabJob(bufCommands);
				CleanBuffers(bufCommands,LOCALJOBS,numberjobs);
			}
			else
			{
				CleanBuffers(bufCommands,LOCALJOBS,numberjobs);
				fprintf(stderr,"Error : SendScilabJobs (3) 'bufCommands MALLOC'.\n");	
				return retcode;
			}
		}
		else
		{
			CleanBuffers(bufCommands,LOCALJOBS,numberjobs);
			fprintf(stderr,"Error : SendScilabJobs (4) 'LOCALJOBS == NULL'.\n");	
			return retcode;
		}
	}
	else
	{
		fprintf(stderr,"Error : SendScilabJobs (5) 'jobs == NULL'.\n");	
		retcode = -10;
	}

	return retcode;
}
/*--------------------------------------------------------------------------*/
static BOOL RemoveCharsFromEOL(char *line,char CharToRemove)
{
	int l = 0;
	BOOL bOK = FALSE;
	int len = 0;

	len = (int)strlen(line);
	for (l = (len-1); l>0 ;l--)
	{
		if (line[l] == CharToRemove)
		{
			line[l] = '\0';
			bOK = TRUE;
		}
		else break;
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/
static BOOL RemoveComments(char *line)
{
	int l = 0;
	int len = 0;
	int idx = -1;

	len=(int)strlen(line);
	for (l=len-1 ; l > 0 ; l--)
	{
		if (line[l] == '/') 
		{
			if (l-1 >= 0)
			{
				if (line[l-1] == '/') 
				{
					idx = l-1;
					l = l-2;
				}
			}
		}
	}

	if (idx>=0) line[idx] = '\0';

	return FALSE;
}
/*--------------------------------------------------------------------------*/
static BOOL CleanBuffers(char *bufCommands,char **LOCALJOBS,int numberjobs)
{
	if (bufCommands) {FREE(bufCommands);bufCommands = NULL;}
	freeArrayOfString(LOCALJOBS,numberjobs);
	return TRUE;
}
/*--------------------------------------------------------------------------*/
