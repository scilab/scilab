
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
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
#ifndef __MEN_SUTILS_H__
#define __MEN_SUTILS_H__

#include "dynlib_string.h"

/**
* Converts a Scilab String coded as int array
* into a C string [ the C string is allocated with MALLOC ]
* @param[in] size of int array
* @param[in] int array
* @param[out] C string converted from int array
* @param[out] error code 0 = OK , 1 = Problem
*/
STRING_IMPEXP void ScilabStr2C(int *n, int *Scistring, char **strh, int *ierr);


/**
* Converts a Scilab array of
* String coded as int array
* into a C array of strings [ NULL terminated ]
* @param[in] Scistring
* @param[in] nstring
* @param[in] ptrstrings
* @param[out] C string converted from int array
* @param[out] error code 0 = OK , 1 = Problem
*/
STRING_IMPEXP void ScilabMStr2CM(int *Scistring, int *nstring, int *ptrstrings, char ***strh, int *ierr);

/**
* Converts a Scilab array of
* String coded as int array
* into a C string where all the Scilab strings are
* separated by '\n' or '\r\n' (windows)
* desc,nd,ptrdesc : scilab string info
* strh : the C coded string
* @param[in] descriptor
* @param[in] size of array of integer
* @param[in] pointer on descriptor
* @param[out] C string converted from int array
* @param[out] error code 0 = OK , 1 = Problem
*/
STRING_IMPEXP void ScilabMStr2C(int *desc, int *nd, int *ptrdesc, char **strh, int *ierr);

#endif /*  __MEN_SUTILS_H__ */
/*--------------------------------------------------------------------------*/
