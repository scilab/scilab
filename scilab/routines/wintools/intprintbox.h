/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTPRINTBOX__
#define __INTPRINTBOX__

#ifdef WIN32
  #include <windows.h>
#endif

#include <stdio.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"


#ifndef NULL
  #define NULL 0
#endif

#define TRUE  1
#define FALSE 0


int C2F(intprintsetupbox) _PARAMS((char *fname));
#if WIN32
	HDC GetPrinterDC(void);
	char GetPrinterOrientation(void);
	char* GetPrinterName(void);
#endif
#endif /* __INTPRINTBOX__ */
/*-----------------------------------------------------------------------------------*/ 
