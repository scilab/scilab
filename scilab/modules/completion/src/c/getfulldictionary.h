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

#ifndef __GETFULLDICTIONARY_H__
#define _GETFULLDICTIONARY_H__

/**
* get full scilab dictionary (macros,functions,variables,...)
* @param[out] size of returned array
* @return array of strings
*/
char **getfulldictionary(int *sizearray);

#endif /* _GETFULLDICTIONARY_H__ */
/*--------------------------------------------------------------------------*/
