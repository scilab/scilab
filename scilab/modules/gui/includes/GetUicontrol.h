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

GUI_IMPEXP void* GetUicontrolIcon(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolBackgroundColor(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolFontAngle(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolFontName(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolFontSize(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolFontUnits(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolFontWeight(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolHorizontalAlignment(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolListboxTop(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolMax(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolMin(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolPosition(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolPosition(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolSliderStep(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolString(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolStyle(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolTooltipString(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolUnits(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolValue(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolVerticalAlignment(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUimenuChecked(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUimenuLabel(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUiobjectEnable(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUiobjectForegroundColor(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetConsoleShowHiddenHandles(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetConsoleShowHiddenProperties(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolRelief(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetConsoleUseDeprecatedLF(void* _pvCtx, int iObjUID);
GUI_IMPEXP void* GetUicontrolTooltipString(void* _pvCtx, int iObjUID);

const char *IntToStyle(int _iStyle);

#endif /* __GET_UICONTROL_H__ */
