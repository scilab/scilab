
/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#ifndef __SCSTRTOSTR_H__
#define __SCSTRTOSTR_H__

#include "dynlib_string.h"

/*!
* \brief
* Converts a Scilab array of  String coded as int array
* into a regular string.
* entries of the original array are catenated, separated by
* '\n'   char
*
* \param Scistring
* scilab string
*
* \param nstring
* number of scilab scilab string
*
* \param ptrstrings
* returns size of array of string
*
* \param strh
* array of strings
*
* \returns
* return value 0 or MEM_LACK.
*

*/
STRING_IMPEXP int SciStrtoStr (int *Scistring, int *nstring, int *ptrstrings, char **strh);

#endif /* __SCSTRTOSTR_H__ */
