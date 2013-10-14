/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __CURRENT_SUBWIN_H__
#define __CURRENT_SUBWIN_H__

#include "dynlib_graphic_objects.h"

GRAPHIC_OBJECTS_IMPEXP int getCurrentSubWin(void);

GRAPHIC_OBJECTS_IMPEXP void setCurrentSubWin(int iUID);

#endif /* !__CURRENT_SUBWIN_H__ */
