
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
