/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_SPARSE__
#define __GW_SPARSE__
#include "machine.h"
/*--------------------------------------------------------------------------*/
int gw_sparse(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_sparsefunc) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_spget) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_full) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_lufact) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_lusolve) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ludel) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_luget) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_spclean) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_nnz) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_spmax) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_spmin) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_spmatrix) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_spchol) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_readmps) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fadj2sp) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_spcompa) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ordmmd) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_blkfc1i) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_blkslvi) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_inpnvi) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sfinit) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_symfcti) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_bfinit) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_msparse) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_mspget) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_mfull) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_SPARSE__ */
/*--------------------------------------------------------------------------*/

