
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - Allan CORNET - 2009
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
#ifndef __ISALPHANUM_H__
#define __ISALPHANUM_H__

#include <wchar.h>
#include "BOOL.h"

#include "dynlib_string.h"
/**
* array elements that are alphabetic letters or digits
* @param[in] a string
* @return BOOL array
*/
STRING_IMPEXP BOOL *isalphanumW(wchar_t *input_string, int *returnedSize);

#endif /* __ISALPHANUM_H__ */
/*--------------------------------------------------------------------------*/
