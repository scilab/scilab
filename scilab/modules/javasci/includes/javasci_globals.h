#ifndef _Included_javasci_globals
#define _Included_javasci_globals
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
#include <math.h>
#include <stdio.h> 
#include <string.h>
#ifdef _MSC_VER
  #include <windows.h>
  #pragma comment(lib, "User32.lib")
#endif
#include "jni.h"
#include "machine.h"
#include "stack-c.h"
#include "tmpdir.h"
#include "scirun.h"
#include "sciquit.h"
#include "CallScilab.h"
#include "fromjava.h"

#include "MALLOC.h"

/********************************************************************************************************/
#ifndef SCI 
  #define SCI ".."
#endif 
/********************************************************************************************************/
#define MAX_STR 512
/********************************************************************************************************/
/**
 * Enable the Interface
 *
 */
void EnableInterf(void);

/**
 * Disable the Interface
 *
 */
void DisableInterf(void);

/**
 * Return the state of the Interface
 * @return int the state (1 or 0)
 */
int GetInterfState(void);

/**
 * Send a Scilab Job
 * @param the Job
 * @return the result of the process
 */
int send_scilab_job(char *job) ;

/**
 * Initialisation of Scilab
 *
 */
void Initialize(void) ;
/********************************************************************************************************/
extern void C2F(banier)(int *x) ;
extern void initTCLTK();
extern int C2F(inisci)(int *,int *,int *);
extern void C2F(storeversion)(char *str,int n);
extern void sciGetIdFigure (int *vect, int *id, int *flag);
extern int GetLastErrorCode(void);
extern void *GetDataFromName( char *name );
/********************************************************************************************************/
#ifdef _MSC_VER
extern void start_sci_tcltk();
#endif
/********************************************************************************************************/
#endif
/********************************************************************************************************/
