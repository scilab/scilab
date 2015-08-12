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

#ifndef __SET_UICONTROL_H__
#define __SET_UICONTROL_H__

#include <stdio.h>

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
#include "checkColorRange.h"
#include "getPropertyAssignedValue.h"
#include "stricmp.h"

GUI_IMPEXP int SetUicontrolIcon(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolBackgroundColor(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolFontAngle(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolFontName(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolFontSize(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolFontUnits(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolFontWeight(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolHorizontalAlignment(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolListboxTop(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolMax(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolMin(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolPosition(int iObjUID, void* pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolRelief(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolSliderStep(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolString(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol ) ;
GUI_IMPEXP int SetUicontrolTooltipString(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol ) ;
GUI_IMPEXP int SetUicontrolUnits(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolValue(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUicontrolVerticalAlignment(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUimenuChecked(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUimenuLabel(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetUiobjectEnable(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol ) ;
GUI_IMPEXP int SetUiobjectForegroundColor(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);
GUI_IMPEXP int SetConsoleShowHiddenHandles(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol ) ;
GUI_IMPEXP int SetConsoleShowHiddenProperties(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol ) ;
GUI_IMPEXP int SetConsoleUseDeprecatedLF(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);

char *IntToStyle(int _iStyle);
#endif /* __SET_UICONTROL_H__ */
