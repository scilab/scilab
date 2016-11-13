
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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

#ifndef __SCIPRINT_H__
#define __SCIPRINT_H__

#include <stdarg.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "BOOL.h"

/**
 * Print
 * @param fmt Format of the format string
 * @param ...
 */
void sciprint(const char *fmt, ...);
//void sciprintW(wchar_t* fmt, ...);


/**
* Print: works as sciprint
* difference is that the argument list is a pointer to a list of arguments
* @param fmt Format of the format string
* @param[in] fmt
* @param[in] va_list
* @return nb characters
*/

int scivprint(const char* fmt, va_list args);
//int scivprintW(wchar_t* fmt, va_list args);

#ifdef __cplusplus
}
#endif

#endif /* __SCIPRINT_H__ */
/*--------------------------------------------------------------------------*/
