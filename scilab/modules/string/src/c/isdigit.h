
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __ISDIGIT_H__
#define __ISDIGIT_H__

#include "BOOL.h"

/**
* array elements that are digit between 0 and 9
* @param[in] a string
* @return BOOL array 
*/
BOOL *IsDigit(char *input_string);

#endif /* __ISDIGIT_H__ */
/*--------------------------------------------------------------------------*/
