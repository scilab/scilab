/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_ARNOLDI_H__
#define __GW_ARNOLDI_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_arnoldi.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
ARNOLDI_IMPEXP int gw_arnoldi(void);
/*--------------------------------------------------------------------------*/
ARNOLDI_IMPEXP int sci_dsaupd(char *fname,unsigned long fname_len);
ARNOLDI_IMPEXP int sci_dnaupd(char *fname,unsigned long fname_len);
ARNOLDI_IMPEXP int sci_znaupd(char *fname,unsigned long fname_len);
ARNOLDI_IMPEXP int sci_dseupd(char *fname,unsigned long fname_len);
ARNOLDI_IMPEXP int sci_dneupd(char *fname,unsigned long fname_len);
ARNOLDI_IMPEXP int sci_zneupd(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /* __GW_ARNOLDI_H__ */
/*--------------------------------------------------------------------------*/

