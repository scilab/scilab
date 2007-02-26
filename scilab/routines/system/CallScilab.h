/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __CALLSCILAB__
#define __CALLSCILAB__
/*-----------------------------------------------------------------------------------*/
#if WIN32
  #include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"
/*-----------------------------------------------------------------------------------*/
#define TRUE 1
#define FALSE 0
/*-----------------------------------------------------------------------------------*/
void DisableInteractiveMode(void);
/* Disable TCL/TK and graphic interfaces */
/* Scilab no GUI no TCL/TK "kernel mode" */

int StartScilab(char *SCIpath,char *ScilabStartup,int *Stacksize);
/* Initialize Scilab */
/* returns TRUE if it is OK else FALSE */
/* SCIpath define SCI environment variable : Default --> NULL */
/* ScilabStartup path & filename of scilab.star : Default --> NULL */
/* Stacksize : Default --> NULL */

int TerminateScilab(char *ScilabQuit);
/* Terminate Scilab */
/* returns TRUE if it is OK else FALSE */
/* ScilabQuit path & filename of scilab.quit : Default --> NULL */

int SendScilabJob(char *job); 
/* Send a job to scilab */
/* returns error code operation */

int SendScilabJobs(char **jobs,int numberjobs);
/**
Send jobs to scilab 
returns last error code operation 0 : OK
Example :
	jobs[0] : a = 1;
	jobs[1] : b = 3;
	jobs[2] : c = a + b;
	SendScilabJobs(jobs,3);
**/

int GetLastJob(char *JOB,int nbcharsJOB);
/**
Returns last job send to scilab by SendScilabJobs or SendScilabJob
Example :
jobs[0] : a = 1;
jobs[1] : b = V_NOT_EXIST;
jobs[2] : c = a + b;
if (SendScilabJobs(jobs,3))
{
	char lastjob[4096]; // bsiz in scilab 
	if (GetLastJob(lastjob,4096))
	{
		printf("%s\n",lastjob);
	}
}
**/

void ScilabDoOneEvent(void);
/* This procedure is the entry point to Scilab's event loop */

int ScilabHaveAGraph(void);
/* Returns TRUE if a graphic windows is opened */

/*-----------------------------------------------------------------------------------*/
#endif /* __CALLSCILAB__ */
/*-----------------------------------------------------------------------------------*/
