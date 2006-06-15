#ifndef _Included_javasci_globals
#define _Included_javasci_globals
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
#include <math.h>
#include <stdio.h> 
#include <string.h>
#if _MSC_VER
  #include <windows.h>
  #pragma comment(lib, "User32.lib")
#endif
#include "jni.h"
#include "../machine.h"
#include "../stack-c.h"

#ifdef _MSC_VER
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
/********************************************************************************************************/
#ifndef SCI 
  #define SCI ".."
#endif 

#ifndef NULL
	#define NULL 0
#endif
/********************************************************************************************************/
#define MAX_STR 512
/********************************************************************************************************/
void EnableInterf();
void DisableInterf();
int GetInterfState();
int send_scilab_job(char *job) ;
void Initialize() ;
/********************************************************************************************************/
extern void C2F(banier)(int *x) ;
extern void initTCLTK();
extern int C2F(inisci)(int *,int *,int *);
extern int C2F (sciquit) (void);
extern void C2F(settmpdir) (void);
extern int C2F(scirun)(char * startup, int lstartup);
extern void C2F(storeversion)(char *str,int n);
extern void sciGetIdFigure (int *vect, int *id, int *flag);
extern int version_flag();
extern void SetFromJavaToON(void);
extern int SendScilabJob(char *job); 
extern void ScilabDoOneEvent(void);
extern int TerminateScilab(char *ScilabQuit);
extern int GetLastErrorCode(void);
/********************************************************************************************************/
#if _MSC_VER
extern void start_sci_tcltk();
extern void add_sci_argv();
extern void add_sci_argv();
extern void set_sci_env(char *DefaultSCIPATH);
extern void InitWindowGraphDll(void);
#endif
/********************************************************************************************************/
#endif
/********************************************************************************************************/
