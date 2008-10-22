
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
#ifndef __LOADLIBRARYPATH_H__
#define __LOADLIBRARYPATH_H__

#include "BOOL.h" /* BOOL */

/**
* add paths to java.library.path
* @param a filename SCI/etc/librarypath.xml
* @return TRUE or FALSE
*/
BOOL LoadLibrarypath(char *xmlfilename);

#endif /* __LOADLIBRARYPATH_H__ */
/*--------------------------------------------------------------------------*/ 
