/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
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

/*--------------------------------------------------------------------------*/
#ifndef __GW_ARNOLDI_H__
#define __GW_ARNOLDI_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_arnoldi.h"
/*--------------------------------------------------------------------------*/
ARNOLDI_IMPEXP int gw_arnoldi(void);
/*--------------------------------------------------------------------------*/
ARNOLDI_IMPEXP int sci_dsaupd(char *fname, void* pvApiCtx);
ARNOLDI_IMPEXP int sci_dnaupd(char *fname, void* pvApiCtx);
ARNOLDI_IMPEXP int sci_znaupd(char *fname, void* pvApiCtx);
ARNOLDI_IMPEXP int sci_dseupd(char *fname, void* pvApiCtx);
ARNOLDI_IMPEXP int sci_dneupd(char *fname, void* pvApiCtx);
ARNOLDI_IMPEXP int sci_zneupd(char *fname, void* pvApiCtx);
ARNOLDI_IMPEXP int sci_eigs(char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
#endif /* __GW_ARNOLDI_H__ */
/*--------------------------------------------------------------------------*/

