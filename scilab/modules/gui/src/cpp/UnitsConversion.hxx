/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Return the style name corresponding to the int value from UicontrolStyle enum
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
