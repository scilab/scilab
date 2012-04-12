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

#include "UnitsConversion.hxx"

using namespace org_scilab_modules_gui_bridge;

double pixelTomm(double dvalue)
{
	return (double) (pixelTocm(dvalue) * 10.0);
}

double pixelTocm(double dvalue)
{
	return (double) (pixelToinch(dvalue) * CM_PER_INCH);
}

double pixelToinch(double dvalue)
{
	return (double) (dvalue * (1.0 / POINT_PER_INCH));
}

int pixelTopt(double dvalue)
{
	return (int)(dvalue * POINT_PER_INCH / CallScilabBridge::getScreenResolution(getScilabJavaVM()));
}
