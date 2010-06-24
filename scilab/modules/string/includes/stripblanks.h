
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __STRIPBLANKS_H__
#define __STRIPBLANKS_H__

#include "BOOL.h"
/**
* stripblanks 
* remove blank or tab characters in a string matrix
* param[in] InputStrings
* param[out] OutputStrings
* param[in] InputStringsDim size of InputStrings 
* param[in] BOOLEAN remove TAB if TRUE
*/


void stripblanks(char **InputStrings,char **OutputStrings,int InputStringsDim,BOOL withTAB);

#endif /* __STRIPBLANKS_H__ */
/*--------------------------------------------------------------------------*/
