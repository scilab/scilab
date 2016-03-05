/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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

#ifndef __BASENAME_H__
#define __BASENAME_H__

#include <wchar.h>
#include "BOOL.h"
#include "dynlib_fileio.h"

/**
* strip directory and suffix from filenames (wide string)
* @param[in] filename
* @param[in] BOOL flagexpand
* @return basename
*/
FILEIO_IMPEXP wchar_t *basenameW(wchar_t *wcfullfilename, BOOL bExpand);

#endif
