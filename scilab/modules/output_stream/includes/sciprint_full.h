
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Francois VOGEL
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
