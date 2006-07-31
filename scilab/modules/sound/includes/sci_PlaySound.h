/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTPLAYSOUND__
#define __INTPLAYSOUND__

#ifdef _MSC_VER
#include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"


#ifndef NULL
#define NULL 0
#endif

#define TRUE  1
#define FALSE 0

int sci_Playsound __PARAMS((char *fname,unsigned long fname_len));

#endif /* __INTPLAYSOUND__ */
/*-----------------------------------------------------------------------------------*/
