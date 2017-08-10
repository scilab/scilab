/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
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
#ifndef __READLINE_H__
#define __READLINE_H__

#include "dynlib_fileio.h"
#include "machine.h"

#define READNEXTLINE_ERROR_EOF_REACHED -1
#define READNEXTLINE_ERROR_EOF_REACHED_AFTER_EOL 0
#define READNEXTLINE_ERROR_EOL 1
#define READNEXTLINE_ERROR_BUFFER_FULL 2
#define READNEXTLINE_ERROR_EOF_REACHED_BEFORE_EOL 3
#define READNEXTLINE_ERROR_ERROR_UNMANAGED 4

/* PLEASE USE mgetl (see mgetl.h) and not these functions */
/* only for compatibility with fortran code */

/**
* read a line from a text file
* used by texmacs mode and readnextline
* @param[in] fd: a int file descriptor to open
* @param[in/out] buf : string buffer returned
* @param[in] n : bsiz (4096)
* @param[in/out]: *cnt numbers of returned characters
* @param[in/out]: *nr numbers of returned characters
* @return info code
* -1 : EOF reached
* 0 : EOF reached after an EOL
* 1 : EOL reached
* 2 : buffer full
* 3 : EOF reached before any EOL
* 4 : ERROR (not managed)
*/
FILEIO_IMPEXP int LineRead(int fd, char buf[], int n, int *cnt, int *nr);

/**
* read a line from a text file
* used by exec to read a .sce file
* @param[in] fd : a int file descriptor to open
* @param[in/out] buf : string buffer returned
* @param[in] *n : bsiz (4096)
* @param[out] *count : numbers of returned characters + 1
* @param[out] *nr : numbers of returned characters
* @param[out] *ierr : int error code
* -1 : EOF reached
* 0 : EOF reached after an EOL
* 1 : EOL reached
* 2 : buffer full
* 3 : EOF reached before any EOL
* 4 : ERROR (not managed)
*/
FILEIO_IMPEXP void C2F(readnextline)(int *fd, char buf[], int *n, int *count, int *nr, int *ierr);

#endif /* __READLINE_H__ */
