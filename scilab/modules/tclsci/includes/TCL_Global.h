/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __TCLGLOBAL__
#define __TCLGLOBAL__

/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
  #include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include <tcl.h>
#include <tk.h>

#include "machine.h"
#include "stack-c.h"
#include "version.h"

#include "MALLOC.h"

/*--------------------------------------------------------------------------*/
extern Tk_Window TKmainWindow;
extern int XTKsocket;
/*--------------------------------------------------------------------------*/

/**
 * @TODO : add comment
 *
 * @param TCLinterp
 * @param StringUTF8
 * @return <ReturnValue>
 */
char *UTF8toANSI(Tcl_Interp *TCLinterp,char *StringUTF8);

/*--------------------------------------------------------------------------*/
#endif /* __TCLGLOBAL__ */
/*--------------------------------------------------------------------------*/
