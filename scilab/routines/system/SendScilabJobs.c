/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "CallScilab.h"
#ifdef WIN32
  #include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
  #include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/
extern int C2F(scirun)(char * startup, int lstartup);
/*-----------------------------------------------------------------------------------*/
static int RemoveCharsFromEOL(char *line,char CharToRemove);
static int RemoveComments(char *line);
static int CleanBuffers(char *bufCommands,char **LOCALJOBS,int numberjobs);
static int SetLastJob(char *JOB);
static char *lastjob=NULL;
static int SendScilabJob2(char *job);
/*-----------------------------------------------------------------------------------*/
/* see CallScilab.h more informations*/
/*-----------------------------------------------------------------------------------*/
static int SendScilabJob2(char *job)
{
	int retCode = -1;
	int lencommand=0;
	static char *command=NULL;
	
	char ScirunCommand[]="Err=execstr(TMP_EXEC_STRING,\"errcatch\",\"n\");quit;";
	char ClearTmpVariables[]="clear TMP_EXEC_STRING;clear Err;quit;";
	
	lencommand=strlen(job);
	command=MALLOC(sizeof(char)*(lencommand+1));

	if (command)
	{
		/* clear prev. Err , TMP_EXEC_STRING scilab variables */
		C2F(scirun)(ClearTmpVariables,strlen(ClearTmpVariables));

		strcpy(command,job);
		SetLastJob(command);

		/* Creation Variable temporaire dans Scilab */
		if (!C2F(cwritechain)("TMP_EXEC_STRING",&lencommand,(char*)command,(int)strlen("TMP_EXEC_STRING"),(int)strlen(command)) )
		{
			/* Probleme */
			fprintf(stderr,"Error : SendScilabJob (1) 'TMP_EXEC_STRING'.\n");
			retCode = -1;

			if (command) {FREE(command);command=NULL;}
			return retCode;
		}
		else
		{
			int m=0,n=0,lp=0;

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
/*-----------------------------------------------------------------------------------*/
static int SetLastJob(char *JOB)
{
	int bOK=0;
	
	if (lastjob) { FREE(lastjob);lastjob=NULL;}

	if (JOB)
	{
		lastjob = MALLOC(sizeof(char)*(strlen(JOB)+1));	
		if (lastjob)
		{
			strcpy(lastjob,JOB);
			bOK = 1;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int GetLastJob(char *JOB,int nbcharsJOB)
{
	int bOK=0;
	if (JOB)
	{
		if ((int)strlen(lastjob)<nbcharsJOB)
		{
			strcpy(JOB,lastjob);
		}
		else strncpy(JOB,lastjob,nbcharsJOB);
		bOK=1;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int SendScilabJobs(char **jobs,int numberjobs)
{
	#define BufferSecuritySize 64

	int retcode=-10;

	if (jobs)
	{
		int i=0;
		int nbcharsjobs=0;
		char *bufCommands=NULL;
		char **LOCALJOBS=NULL;

		int jobsloop=0;

		LOCALJOBS=(char**)MALLOC(sizeof(char*)*numberjobs);

		if (LOCALJOBS)
		{
			for (i=0;i<numberjobs;i++)
			{
				if (jobs[i])
				{
					nbcharsjobs = nbcharsjobs+(int)strlen(jobs[i]);
					LOCALJOBS[i]=(char*)MALLOC( sizeof(char)*(strlen(jobs[i])+BufferSecuritySize) );
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

			bufCommands = (char*)MALLOC( sizeof(char)*(nbcharsjobs+numberjobs+BufferSecuritySize) );

			if (bufCommands)
			{
				strcpy(bufCommands,"");

				for (jobsloop=0;jobsloop<numberjobs;jobsloop++)
				{
					if (jobs[jobsloop])
					{
						char *currentline=NULL;
						int AddSemiColon;

						if (jobsloop == 0) AddSemiColon=0;
						else  AddSemiColon=1;

				DOTDOTLOOP:
						currentline=LOCALJOBS[jobsloop];

						RemoveCharsFromEOL(currentline,'\n');
						RemoveComments(currentline);
						RemoveCharsFromEOL(currentline,' ');

						if (RemoveCharsFromEOL(currentline,'.'))
						{
							RemoveCharsFromEOL(currentline,' ');
							strcat(bufCommands,currentline);
							jobsloop++;
							AddSemiColon=0;
							goto DOTDOTLOOP;
						}
						else
						{
							if (!AddSemiColon)
							{
								AddSemiColon=1;
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

				retcode=SendScilabJob2(bufCommands);

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
/*-----------------------------------------------------------------------------------*/
static int RemoveCharsFromEOL(char *line,char CharToRemove)
{
	int l=0;
	int bOK=0;
	int len=0;

	len=strlen(line);
	for (l=len-1;l>0;l--)
	{
		if (line[l] == CharToRemove)
		{
			line[l] = '\0';
			bOK=1;
		}
		else break;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
static int RemoveComments(char *line)
{
	int l=0;
	int bOK=0;
	int len=0;
	int idx=-1;

	len=strlen(line);
	for (l=len-1;l>0;l--)
	{
		if (line[l] == '/') 
		{
			if (l-1>0)
			{
				if (line[l-1] == '/') 
				{
					idx=l-1;
					l=l-2;
				}
			}
		}
	}

	if (idx>=0) line[idx]='\0';

	bOK = 1;
	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
static int CleanBuffers(char *bufCommands,char **LOCALJOBS,int numberjobs)
{
	int bOK=0;

	if (bufCommands) {FREE(bufCommands);bufCommands=NULL;}

	if (LOCALJOBS)
	{
		int i=0;
		for (i=0; i < numberjobs;i++)
		{
			if (LOCALJOBS[i]) {FREE(LOCALJOBS[i]);LOCALJOBS[i]=NULL;}
		}
		LOCALJOBS=NULL;
	}

	bOK=1;
	
	return bOK;

}
/*-----------------------------------------------------------------------------------*/
