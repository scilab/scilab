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
/*--------------------------------------------------------------------------*/
#ifndef __CREATEDIRECTORY_H__
#define __CREATEDIRECTORY_H__

#include "machine.h" /* BOOL */

/**
* Create a directory
* @param path the path of the futur directory
* @return the result of the operation TRUE or FALSE
*/
BOOL createdirectory(const char *path);

#endif /* __CREATEDIRECTORY_H__ */
/*--------------------------------------------------------------------------*/ 
