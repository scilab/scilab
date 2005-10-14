/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTWHAT__
#define __INTWHAT__

#include <stdio.h>
#include <string.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"
/*-----------------------------------------------------------------------------------*/ 
#ifndef NULL
#define NULL 0
#endif
#ifndef TRUE 
#define TRUE  1
#endif 
#ifndef FALSE
#define FALSE 0
#endif
/*-----------------------------------------------------------------------------------*/ 
int C2F(intwhat) _PARAMS((char *fname));

#endif /*__INTWHAT__*/
/*-----------------------------------------------------------------------------------*/ 
