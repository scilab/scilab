#ifndef _Included_javasci_globals
#define _Included_javasci_globals
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
#include <math.h>
#include <stdio.h> 
#include <string.h>
#if WIN32
  #include <windows.h>
  #pragma comment(lib, "User32.lib")
#endif
#include "jni.h"
#include "../machine.h"
#include "../stack-c.h"
/********************************************************************************************************/
#ifndef SCI 
  #define SCI ".."
#endif 
/********************************************************************************************************/
void EnableInterf();
void DisableInterf();
int GetInterfState();
int send_scilab_job(char *job) ;
void Initialize() ;
/********************************************************************************************************/
extern void C2F(banier)(int *x) ;
extern integer C2F (ismenu) __PARAMS((void));
extern void initTCLTK();
extern int C2F(inisci)(int *,int *,int *);
extern int C2F (sciquit) (void);
extern void C2F(settmpdir) (void);
extern int C2F(scirun)(char * startup, int lstartup);
extern void C2F(storeversion)(char *str,int n);
extern  int C2F(sxevents)();
extern void sciGetIdFigure (int *vect, int *id, int *flag);
extern int version_flag();
extern void SetFromJavaToON(void);
/********************************************************************************************************/
#if WIN32
extern char *getenv();
extern void start_sci_gtk();
extern void add_sci_argv();
extern void add_sci_argv();
extern void set_sci_env(char *DefaultSCIPATH);
extern void TextMessage1 (int ctrlflag);
extern void InitWindowGraphDll(void);
#endif
/********************************************************************************************************/
#endif
/********************************************************************************************************/
