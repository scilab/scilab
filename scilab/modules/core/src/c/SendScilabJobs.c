/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "CallScilab.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(scirun)(char * startup, int lstartup);
/*-----------------------------------------------------------------------------------*/
static BOOL RemoveCharsFromEOL(char *line,char CharToRemove);
static char lastjob[bsiz]="";
/*-----------------------------------------------------------------------------------*/
int SendScilabJob(char *job)
{
	int m,n,lp;
	double code = -1;

	static char buf[bsiz];
	char format[]="try %s,catch,end;quit;";
	char CommandGetLastError[]="[StrErr,Err]=lasterror(%f);quit;";
	char ClearErrVariables[]="clear StrErr;clear Err;quit;";

	if ( (strlen(format)+strlen(job)) >= bsiz)
	{
		fprintf(stderr,"Error : SendScilabJob string 'job' too long \n");
		return (int) code;
	}

	sprintf(buf,format,job);
	strcpy(lastjob,job);
	C2F(scirun)(buf,strlen(buf));

	/* clear prev. Err & StrErr scilab variables */
	C2F(scirun)(ClearErrVariables,strlen(ClearErrVariables));

	/* check lasterror */ 
	C2F(scirun)(CommandGetLastError,strlen(CommandGetLastError));

	if ( ! C2F(cmatptr)("Err", &m, &n, &lp,strlen("Err")))
	{
		fprintf(stderr,"Error : SendScilabJob (cmatptr) 'Err' \n");
	}
	else
	{
		if (m*n == 1)
		{
			ReadMatrix("Err", &m, &n, &code);
		}
		else fprintf(stderr,"Error : SendScilabJob (ReadMatrix Err)\n");
	}

	/* clear prev. Err & StrErr scilab variables */
	C2F(scirun)(ClearErrVariables,strlen(ClearErrVariables));

	return (int) code;
}
/*-----------------------------------------------------------------------------------*/
BOOL GetLastJob(char *JOB)
{
	BOOL bOK=FALSE;

	if (strcmp(lastjob,""))
	{
		if (JOB)
		{
			strcpy(JOB,lastjob);
			bOK=TRUE;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int SendScilabJobs(char **jobs,int numberjobs)
{
	int retcode=-1;
	int jobsloop=0;

	char buf[bsiz];

	for (jobsloop=0;jobsloop<numberjobs;jobsloop++)
	{
		strcpy(buf,"");
		if (jobs[jobsloop])
		{
			char *currentline=jobs[jobsloop];
			RemoveCharsFromEOL(currentline,'\n');
			RemoveCharsFromEOL(currentline,' ');
			while (RemoveCharsFromEOL(currentline,'.'))
			{
				RemoveCharsFromEOL(currentline,' ');
				strcat(buf,currentline);

				jobsloop++;
				currentline=jobs[jobsloop];
				RemoveCharsFromEOL(currentline,'\n');
				RemoveCharsFromEOL(currentline,' ');
			}
			strcat(buf,currentline);
			retcode=SendScilabJob(buf);
			if (retcode) return retcode;
		}
	}
	return retcode;
}
/*-----------------------------------------------------------------------------------*/
static BOOL RemoveCharsFromEOL(char *line,char CharToRemove)
{
	int l=0;
	BOOL bOK=FALSE;
	int len=0;

	len=strlen(line);
	for (l=len-1;l>0;l--)
	{
		if (line[l] == CharToRemove)
		{
			line[l] = '\0';
			bOK=TRUE;
		}
		else break;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
