/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __INTLOADFFTWLIBRARY__
#define __INTLOADFFTWLIBRARY__

#ifdef _MSC_VER
#include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"

int sci_loadfftwlibrary __PARAMS((char *fname,unsigned long fname_len));

#endif /* __INTLOADFFTWLIBRARY__ */
/*--------------------------------------------------------------------------*/
