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
void set_sci_env(char *p) ;
BOOL ConvertPathWindowsToUnixFormat(char *pathwindows,char *pathunix);
BOOL ConvertPathUnixToWindowsFormat(char *pathunix,char *pathwindows);

/*-----------------------------------------------------------------------------------*/
#endif /* __SCIENV_H__ */