/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __GET_UICONTROL_H__
#define __GET_UICONTROL_H__

#include "BOOL.h"
#include "returnProperty.h"
#include "dynlib_gui.h"
#include "setHandleProperty.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "sci_types.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

GUI_IMPEXP int GetUicontrolIcon(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolBackgroundColor(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolFontAngle(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolFontName(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolFontSize(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolFontUnits(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolFontWeight(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolHorizontalAlignment(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolListboxTop(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolMax(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolMin(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolPosition(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolPosition(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolSliderStep(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolString(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolStyle(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolTooltipString(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolUnits(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolValue(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolVerticalAlignment(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUimenuChecked(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUimenuLabel(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUiobjectEnable(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUiobjectForegroundColor(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetConsoleShowHiddenHandles(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetConsoleShowHiddenProperties(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolRelief(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetConsoleUseDeprecatedLF(void* _pvCtx, int iObjUID);
GUI_IMPEXP int GetUicontrolTooltipString(void* _pvCtx, int iObjUID);

char *IntToStyle(int _iStyle);

#endif /* __GET_UICONTROL_H__ */
