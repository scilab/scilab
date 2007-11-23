/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __SCI_GETDATE__
#define __SCI_GETDATE__

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"

int C2F(sci_getdate) _PARAMS((char *fname,unsigned long fname_len));

#endif /* __SCI_GETDATE__ */
/*--------------------------------------------------------------------------*/ 
