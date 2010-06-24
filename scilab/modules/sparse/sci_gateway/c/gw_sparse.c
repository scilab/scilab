/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "gw_sparse.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
    {sci_sparsefunc, "sparse"},
    {sci_spget, "spget"},
    {sci_full, "full"},
    {sci_lufact, "lufact"},
    {sci_lusolve, "lusolve"},
    {sci_ludel, "ludel"},
    {sci_luget, "luget"},
    {sci_spclean, "spclean"},
    {sci_nnz, "nnz"},
    {sci_spmax, "spmax"},
    {sci_spmin, "spmin"},
    {sci_spmatrix, "spmatrix"},
    {sci_spchol, "spchol"},
    {sci_fadj2sp, "fadj2sp"},
    {sci_spcompa, "spcompa"},
    {sci_ordmmd, "ordmmd"},
    {sci_blkfc1i, "blkfc1i"},
    {sci_blkslvi, "blkslvi"},
    {sci_inpnvi, "inpnvi"},
    {sci_sfinit, "sfinit"},
    {sci_symfcti, "symfcti"},
    {sci_bfinit, "bfinit"},
    {sci_msparse, "msparse"},
    {sci_mspget, "mspget"},
    {sci_mfull, "mfull"},
    {C2F(scita2lpd), "ta2lpd"}
};
/*--------------------------------------------------------------------------*/
int gw_sparse(void)
{  
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}
/*--------------------------------------------------------------------------*/
