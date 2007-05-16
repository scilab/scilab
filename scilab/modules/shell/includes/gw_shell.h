/*-----------------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/
#ifndef __GW_SHELL_H__
#define __GW_SHELL_H__
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
int C2F(gw_shell)(void);
/*-----------------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
int C2F(sci_completion) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_clc) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_tohome) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_loadhistory) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_savehistory) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_gethistory) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_resethistory) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
#endif /*  __GW_SHELL_H__ */


