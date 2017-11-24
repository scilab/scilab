/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#ifndef __MGETL_H__
#define __MGETL_H__

#include "dynlib_fileio.h"

#include <stddef.h>

/**
 * @fn int mgetl(FILE *fd, int iLineCount, wchar_t ***pwstLines)
 *
 * @brief reads all the lines (or a part) of a text file.
 *
 * @param [in]  iFileID     the file ID.
 * @param [in]  iLineCount  the number of lines to read.
 * @param [out] pwstLines   a wide string array containing the lines
 *
 * @return the number of lines read, -1 if an error occurred
**/
FILEIO_IMPEXP int mgetl(int iFileID, int iLineCount, wchar_t ***pwstLines);

#endif /* __MGETL_H__ */
/*--------------------------------------------------------------------------*/
