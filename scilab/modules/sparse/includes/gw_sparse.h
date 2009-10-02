/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GW_SPARSE__
#define __GW_SPARSE__

#include "machine.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int gw_sparse(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_sparsefunc) (char *fname,unsigned long fname_len);
int C2F(sci_spget) (char *fname,unsigned long fname_len);
int C2F(sci_full) (char *fname,unsigned long fname_len);
int C2F(sci_lufact) (char *fname,unsigned long fname_len);
int C2F(sci_lusolve) (char *fname,unsigned long fname_len);
int C2F(sci_ludel) (char *fname,unsigned long fname_len);
int C2F(sci_luget) (char *fname,unsigned long fname_len);
int C2F(sci_spclean) (char *fname,unsigned long fname_len);
int C2F(sci_nnz) (char *fname,unsigned long fname_len);
int C2F(sci_spmax) (char *fname,unsigned long fname_len);
int C2F(sci_spmin) (char *fname,unsigned long fname_len);
int C2F(sci_spmatrix) (char *fname,unsigned long fname_len);
int C2F(sci_spchol) (char *fname,unsigned long fname_len);
int C2F(sci_readmps) (char *fname,unsigned long fname_len);
int C2F(sci_fadj2sp) (char *fname,unsigned long fname_len);
int C2F(sci_spcompa) (char *fname,unsigned long fname_len);
int C2F(sci_ordmmd) (char *fname,unsigned long fname_len);
int C2F(sci_blkfc1i) (char *fname,unsigned long fname_len);
int C2F(sci_blkslvi) (char *fname,unsigned long fname_len);
int C2F(sci_inpnvi) (char *fname,unsigned long fname_len);
int C2F(sci_sfinit) (char *fname,unsigned long fname_len);
int C2F(sci_symfcti) (char *fname,unsigned long fname_len);
int C2F(sci_bfinit) (char *fname,unsigned long fname_len);
int C2F(sci_msparse) (char *fname,unsigned long fname_len);
int C2F(sci_mspget) (char *fname,unsigned long fname_len);
int C2F(sci_mfull) (char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_SPARSE__ */
/*--------------------------------------------------------------------------*/

