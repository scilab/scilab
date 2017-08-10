/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __GW_SPARSE__
#define __GW_SPARSE__

#include "machine.h"
#include "dynlib_sparse.h"
/*--------------------------------------------------------------------------*/
SPARSE_IMPEXP int gw_sparse(void);
/*--------------------------------------------------------------------------*/
//Scilab 6
SPARSE_IMPEXP int sci_spmin (char *fname, void* pvApiCtx);
SPARSE_IMPEXP int sci_spmax (char *fname, void* pvApiCtx);

int sci_sparsefunc (char *fname, unsigned long fname_len);
int sci_spget (char *fname, unsigned long fname_len);
int sci_full (char *fname, unsigned long fname_len);
//int sci_lufact (char *fname, unsigned long fname_len); -> c++
//int sci_lusolve (char *fname, unsigned long fname_len);
//int sci_ludel (char *fname, unsigned long fname_len);
//int sci_luget (char *fname, unsigned long fname_len);
SPARSE_IMPEXP int sci_spclean (char *fname, unsigned long fname_len);
int sci_nnz (char *fname, unsigned long fname_len);
int sci_spmatrix (char *fname, unsigned long fname_len);
int sci_spchol (char *fname, unsigned long fname_len);
int sci_fadj2sp (char *fname, unsigned long fname_len);
int sci_spcompa (char *fname, unsigned long fname_len);
//int sci_ordmmd (char *fname, unsigned long fname_len);
//int sci_blkfc1i (char *fname, unsigned long fname_len);
int sci_blkslvi (char *fname, unsigned long fname_len);
//int sci_inpnvi (char *fname, unsigned long fname_len);
//int sci_sfinit (char *fname, unsigned long fname_len);
//int sci_symfcti (char *fname, unsigned long fname_len);
//int sci_bfinit (char *fname, unsigned long fname_len);
int sci_msparse (char *fname, unsigned long fname_len);
int sci_mspget (char *fname, unsigned long fname_len);
int sci_mfull (char *fname, unsigned long fname_len);
extern int C2F(scita2lpd) (char *fname, unsigned long fname_len);

/*--------------------------------------------------------------------------*/
#endif /*  __GW_SPARSE__ */
/*--------------------------------------------------------------------------*/

