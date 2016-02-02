/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Get the position of an uicontrol
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

#include "GetScreenProperty.hxx"

using namespace org_scilab_modules_gui_bridge;


void* GetScreenProperty(void* _pvCtx, char *propertyName)
{
    double *value = new double[4];

    void* flag = NULL;

    if (!stricmp(propertyName, "screensize_px"))
    {
        value[0] = 1.0;
        value[1] = 1.0;
        value[2] = CallScilabBridge::getScreenWidth(getScilabJavaVM());
        value[3] = CallScilabBridge::getScreenHeight(getScilabJavaVM());

        flag = sciReturnRowVector(value, 4);

        delete[] value;

        return flag;

    }
    else if (!stricmp(propertyName, "screensize_mm"))
    {
        value[0] = 0.0;
        value[1] = 0.0;
        value[2] = pixelTomm( CallScilabBridge::getScreenWidth(getScilabJavaVM()) );
        value[3] = pixelTomm( CallScilabBridge::getScreenHeight(getScilabJavaVM()) );

        flag = sciReturnRowVector(value, 4);

        delete[] value;

        return flag;
    }
    else if (!stricmp(propertyName, "screensize_cm"))
    {
        value[0] = 0.0;
        value[1] = 0.0;
        value[2] = pixelTocm( CallScilabBridge::getScreenWidth(getScilabJavaVM()) );
        value[3] = pixelTocm( CallScilabBridge::getScreenHeight(getScilabJavaVM()) );

        flag = sciReturnRowVector(value, 4);

        delete[] value;

        return flag;
    }
    else if (!stricmp(propertyName, "screensize_in"))
    {
        value[0] = 0.0;
        value[1] = 0.0;
        value[2] = pixelToinch( CallScilabBridge::getScreenWidth(getScilabJavaVM()) );
        value[3] = pixelToinch( CallScilabBridge::getScreenHeight(getScilabJavaVM()) );

        flag = sciReturnRowVector(value, 4);

        delete[] value;

        return flag;
    }
    else if (!stricmp(propertyName, "screensize_pt"))
    {
        value[0] = 0.0;
        value[1] = 0.0;
        value[2] = pixelTopt( CallScilabBridge::getScreenWidth(getScilabJavaVM()) );
        value[3] = pixelTopt( CallScilabBridge::getScreenHeight(getScilabJavaVM()) );

        flag = sciReturnRowVector(value, 4);

        delete[] value;

        return flag;
    }
    else if (!stricmp(propertyName, "screensize_norm"))
    {
        value[0] = 0.0;
        value[1] = 0.0;
        value[2] = 1.0;
        value[3] = 1.0;

        flag = sciReturnRowVector(value, 4);

        delete[] value;

        return flag;
    }
    else if (!stricmp(propertyName, "screendepth"))
    {
        value[0] = CallScilabBridge::getScreenDepth(getScilabJavaVM());

        flag = sciReturnRowVector(value, 1);

        delete[] value;

        return flag;
    }
    else
    {
        delete[] value;
        return NULL;
    }
}
/*--------------------------------------------------------------------------*/
