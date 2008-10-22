/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include "CallScilab.h"
#include "MALLOC.h"
#include "scirun.h"
#include "localization.h"
#include "stack-c.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
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
	static char *command = NULL;
	
	static char ScirunCommand[] = "Err=execstr(TMP_EXEC_STRING,\"errcatch\",\"n\");quit;";
	static char ClearTmpVariables[] = "clear TMP_EXEC_STRING;clear Err;quit;";
	
	lencommand = strlen(job);
	command = MALLOC(sizeof(char)*(lencommand+1));

	if (command)
	{
		/* clear prev. Err , TMP_EXEC_STRING scilab variables */
		C2F(scirun)(ClearTmpVariables,strlen(ClearTmpVariables));

		strcpy(command,job);
		SetLastJob(command);

		/* Creation of a temp variable in Scilab */
		if (!C2F(cwritechain)("TMP_EXEC_STRING",&lencommand,(char*)command,(int)strlen("TMP_EXEC_STRING"),(int)strlen(command)) )
		{
			/* Problem */
			fprintf(stderr,"Error : SendScilabJob (1) 'TMP_EXEC_STRING'.\n");
			retCode = -1;

			if (command) {FREE(command);command=NULL;}
			return retCode;
		}
		else
		{
			int m = 0, n = 0, lp = 0;

			C2F(scirun)(ScirunCommand,strlen(ScirunCommand));

			/* get error code from scilab */

			if ( ! C2F(cmatptr)("Err", &m, &n, &lp,strlen("Err")))
			{
				fprintf(stderr,"Error : SendScilabJob (2) 'Err'.\n");
				retCode = -2;
			}
			else
			{
				if (m*n == 1)
				{
					double code = -1;
					ReadMatrix("Err", &m, &n, &code);
					retCode = (int) code ;
				}
				else 
				{
					fprintf(stderr,"Error : SendScilabJob (3) 'Err'.\n");	
					retCode = -3;
				}
			}

			/* clear prev. Err , TMP_EXEC_STRING scilab variables */
			C2F(scirun)(ClearTmpVariables,strlen(ClearTmpVariables));
		}
		if (command) {FREE(command);command=NULL;}
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
	
	if (lastjob) { FREE(lastjob);lastjob=NULL;}

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
		if ((int)strlen(lastjob)<nbcharsJOB)
		{
			strcpy(JOB,lastjob);
		}
		else strncpy(JOB,lastjob,nbcharsJOB);
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

		LOCALJOBS = (char**)MALLOC(sizeof(char*)*numberjobs);

		if (LOCALJOBS)
		{
			for (i = 0; i < numberjobs; i++)
			{
				if (jobs[i])
				{
					nbcharsjobs = nbcharsjobs+(int)strlen(jobs[i]);
					LOCALJOBS[i] = (char*)MALLOC( sizeof(char)*(strlen(jobs[i])+BUFFERSECURITYSIZE) );
					if (LOCALJOBS[i])
					{
						strcpy(LOCALJOBS[i],jobs[i]);
					}
					else
					{
						CleanBuffers(bufCommands,LOCALJOBS,numberjobs);
						fprintf(stderr,"Error : SendScilabJobs (1) 'LOCALJOBS[i] MALLOC'.\n",i);	
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
