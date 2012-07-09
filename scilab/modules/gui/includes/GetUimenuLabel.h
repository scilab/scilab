/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GET_UIMENU_LABEL_H__
#define __GET_UIMENU_LABEL_H__
#include "dynlib_gui.h"
#include "returnProperty.h"

/**
 * Get the label of an uimenu
 *
 * @param pobjUID UID of the uimenu
 * @return the label
 */
GUI_IMPEXP int GetUimenuLabel(void* _pvCtx, char *pobjUID);

#endif /* __GET_UIMENU_LABEL_H__ */
