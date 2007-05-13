/*-----------------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/
#ifndef __GW_CONSOLE_H__
#define __GW_CONSOLE_H__
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
int C2F(gw_console)(void);
/*-----------------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
int C2F(sci_completion) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_clc) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_tohome) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
#endif /*  __GW_CONSOLE_H__ */


