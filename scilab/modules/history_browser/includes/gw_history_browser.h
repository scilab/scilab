/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Allan CORNET
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
