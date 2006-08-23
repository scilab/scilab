/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __FIGURETOFILE__
#define __FIGURETOFILE__
/*-----------------------------------------------------------------------------------*/ 
#include <windows.h>

#include "machine.h"
#include "stack-c.h"
#include "version.h"
#include "scigraphic.h"
#include "Graphics.h"

#include "wgraph.h"
#include "wgnuplib.h"
#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"

/*-----------------------------------------------------------------------------------*/ 
#ifndef MAXSTR
#define MAXSTR 255
#endif
/*-----------------------------------------------------------------------------------*/ 
void dos2win32 (char *filename, char *filename1);
char * GetFileExtension(char* filename);
#endif /* __TEXTTOFILE__ */
/*-----------------------------------------------------------------------------------*/ 
