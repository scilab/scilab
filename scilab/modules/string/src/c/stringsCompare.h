
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
#ifndef __STRINGSCOMPARE_H__
#define __STRINGSCOMPARE_H__

#include "BOOL.h"

/** 
* Compare strings matrix
* @param[in] String matrix One
* @param[in] Dimension m*n of String matrix one
* @param[in] String matrix Two
* @param[in] Dimension m*n of String matrix two
* @param[in] BOOL to do stricmp
* @return a integer matrix (result of strcmp)
*/
int * stringsCompare(char **Input_String_One,int dim_One,char **Input_String_Two,int dim_Two,BOOL dostricmp);

#endif /* __STRINGSCOMPARE_H__ */
/*--------------------------------------------------------------------------*/
