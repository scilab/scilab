/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#ifndef __COMPLETION_GENERIC_H__
#define __COMPLETION_GENERIC_H__

/**
* completion
* @param[in] dictionary
* @param[in] size of dictionary
* @param[in] some chars of a symbol
* @param[out] size of returned strings
* @return strings found in dictionary
*/
char **completion_generic(char **dictionary, int sizedictionary,
                          char *somechars, int *sizeArrayReturned);

#endif /* __COMPLETION_GENERIC_H__ */
/*--------------------------------------------------------------------------*/
