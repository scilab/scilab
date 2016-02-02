
/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
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
