/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_SPECIAL_FUNCTIONS__
#define __GW_SPECIAL_FUNCTIONS__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_special_functions1)(void);
/*--------------------------------------------------------------------------*/ 
int C2F(sci_oldbesseli) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_oldbesselj) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_oldbesselk) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_oldbessely) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_gamma) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_lgamma) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_dlgamma) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_calerf) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_SPECIAL_FUNCTIONS__ */
/*--------------------------------------------------------------------------*/

