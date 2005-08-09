/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __FIGURETOPRINT__
#define __FIGURETOPRINT__
/*-----------------------------------------------------------------------------------*/ 

#include <windows.h>

#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"
#include "../graphics/scigraphic.h"
#include "../graphics/Graphics.h"

#include "wgraph.h"
#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"


/*-----------------------------------------------------------------------------------*/ 
#ifndef NULL
  #define NULL 0
#endif

#define TRUE  1
#define FALSE 0

BOOL FigureToPrint(int figurenumber,BOOL Postscript);
int CopyPrint (struct BCG *ScilabGC);
void PrintPs (struct BCG *ScilabGC);
#endif /* __TEXTTOPRINT__ */
/*-----------------------------------------------------------------------------------*/ 
