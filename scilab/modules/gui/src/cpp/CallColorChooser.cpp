/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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

#include "CallColorChooser.hxx"

using namespace org_scilab_modules_gui_bridge;

int createColorChooser()
{
    return CallScilabBridge::newColorChooser(getScilabJavaVM());
}

void setColorChooserTitle(int colorChooserID, char *title)
{
    CallScilabBridge::setColorChooserTitle(getScilabJavaVM(), colorChooserID, title);
}

void setColorChooserDefaultRGB(int colorChooserID, double *RGB)
{
    int *RGBint = new int[3];
    RGBint[0] = (int) RGB[0];
    RGBint[1] = (int) RGB[1];
    RGBint[2] = (int) RGB[2];
    CallScilabBridge::setColorChooserDefaultColor(getScilabJavaVM(), colorChooserID, RGBint, 3);
    delete[] RGBint;
}

void setColorChooserDefaultRGBSeparateValues(int colorChooserID, int R, int G, int B)
{
    int *RGB = new int[3];
    RGB[0] = R;
    RGB[1] = G;
    RGB[2] = B;
    CallScilabBridge::setColorChooserDefaultColor(getScilabJavaVM(), colorChooserID, RGB, 3);
    delete[] RGB;
}

double *getColorChooserSelectedRGB(int colorChooserID)
{
    int *RGB = CallScilabBridge::getColorChooserSelectedColor(getScilabJavaVM(), colorChooserID);

    double *RGBdouble = new double[3];
    RGBdouble[0] = RGB[0];
    RGBdouble[1] = RGB[1];
    RGBdouble[2] = RGB[2];

    delete [] RGB;
    return RGBdouble;
}

void colorChooserDisplayAndWait(int colorChooserID)
{
    CallScilabBridge::colorChooserDisplayAndWait(getScilabJavaVM(), colorChooserID);
}
