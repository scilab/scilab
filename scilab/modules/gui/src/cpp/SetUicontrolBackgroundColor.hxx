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

#ifndef __SET_UICONTROL_BACKGROUNDCOLOR_HXX__
#define __SET_UICONTROL_BACKGROUNDCOLOR_HXX__

#include "CallScilabBridge.hxx"
extern "C"
{
#include "sci_types.h"
#include "SetUicontrolBackgroundColor.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "returnProperty.h"
#include "localization.h"
#include "getScilabJavaVM.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "UicontrolStyleToString.h"
#include "checkColorRange.h"
}

#endif /* __SET_UICONTROL_BACKGROUNDCOLOR_HXX__ */
