/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __NUMTOKENS_H__
#define __NUMTOKENS_H__

#include "dynlib_string.h"

/**
* get position of a numeric token
* @param[in] a string
* @return a position
*/
STRING_IMPEXP int NumTokens(char *string);

#endif /* __NUMTOKENS_H__ */ 