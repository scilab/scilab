/*--------------------------------------------------------------------------*/
/* INRIA */
/*--------------------------------------------------------------------------*/
#ifndef  __GW_INTERPOLATION_H__
#define __GW_INTERPOLATION_H__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int C2F(gw_interpolation)(void);
/*--------------------------------------------------------------------------*/
int intsplin(char *fname,unsigned long fname_len); 
int intlsq_splin(char *fname,unsigned long fname_len);
int intinterp1(char *fname,unsigned long fname_len); 
int intlinear_interpn(char *fname,unsigned long fname_len);
int intsplin2d(char *fname,unsigned long fname_len); 
int intinterp2d(char *fname,unsigned long fname_len); 
int intcshep2d(char *fname,unsigned long fname_len); 
int inteval_cshep2d(char *fname,unsigned long fname_len);
int intsplin3d(char *fname,unsigned long fname_len); 
int intinterp3d(char *fname,unsigned long fname_len); 
int intbsplin3val(char *fname,unsigned long fname_len) ;
/*--------------------------------------------------------------------------*/
#endif /* __GW_INTERPOLATION_H__ */
/*--------------------------------------------------------------------------*/

