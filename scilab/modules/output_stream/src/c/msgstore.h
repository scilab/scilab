/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - AS
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __MSGSTORE_H__
#define __MSGSTORE_H__

#include "machine.h"
#include "dynlib_output_stream.h"

/* all these functions are obsolete */
/* only used from fortran */
/* see lasterror.h */

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
OUTPUT_STREAM_IMPEXP int C2F(funnamestore)(char *str, int *n, int lenstr);

/**
* TODO : comment
* @param str
* @param n
*/
OUTPUT_STREAM_IMPEXP int C2F(msgstore)(char *str, int *n);

/**
* TODO : comment
*/
OUTPUT_STREAM_IMPEXP int GetLastErrorCode(void);

#endif /* __MSGSTORE_H__ */
