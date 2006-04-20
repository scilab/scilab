/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTREALTIME__
#define __INTREALTIME__

#include <stdio.h>
#include <string.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"

int C2F(intrealtimeinit) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intrealtime) _PARAMS((char *fname,unsigned long fname_len));

#endif /*__INTREALTIME__*/
/*-----------------------------------------------------------------------------------*/ 
