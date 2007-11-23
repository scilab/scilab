/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __INTFFTWLIBRARYISLOADED__
#define __INTFFTWLIBRARYISLOADED__

#ifdef _MSC_VER
#include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"

int sci_fftwlibraryisloaded __PARAMS((char *fname,unsigned long fname_len));

#endif /* __INTFFTWLIBRARYISLOADED__ */
/*--------------------------------------------------------------------------*/
