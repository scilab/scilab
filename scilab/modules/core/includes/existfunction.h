/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __EXISTFUNCTION_H__
#define __EXISTFUNCTION_H__

#include "BOOL.h"
/** 
 * Check if a function exists in the list
 * @param[in] name the name of the function
 * @return if there is this function or not
 */
BOOL ExistFunction(char *name);

/*--------------------------------------------------------------------------*/
#endif /* __EXISTFUNCTION_H__ */
/*--------------------------------------------------------------------------*/
