/* scilab-tk link main header */
/* Bertrand Guiheneuf INRIA 1997 */

#ifndef INCLUDE_TKSCI
#define INCLUDE_TKSCI

#if defined(_WIN32) || defined(WIN32) || defined(WIN)
#define IS_WIN32
#ifdef _MSC_VER
#define IS_MSVC
#endif
#endif 

#ifndef IS_MSVC
#ifdef SYSV
#include <string.h>
#else 
#include <strings.h>
#endif /** SYSV **/
#else
#include <string.h>
#endif /** IS_MSVC **/

#include "tcl.h"
#include "tk.h"

#include "C-LAB_Interf.h"


extern Tcl_Interp *TKinterp;
extern Tk_Window TKmainWindow;
extern int XTKsocket;

#define MAX_HANDLE 4000
extern Matrix *UserData[MAX_HANDLE];



extern int TK_Wait;
extern int TK_NbEv;
extern int TK_Started;

extern void flushTKEvents();
extern void nocase ();
#endif





