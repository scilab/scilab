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
#include "ObjectStructure.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "sciprint.h"
#include "ObjectStructure.h"
#include "GetProperty.h"
}

#define POINT_PER_INCH 72
#define CM_PER_INCH 2.54

/*
 * Convert a value from Point units to another unit
 *
 * @param value the value to convert
 * @param newUnit the new unit
 * @param sciObj the font parent
 * @param widthAsRef TRUE to use width to compute normalized size, FALSE to use height to compute normalized size
 * @return the new value
 */
double ConvertFromPoint(int value, int newUnit, sciPointObj *sciObj, BOOL widthAsRef);

/*
 * Convert a value to Point units
 *
 * @param value the value to convert
 * @param newUnit the new unit
 * @param sciObj the font parent
 * @param widthAsRef TRUE to use width to compute normalized size, FALSE to use height to compute normalized size
 * @return the new value
 */
int ConvertToPoint(double value, int oldUnit, sciPointObj *sciObj, BOOL widthAsRef);

/*
 * Convert a value from Pixels units to another unit
 *
 * @param value the value to convert
 * @param newUnit the new unit
 * @param sciObj the font parent
 * @param widthAsRef TRUE to use width to compute normalized size, FALSE to use height to compute normalized size
 * @return the new value
 */
double ConvertFromPixel(int value, int newUnit, sciPointObj *sciObj, BOOL widthAsRef);

/*
 * Convert a value to Pixel units
 *
 * @param value the value to convert
 * @param newUnit the new unit
 * @param sciObj the font parent
 * @return the new value
 * @param widthAsRef TRUE to use width to compute normalized size, FALSE to use height to compute normalized size
 */
int ConvertToPixel(double value, int oldUnit, sciPointObj *sciObj, BOOL widthAsRef);

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
