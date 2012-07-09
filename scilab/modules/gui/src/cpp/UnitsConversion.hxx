/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Return the style name corresponding to the int value from UicontrolStyle enum
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __UNITS_CONVERSION_HXX__
#define __UNITS_CONVERSION_HXX__

#include "CallScilabBridge.hxx"
extern "C"
{
#include "getScilabJavaVM.h"
#include "localization.h"
#include "sciprint.h"
#include "GetProperty.h"
#include "HandleManagement.h"
}

#define POINT_PER_INCH 72
#define CM_PER_INCH 2.54

/**
* Convert a pixel value to mm units
* function used by GetScreenProperty
* @param value the value to convert
* @return the new value
*/
double pixelTomm(double dvalue);

/**
* Convert a pixel value to cm units
* function used by GetScreenProperty
* @param value the value to convert
* @return the new value
*/
double pixelTocm(double dvalue);

/**
* Convert a pixel value to inch units
* function used by GetScreenProperty
* @param value the value to convert
* @return the new value
*/
double pixelToinch(double dvalue);

/**
* Convert a pixel value to point units
* function used by GetScreenProperty
* @param value the value to convert
* @return the new value
*/
int pixelTopt(double dvalue);

#endif /*!__UNITS_CONVERSION_HXX__*/
