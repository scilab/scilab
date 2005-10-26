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

void ScilabDoOneEvent(void);
/* This procedure is the entry point to Scilab's event loop */

int ScilabHaveAGraph(void);
/* Returns TRUE if a graphic windows is opened */

/*-----------------------------------------------------------------------------------*/
#endif /* __CALLSCILAB__ */
/*-----------------------------------------------------------------------------------*/
