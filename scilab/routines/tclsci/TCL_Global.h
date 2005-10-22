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
#include <string.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"

#include "tcl.h"
#include "tk.h"

#include "Errors.h"
#include "Warnings.h"
#include "Messages.h"

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif


/*-----------------------------------------------------------------------------------*/
#ifndef NULL
  #define NULL 0
#endif
#ifndef TRUE 
#define TRUE  1
#endif 
#ifndef FALSE
#define FALSE 0
#endif
/*-----------------------------------------------------------------------------------*/
extern Tcl_Interp *TCLinterp;
extern Tk_Window TKmainWindow;
extern int TK_Started;
extern int XTKsocket;
extern int IsAScalar(int RhsNumber); 
/*-----------------------------------------------------------------------------------*/
void nocase (char *s);
char *Matrix2String(int RhsMatrix);
double *String2Matrix(char *StringIn,int *nbelemOut);
int MustReturnAMatrix(char *FieldPropertie);
int MustReturnAString(char *FieldPropertie);
int ValueMustBeAMatrix(char *FieldPropertie);
int ValueMustBeAString(char *FieldPropertie);
int CheckPropertyField(char *FieldPropertie);
char *UTF8toANSI(Tcl_Interp *TCLinterp,char *StringUTF8);
/*-----------------------------------------------------------------------------------*/
#endif /* __TCLGLOBAL__ */
/*-----------------------------------------------------------------------------------*/ 
