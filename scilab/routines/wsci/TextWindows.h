/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __TEXTWINDOWS__
#define __TEXTWINDOWS__
/*-----------------------------------------------------------------------------------*/ 

#include <windows.h>

#include "wgnuplib.h"

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"


/*-----------------------------------------------------------------------------------*/ 
#ifndef NULL
  #define NULL 0
#endif

#define TRUE  1
#define FALSE 0

TW InitTWStruct(void);
LPTW GetTextWinScilab(void);

#endif /* __TEXTWINDOWS__ */
/*-----------------------------------------------------------------------------------*/ 
