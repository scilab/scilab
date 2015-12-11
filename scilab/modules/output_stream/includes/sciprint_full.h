
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Francois VOGEL
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SCIPRINT_FULL_H__
#define __SCIPRINT_FULL_H__

#include <stdarg.h>
#include "dynlib_output_stream.h"

/**
* sciprint geared towards long strings (>MAXPRINTF)
* the long string is splitted in elements of length
* equal to the number of columns from lines()
* @param fmt Format of the format string
* @param ...
*/
OUTPUT_STREAM_IMPEXP void sciprint_full(char *fmt, ...) ;

#endif /* __SCIPRINT_FULL_H__ */
/*--------------------------------------------------------------------------*/
