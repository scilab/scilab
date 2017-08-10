/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
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

#ifndef __GW_UMFPACK__
#define __GW_UMFPACK__
/*--------------------------------------------------------------------------*/
#include "dynlib_umfpack.h"
/*--------------------------------------------------------------------------*/
UMFPACK_IMPEXP int gw_umfpack(void);
/*--------------------------------------------------------------------------*/
UMFPACK_IMPEXP int sci_umfpack(char* fname, void* pvApiCtx);
UMFPACK_IMPEXP int sci_umf_lufact(char* fname, void* pvApiCtx);
UMFPACK_IMPEXP int sci_umf_lusolve(char* fname, void* pvApiCtx);
UMFPACK_IMPEXP int sci_umf_ludel(char* fname, void* pvApiCtx);
UMFPACK_IMPEXP int sci_umf_luinfo(char* fname, void* pvApiCtx);
UMFPACK_IMPEXP int sci_umf_luget(char* fname, void* pvApiCtx);
UMFPACK_IMPEXP int sci_taucs_chfact(char* fname, void* pvApiCtx);
UMFPACK_IMPEXP int sci_taucs_chsolve(char* fname, void* pvApiCtx);
UMFPACK_IMPEXP int sci_taucs_chdel(char* fname, void* pvApiCtx);
UMFPACK_IMPEXP int sci_taucs_chinfo(char* fname, void* pvApiCtx);
UMFPACK_IMPEXP int sci_taucs_chget(char* fname, void* pvApiCtx);
UMFPACK_IMPEXP int sci_res_with_prec(char* fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_UMFPACK__ */
/*--------------------------------------------------------------------------*/
