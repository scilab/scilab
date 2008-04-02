/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Return the style name corresponding to the integer value from UicontrolStyle enum
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
}

#define POINT_PER_INCH 72
#define CM_PER_INCH 2.54

/*
 * Convert a value from Point units to another unit
 *
 * @param value the value to convert
 * @param newUnit the new unit
 * @param sciObj the font parent
 * @return the new value
 */
double ConvertFromPoint(int value, int newUnit, sciPointObj *sciObj);

/*
 * Convert a value to Point units
 *
 * @param value the value to convert
 * @param newUnit the new unit
 * @param sciObj the font parent
 * @return the new value
 */
int ConvertToPoint(double value, int oldUnit, sciPointObj *sciObj);

#endif /*!__UNITS_CONVERSION_HXX__*/
