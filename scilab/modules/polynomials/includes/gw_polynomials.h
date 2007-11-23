/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_POLYNOMIALS__
#define __GW_POLYNOMIALS__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int C2F(gw_polynomials)(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_poly) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_roots) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_degree) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_coeff) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_eval) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_pppdiv) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_simp) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_psum) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_pprod) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_pdiag) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ptriu) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ptril) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_bezout) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sfact) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_simp_mode) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_varn) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_cleanp) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_POLYNOMIALS__ */
/*--------------------------------------------------------------------------*/

