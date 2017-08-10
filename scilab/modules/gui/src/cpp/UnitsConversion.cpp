/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Return the style name corresponding to the integer value from UicontrolStyle enum
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
