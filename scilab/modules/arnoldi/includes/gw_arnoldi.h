/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_ARNOLDI_H__
#define __GW_ARNOLDI_H__
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
int C2F(gw_arnoldi)(void);
/*-----------------------------------------------------------------------------------*/
int C2F(intdsaupd) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intdnaupd) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intznaupd) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intdseupd) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intdneupd) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intzneupd) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
#endif /* __GW_ARNOLDI_H__ */
/*-----------------------------------------------------------------------------------*/

