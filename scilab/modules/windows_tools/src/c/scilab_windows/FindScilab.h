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
#ifndef __FINDSCILAB_H__
#define __FINDSCILAB_H__

#include "dynlib_scilab_windows.h"
#include "BOOL.h" /* BOOL */

/**
* Check if exists another scilab window 
* @return TRUE or FALSE
*/
SCILAB_WINDOWS_IMPEXP BOOL HaveAnotherWindowScilab(void);

/**
* returns name of the first Scilab found
* @return name
*/
SCILAB_WINDOWS_IMPEXP char * getFirstScilabFound(void);

/**
* returns name of the last Scilab found
* @return name
*/
SCILAB_WINDOWS_IMPEXP char * getLastScilabFound(void);

#endif /*__FINDSCILAB_H__ */
/*--------------------------------------------------------------------------*/ 
