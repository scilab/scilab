/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __TCLGLOBAL__
#define __TCLGLOBAL__

/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
  #include <windows.h>
#endif
#include <stdio.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"

#include "tcl.h"
#include "tk.h"
/*-----------------------------------------------------------------------------------*/
#ifndef NULL
  #define NULL 0
#endif

#define TRUE  1
#define FALSE 0
/*-----------------------------------------------------------------------------------*/
/*Tcl_Interp *TKinterp=NULL;*/
/*Tk_Window TKmainWindow=NULL;*/
extern Tcl_Interp *TKinterp;
extern Tk_Window TKmainWindow;
/*-----------------------------------------------------------------------------------*/
int TCL_EvalScilabCmd(ClientData clientData,Tcl_Interp * theinterp,int objc,CONST char ** argv);
/*-----------------------------------------------------------------------------------*/
#endif /* __TCLGLOBAL__ */
/*-----------------------------------------------------------------------------------*/ 