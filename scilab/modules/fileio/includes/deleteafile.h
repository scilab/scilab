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
#ifndef __DELETEAFILE_H__
#define __DELETEAFILE_H__

#include <wchar.h>
#include "BOOL.h"
#include "dynlib_fileio.h"
/**
* delete a file by filename
* @param[in] filename
* @return BOOLEAN TRUE or FALSE
*/
FILEIO_IMPEXP BOOL deleteafile(const char *filename);

/**
* delete a file by filename
* @param[in] filenameW wide string
* @return BOOLEAN TRUE or FALSE
*/
FILEIO_IMPEXP BOOL deleteafileW(const wchar_t *filenameW);

#endif /* __DELETEAFILE_H__ */
/*--------------------------------------------------------------------------*/
