/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GETUIOBJECTENABLE_H__
#define __GETUIOBJECTENABLE_H__
#include "dynlib_gui.h"
#include "returnProperty.h"

/**
 * Get the enable status of a uicontrol or uimenu
 *
 * @param pObjUID Scilab object corresponding to the uicontrol or uimenu
 * @return true if the status is got without error
 */
GUI_IMPEXP int GetUiobjectEnable(void* _pvCtx, char *pObjUID);

#endif /* !__GETUIOBJECTENABLE_H__ */
