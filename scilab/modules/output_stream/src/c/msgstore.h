
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - AS
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __MSGSTORE_H__
#define __MSGSTORE_H__

#include "machine.h"
#include "dynlib_output_stream.h"

/* max numbers of lines in "error" buffer */
#define MAX_MSG_LINES  20

/**
 * TODO : comment
 * @param n
 */
OUTPUT_STREAM_IMPEXP int C2F(errstore)(int *n);


/**
 * TODO : comment
 * @param n
 */
OUTPUT_STREAM_IMPEXP int C2F(linestore)(int *n);

/**
 * TODO : comment
 * @param str
 * @param n
 * @param length str (required by fortran)
 */
OUTPUT_STREAM_IMPEXP int C2F(funnamestore)(char *str,int *n,int lenstr);

/**
 * TODO : comment
 * @param str
 * @param n
 */
OUTPUT_STREAM_IMPEXP int C2F(msgstore)(char *str,int *n);

/**
 * TODO : comment
 */
OUTPUT_STREAM_IMPEXP void C2F(freemsgtable)(void);


/**
 * TODO : comment
 * @param fname
 * @param fname_len
 */
OUTPUT_STREAM_IMPEXP int C2F(lasterror)(char *fname, unsigned long fname_len);


/**
 * TODO : comment
 */
OUTPUT_STREAM_IMPEXP int GetLastErrorCode(void);

#endif /* __MSGSTORE_H__ */
