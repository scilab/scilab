/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
#ifndef __GETSCILABDIRECTORY_H__
#define __GETSCILABDIRECTORY_H__

#include "dynlib_scilab_windows.h"
#include "BOOL.h" /* BOOL */

/**
* get scilab directory (windows)
* @param [in]
* @return
*/
SCILAB_WINDOWS_IMPEXP char *getScilabDirectory(BOOL UnixStyle);

#endif /* __GETSCILABDIRECTORY_H__ */
/*--------------------------------------------------------------------------*/
