/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_HISTORY_BROWSER_H__
#define __GW_HISTORY_BROWSER_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_history_browser.h"
/*--------------------------------------------------------------------------*/
HISTORY_BROWSER_IMPEXP int gw_history_browser(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
HISTORY_BROWSER_IMPEXP int sci_browsehistory(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_HISTORY_BROWSER_H__ */
