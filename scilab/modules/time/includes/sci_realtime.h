/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCI_REALTIME__
#define __SCI_REALTIME__

#include <stdio.h>
#include <string.h>
#include "../../core/includes/machine.h"
#include "../../core/includes/stack-c.h"

#include "../../core/includes/version.h"

int C2F(sci_realtimeinit) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_realtime) _PARAMS((char *fname,unsigned long fname_len));

#endif /*__INTREALTIME__*/
/*-----------------------------------------------------------------------------------*/ 
