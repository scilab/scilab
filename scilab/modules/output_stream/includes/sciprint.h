
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCIPRINT_H__
#define __SCIPRINT_H__

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "BOOL.h"
#include "dynlib_output_stream.h"

/**
 * Print
 * @param fmt Format of the format string
 * @param ...
 */
OUTPUT_STREAM_IMPEXP void sciprint(char *fmt,...) ;


/**
* Print: works as sciprint
* difference is that the argument list is a pointer to a list of arguments
* @param fmt Format of the format string
* @param[in] fmt
* @param[in] va_list
* @return nb characters
*/

OUTPUT_STREAM_IMPEXP int scivprint(char *fmt,va_list args);

#ifdef __cplusplus
}
#endif

#endif /* __SCIPRINT_H__ */
/*--------------------------------------------------------------------------*/ 
