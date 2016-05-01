/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
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

#ifndef __SETENVC_H__
#define __SETENVC_H__

#include <wchar.h>

#include "dynlib_io.h"
#include "BOOL.h"

/**
 * Set in the system environment a variable to a value
 * @param[in] string the name of the variable
 * @param[in] value the value of the variable "string"
 * @return FALSE if there is a problem otherwise TRUE
 */
IO_IMPEXP BOOL setenvc(const char *string, const char *value);
IO_IMPEXP BOOL setenvcW(const wchar_t *string, const wchar_t *value);

#endif /* __SETENVC_H__ */
/*--------------------------------------------------------------------------*/
