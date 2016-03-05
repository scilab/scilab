/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*/

#ifndef __ERROR_API__
#define __ERROR_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_error.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum __SCILAB_STATUS__
{
    STATUS_OK = 0,
    STATUS_ERROR = 1
} scilabStatus;

void scilab_setError(scilabEnv env, const wchar_t* msg);
const wchar_t* scilab_lastError(scilabEnv env);

#ifdef __cplusplus
}
#endif
#endif /* __ERROR_API__ */
