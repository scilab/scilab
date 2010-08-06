/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __MPUTL_H__
#define __MPUTL_H__

typedef enum { 
    MPUTL_NO_ERROR = 0,
    MPUTL_ERROR = 1,
    MPUTL_INVALID_FILE_DESCRIPTOR = 2,
    MPUTL_NO_WRITE_RIGHT = 3
} mputlError;

/* scilab file descriptor id for stdout (see %io in scilab) */
#define STDOUT_ID 6

/* scilab file descriptor id for stdin (see %io in scilab) */
#define STDIN_ID 5

/**
* mputl function
* @param[in] file descriptor from mopen
* @param[in] strings to write
* @param[in] numbers of strings to write
* @return enum error
*/
mputlError mputl(int fileDescriptor, char **pStrings, int sizeStrings);

#endif /* __MPUTL_H__ */
/*--------------------------------------------------------------------------*/

