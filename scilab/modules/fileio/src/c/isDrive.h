/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#ifndef __ISDRIVE_H__
#define __ISDRIVE_H__

#include <wchar.h>

#include "BOOL.h" /* BOOL */

/**
* check if strname is a drive (only for windows)
* @param[in] a string
* @return TRUE or FALSE
* returns always false on linux
*/
BOOL isDrive(const char *strname);

/**
* check if strname is a drive (only for windows)
* @param[in] a wide string
* @return TRUE or FALSE
* returns always false on linux
*/

BOOL isDriveW(const wchar_t *wcstrname);

#endif
/*--------------------------------------------------------------------------*/
