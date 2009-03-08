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

#ifndef __CALLSCILAB__
#define __CALLSCILAB__

/*--------------------------------------------------------------------------*/
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
/**
 * Disable TCL/TK and graphic interfaces 
 * Scilab no GUI no TCL/TK "kernel mode" 
*/
void DisableInteractiveMode(void);

/**
 * Initialize Scilab
 * @param SCIpath define SCI environment variable : Default --> NULL
 * @param ScilabStartup path & filename of scilab.start : Default --> NULL 
 * @param Stacksize : Default --> NULL 
 * @return TRUE if it is OK else FALSE
*/
BOOL StartScilab(char *SCIpath,char *ScilabStartup,int *Stacksize);

/**
 * Terminate Scilab
 * @param ScilabQuit path & filename of scilab.quit : Default --> NULL 
 * @return TRUE if it is OK else FALSE 
*/
BOOL TerminateScilab(char *ScilabQuit);

/**
 * Send a job to scilab 
 * @param job the Scilab Job
 * @return error code operation 0 : OK
*/
int SendScilabJob(char *job); 

/**
 * Send jobs to scilab 
 * @code 
 * 	jobs[0] : a = 1;
 * 	jobs[1] : b = 3;
 *	jobs[2] : c = a + b;
 *	SendScilabJobs(jobs,3);
 * @endcode
 * @return last error code operation 0 : OK
**/
int SendScilabJobs(char **jobs,int numberjobs);

/**
 * Returns last job send to scilab by SendScilabJobs or SendScilabJob
 * @param[out] JOB the last job
 * @param[out] nbcharsJOB the size of JOB
 * @code
 * Example :
 * jobs[0] : a = 1;
 * jobs[1] : b = V_NOT_EXIST;
 * jobs[2] : c = a + b;
 * if (SendScilabJobs(jobs,3))
 * {
 * 	char lastjob[4096]; // bsiz in scilab 
 *	if (GetLastJob(lastjob,4096))
 *	{
 *		printf("%s\n",lastjob);
 *	}
 * }
 * @endcode
 * @return 
**/
BOOL GetLastJob(char *JOB,int nbcharsJOB);

/**
* This procedure is the entry point to Scilab's event loop 
*/
void ScilabDoOneEvent(void);

/**
 * Get the information is a graphic windows is opened or not
 * @Return Returns TRUE if a graphic windows is opened 
*/
int ScilabHaveAGraph(void);

/*--------------------------------------------------------------------------*/
#endif /* __CALLSCILAB__ */
/*--------------------------------------------------------------------------*/
