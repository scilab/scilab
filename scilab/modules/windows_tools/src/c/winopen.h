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
#ifndef __WINOPEN_H__
#define __WINOPEN_H__

#include "machine.h" /* BOOL */

/**
* Opens the item specified by the filename parameter. The item can be a file or folder
* @param[in] a filename. SCI is converted 
* @return TRUE or FALSE
*/
BOOL winopen(char *scilabfilename);

#endif /* __WINOPEN_H__ */
/*--------------------------------------------------------------------------*/
