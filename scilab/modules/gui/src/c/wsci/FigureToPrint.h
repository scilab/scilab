/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __FIGURETOPRINT__
#define __FIGURETOPRINT__
/*-----------------------------------------------------------------------------------*/ 

#include <windows.h>

#include "machine.h"
#include "stack-c.h"
#include "version.h"
#include "scigraphic.h"

#include "wgraph.h"
#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"


/*-----------------------------------------------------------------------------------*/ 
BOOL FigureToPrint(int figurenumber,BOOL Postscript);
int CopyPrint (struct BCG *ScilabGC);
void PrintPs (struct BCG *ScilabGC);
#endif /* __TEXTTOPRINT__ */
/*-----------------------------------------------------------------------------------*/ 
