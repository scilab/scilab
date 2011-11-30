/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2011 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/ 
#ifndef __GETVIDEOADAPTERS_H__
#define __GETVIDEOADAPTERS_H__

#include "dynlib_scilab_windows.h"

SCILAB_WINDOWS_IMPEXP char **getVideoAdapters(int *returnedNbAdapters);
SCILAB_WINDOWS_IMPEXP char * GetPrimaryVideoCardVersion(void);
#endif /* __GETVIDEOADAPTERS_H__ */
/*--------------------------------------------------------------------------*/ 
