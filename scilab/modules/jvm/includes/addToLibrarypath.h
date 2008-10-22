
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
#ifndef __ADDTOLIBRARYPATH_H__
#define __ADDTOLIBRARYPATH_H__

#include "BOOL.h" /* BOOL */

/**
* add a path to java.library.path
* @param a new path to add
* @return TRUE or FALSE
*/
BOOL addToLibrarypath(char *librarypathstring);

#endif /* __ADDTOLIBRARYPATH_H__ */
/*--------------------------------------------------------------------------*/ 
