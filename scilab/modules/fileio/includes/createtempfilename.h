/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
