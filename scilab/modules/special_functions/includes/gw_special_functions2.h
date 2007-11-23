/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_SPECIAL_FUNCTIONS2__
#define __GW_SPECIAL_FUNCTIONS2__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_special_functions2)(void);
/*--------------------------------------------------------------------------*/ 
int sci_besselh(char *fname,unsigned long fname_len);
int sci_besseli(char *fname,unsigned long fname_len);
int sci_besselj(char *fname,unsigned long fname_len);
int sci_besselk(char *fname,unsigned long fname_len);
int sci_bessely(char *fname,unsigned long fname_len);
int sci_beta(char *fname,unsigned long fname_len);
int sci_legendre(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_SPECIAL_FUNCTIONS2__ */
/*--------------------------------------------------------------------------*/

