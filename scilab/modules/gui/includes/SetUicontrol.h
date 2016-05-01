/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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
#include "os_string.h"

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

const char *IntToStyle(int _iStyle);
#endif /* __SET_UICONTROL_H__ */
