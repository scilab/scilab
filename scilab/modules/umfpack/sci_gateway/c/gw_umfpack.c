/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007 - Bruno PINCON
 *  Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

/*
 *  SCILAB INTERFACES FEATURES (Scilab )
 *     
 *               [x] = umfpack(A,"\",b)     : solve directly A x =b
 *               [x] = umfpack(b,"/",A)     : solve directly x A = b (<=> A'x'=b')
 *          [LU_ptr] = umf_lufact(A)        : factorize A
 *               [x] = umf_lusolve(LU_ptr,b [,flag, A])
 *                                          : solve Ax=b or A'x=b with or without iterative 
 *                                            raffinement given the factorization
 *                     umf_ludel(LU_ptr)    : free the memory used by the factorization
 * [OK,nrow,ncol,lnz,unz,udiag_nz,it] = ...
 *                     umf_luinfo(LU_ptr)   : info on the LU factors
 *       [L,U,p,q,R] = umf_luget(LU_ptr)    : getting L,U,p,q  (PA'Q = LU)
 *
 *         [C_ptr] = taucs_chfact(A)        : factorize A (C_ptr is a pointer to the cholesky factors)
 *             [x] = taucs_chsolve(C_ptr,b) : solve Ax=b
 *                   taucs_chdel(C_ptr)     : free the memory used by the factorization
 *    [OK, n, cnz] = taucs_chinfo(C_ptr)    : info on the cholesky factorisation
 *           [C,p] = taucs_chget(C_ptr)     : getting C and p  (P'AP = C'C)
 *
 *
 *     The corresponding C interfaces names are sci_<scilab_name>  
 *
 ******************************************************************************
 */

#include <string.h>
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_umfpack.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
#include "taucs_scilab.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] = 
{
  {sci_umfpack, "umfpack"      },
  {sci_umf_lufact, "umf_lufact"   },
  {sci_umf_lusolve, "umf_lusolve"  },
  {sci_umf_ludel, "umf_ludel"    },
  {sci_umf_luinfo, "umf_luinfo"   },
  {sci_umf_luget, "umf_luget"    },
  {sci_taucs_chfact, "taucs_chfact" },
  {sci_taucs_chsolve, "taucs_chsolve"},
  {sci_taucs_chdel, "taucs_chdel"  },
  {sci_taucs_chinfo, "taucs_chinfo" },
  {sci_taucs_chget, "taucs_chget"  },
  {sci_res_with_prec, "res_with_prec"}
};

/*--------------------------------------------------------------------------*/
int gw_umfpack(void)
{
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
