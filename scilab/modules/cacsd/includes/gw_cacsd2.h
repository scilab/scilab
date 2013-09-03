
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_CACSD2__
#define __GW_CACSD2__
/*--------------------------------------------------------------------------*/
#include "dynlib_cacsd.h"
/*--------------------------------------------------------------------------*/
CACSD_IMPEXP int gw_cacsd2(void);
/*--------------------------------------------------------------------------*/
int sci_ppol(char *fname, unsigned long fname_len);
int sci_tzer(char *fname, unsigned long fname_len);
int sci_freq(char *fname, unsigned long fname_len);
int sci_ltitr(char *fname, unsigned long fname_len);
int sci_rtitr(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_GW_CACSD2__ */
/*--------------------------------------------------------------------------*/

