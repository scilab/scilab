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
#include "charEncoding.h"

typedef enum
{
    MGETL_NO_ERROR = 0,
    MGETL_EOF = 1,
    MGETL_MEMORY_ALLOCATION_ERROR = 2,
    MGETL_ERROR = 3
} mgetlError;

/* file descriptor id for stdin */
#define STDIN_ID 5
/* file descriptor id for stdout */
#define STDOUT_ID 6

/**
 * @fn  char ** mgetl(int fd, int nbLinesIn, int *nbLinesOut, int *ierr) #endif
 *
 * @brief   read lines of a file.
 *
 * @author  Allan Cornet
 * @date    4/16/2010
 *
 * @param   fd                  The file descriptor id.
 * @param   nbLinesIn           The nb lines in .
 * @param [in,out]  nbLinesOut  If non-null, the nb lines out.
 * @param [in,out]  ierr        If non-null, the ierr.
 *
 * @return  null if it fails, else strings readed.
**/
FILEIO_IMPEXP wchar_t ** mgetl(int fd, int nbLinesIn, int *nbLinesOut, int *ierr);

#endif /* __MGETL_H__ */
/*--------------------------------------------------------------------------*/
