#ifndef __SCIENV_H__
#define __SCIENV_H__
/* Allan CORNET */
/* INRIA 2005 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include <Winuser.h>
#include "..\version.h"
#include "wcommon.h"
#include "FilesAssociations.h"
#include "GetOS.h"
/*-----------------------------------------------------------------------------------*/
/* Les Definitions */
/*-----------------------------------------------------------------------------------*/

#define LCCEXE		"\\lcc\\bin\\lcc.exe" 
#define LCCBIN		"\\lcc\\bin" 
#define LCCINCLUDE	"\\lcc\\include" 
#define LCCLIB		"\\lcc\\lib" 
/*-----------------------------------------------------------------------------------*/
void SciEnv ();
void set_sci_env(char *DefaultSCIPATH) ;
BOOL ConvertPathWindowsToUnixFormat(char *pathwindows,char *pathunix);
BOOL ConvertPathUnixToWindowsFormat(char *pathunix,char *pathwindows);
BOOL Set_SCI_PATH(char *DefaultPath);
BOOL Set_HOME_PATH(char *DefaultPath);
BOOL Set_TCL_LIBRARY_PATH(char *DefaultPath);
BOOL Set_TK_LIBRARY_PATH(char *DefaultPath);
BOOL Set_LCC_PATH(char *DefaultPath);
BOOL Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB(void);
/*-----------------------------------------------------------------------------------*/
#endif /* __SCIENV_H__ */