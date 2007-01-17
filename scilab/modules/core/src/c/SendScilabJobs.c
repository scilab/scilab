/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "CallScilab.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(scirun)(char * startup, int lstartup);
/*-----------------------------------------------------------------------------------*/
static BOOL RemoveCharsFromEOL(char *line,char CharToRemove);
static char *RepeatChar(char *S1, char c);
static char lastjob[bsiz]="";
/*-----------------------------------------------------------------------------------*/
/* see CallScilab.h more informations*/
/*-----------------------------------------------------------------------------------*/
int SendScilabJob(char *job)
{
	int m,n,lp;
	double code = -1;

	static char buf[bsiz];
	static char *command=NULL;
	static char *tmprepeat=NULL;
	char formatcommand[]="Err=execstr('%s','errcatch','n');quit;";
	char ClearErrVariable[]="clear Err;quit;";

	/* pretreatment */
	/* we repeat \' & \" */
	tmprepeat=RepeatChar(job,'\"');
	command=RepeatChar(tmprepeat,'\'');

	if ( (strlen(command)+strlen(formatcommand)) >= bsiz)
	{
		fprintf(stderr,"Error : SendScilabJob string 'job' too long \n");
		return (int) code;
	}

	/* clear prev. Err scilab variable */
	C2F(scirun)(ClearErrVariable,strlen(ClearErrVariable));

	sprintf(buf,formatcommand,command);
	strcpy(lastjob,job);
	C2F(scirun)(buf,strlen(buf));

	if ( ! C2F(cmatptr)("Err", &m, &n, &lp,strlen("Err")))
	{
		fprintf(stderr,"Error : SendScilabJob (1) 'Err'.\n");
	}
	else
	{
		if (m*n == 1)
		{
			ReadMatrix("Err", &m, &n, &code);
		}
		else fprintf(stderr,"Error : SendScilabJob (2) 'Err'.\n");
	}

	/* clear prev. Err & StrErr scilab variables */
	C2F(scirun)(ClearErrVariable,strlen(ClearErrVariable));

	if (command) {FREE(command);command=NULL;}
	if (tmprepeat) {FREE(tmprepeat);tmprepeat=NULL;}

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
static char *RepeatChar(char *S1, char c) 
{
	char *buffer=NULL;
	int i=0;
	int j=0;
	int nbchartorepeat=0;

	for (i=0;i<(int)strlen(S1);i++) if (S1[i] == c) nbchartorepeat++;

	buffer = (char*)MALLOC((strlen(S1)+nbchartorepeat+1)*sizeof(char));

	if (buffer)
	{
		for (i=0;i<(int)strlen(S1);i++)
		{
			if (S1[i] == c)
			{
				buffer[j]=c;
				buffer[j+1]=c;
				j=j+2;
			}
			else
			{
				buffer[j]=S1[i];
				j++;
			}
		}
		buffer[j]='\0';
	}
	return buffer;
}
/*-----------------------------------------------------------------------------------*/
