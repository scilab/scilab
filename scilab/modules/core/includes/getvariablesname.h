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
#ifndef __GETVARIABLESNAME_H__
#define __GETVARIABLESNAME_H__

#include "BOOL.h" /* BOOL */

/**
* get names of all variables (global+local) on scilab
* @param[out]size of array
* @param[in] BOOL to sort output
* @return an array of strings
*/
char **getVariablesName(int *sizearray, BOOL sorted);

#endif /*__GETVARIABLESNAME_H__ */
/*--------------------------------------------------------------------------*/
