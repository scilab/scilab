/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
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
#ifndef __CREATETEMPFILENAME_H__
#define __CREATETEMPFILENAME_H__

#include "dynlib_fileio.h"
#include "charEncoding.h"
#include "BOOL.h"

/**
* Create file with unique file name in TMPDIR
* TMPDIR/PREFIX[XXXXXXX]
* @param[in] prefix
* @param[in] bShortFormat
* @return a unique filename
*/
FILEIO_IMPEXP char *createtempfilename(const char *prefix, BOOL bShortFormat);

FILEIO_IMPEXP wchar_t *createtempfilenameW(const wchar_t *wcprefix, BOOL bShortFormat);

#endif /* __CREATETEMPFILENAME_H__ */
/*--------------------------------------------------------------------------*/
