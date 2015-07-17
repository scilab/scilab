/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "dynlib_functions_manager.h"

#ifndef __FUNCMANAGER_H__
#define __FUNCMANAGER_H__


FUNCMAN_IMEXP void LoadModules();
FUNCMAN_IMEXP void UnloadModules();
FUNCMAN_IMEXP void EndModules();
FUNCMAN_IMEXP void destroyfunctionManagerInstance(void);
#endif /* !__FUNCMANAGER_H__ */