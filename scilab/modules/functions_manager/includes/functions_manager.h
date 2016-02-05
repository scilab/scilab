/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

#include "dynlib_functions_manager.h"

#ifndef __FUNCMANAGER_H__
#define __FUNCMANAGER_H__


FUNCMAN_IMEXP bool LoadModules();
FUNCMAN_IMEXP void UnloadModules();
FUNCMAN_IMEXP void EndModules();
FUNCMAN_IMEXP void destroyfunctionManagerInstance(void);
#endif /* !__FUNCMANAGER_H__ */
