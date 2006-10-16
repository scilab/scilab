/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTPRINTBOX__
#define __INTPRINTBOX__

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"

int C2F(sci_printsetupbox) _PARAMS((char *fname,unsigned long l));
#if _MSC_VER
	HDC GetPrinterDC(void);
	char GetPrinterOrientation(void);
	char* GetPrinterName(void);
#endif
#endif /* __INTPRINTBOX__ */
/*-----------------------------------------------------------------------------------*/ 
