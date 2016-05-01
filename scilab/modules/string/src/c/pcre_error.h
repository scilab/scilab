
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
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
#ifndef __PCRE_ERROR_H__
#define __PCRE_ERROR_H__

#include "dynlib_string.h"

/**
 *
 * Display a human readable error message (PCRE error).
 * @param fname the name of the Scilab function
 * @param errorCode the error code returned by the pcre function
 */
STRING_IMPEXP void pcre_error(char const * fname, int errorCode);

#endif

