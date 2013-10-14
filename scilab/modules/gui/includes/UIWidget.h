/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __UIWIDGET_H__
#define __UIWIDGET_H__
/*--------------------------------------------------------------------------*/

#include "dynlib_gui.h"
GUI_IMPEXP void deleteUIWidget(const long long hdl);

void deleteAllUIWidget();

GUI_IMPEXP int isValidUIWidget(const long long hdl);

GUI_IMPEXP void showWindowUIWidget(const long long hdl);

/*--------------------------------------------------------------------------*/
#endif /* __UIWIDGET_H__ */
/*--------------------------------------------------------------------------*/
