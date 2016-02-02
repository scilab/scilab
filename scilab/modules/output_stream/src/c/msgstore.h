/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - AS
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
