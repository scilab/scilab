/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Get some of the properties of the Root window
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2026 - UTC - Stéphane Mottelet
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "GetRootProperty.hxx"

using namespace org_scilab_modules_gui_bridge;

void* GetRootProperty(void* _pvCtx, int iObjUID, char *propertyName)
{
    double value[4] ={0.0,0.0,0.0,0.0};

    if (!stricmp(propertyName, "screensize"))
    {
        char *units = NULL;
        getGraphicObjectProperty(iObjUID, __GO_UI_UNITS__, jni_string, (void **)&units);    

        if (!stricmp(units, "pixels"))
        {
            value[0] = 1.0;
            value[1] = 1.0;
            value[2] = CallScilabBridge::getScreenWidth(getScilabJavaVM());
            value[3] = CallScilabBridge::getScreenHeight(getScilabJavaVM());
        }
        else if (!stricmp(units, "centimeters"))
        {
            value[2] = pixelTocm( CallScilabBridge::getScreenWidth(getScilabJavaVM()) );
            value[3] = pixelTocm( CallScilabBridge::getScreenHeight(getScilabJavaVM()) );
        }
        else if (!stricmp(units, "inches"))
        {
            value[2] = pixelToinch( CallScilabBridge::getScreenWidth(getScilabJavaVM()) );
            value[3] = pixelToinch( CallScilabBridge::getScreenHeight(getScilabJavaVM()) );
        }
        else if (!stricmp(units, "points"))
        {
            value[2] = pixelTopt( CallScilabBridge::getScreenWidth(getScilabJavaVM()) );
            value[3] = pixelTopt( CallScilabBridge::getScreenHeight(getScilabJavaVM()) );
        }
        else if (!stricmp(units, "normalized"))
        {
            value[2] = 1.0;
            value[3] = 1.0;
        }
        else
        {
            FREE(units);
            return NULL;            
        }

        FREE(units);
        return sciReturnRowVector(value, 4);
    }
    else if (!stricmp(propertyName, "screendepth"))
    {
        value[0] = CallScilabBridge::getScreenDepth(getScilabJavaVM());

        return sciReturnRowVector(value, 1);
    }
    else if (!stricmp(propertyName, "pointer_location"))
    {
        int* location = NULL;
        char *units = NULL;
        getGraphicObjectProperty(iObjUID, __GO_POINTER_LOCATION__, jni_int_vector, (void **)&location);
        getGraphicObjectProperty(iObjUID, __GO_UI_UNITS__, jni_string, (void **)&units);

        if (!stricmp(units, "pixels"))
        {
            value[0] = (double) location[0];
            value[1] = (double) location[1];
        }
        else if (!stricmp(units, "centimeters"))
        {
            value[0] = pixelTocm(location[0]);
            value[1] = pixelTocm(location[1]);
        }
        else if (!stricmp(units, "inches"))
        {
            value[0] = pixelToinch(location[0]);
            value[1] = pixelToinch(location[1]);
        }
        else if (!stricmp(units, "points"))
        {
            value[0] = pixelTopt(location[0]);
            value[1] = pixelTopt(location[1]);
        }
        else if (!stricmp(units, "normalized"))
        {
            value[0] = (double)location[0]/(double)CallScilabBridge::getScreenWidth(getScilabJavaVM());
            value[1] = (double)location[1]/(double)CallScilabBridge::getScreenHeight(getScilabJavaVM());
        }
        else
        {
            FREE(units);
            FREE(location);
            return NULL;
        }
        FREE(units);
        FREE(location);
        return sciReturnRowVector(value, 2);
    }
    else
    {
        return NULL;
    }
}
/*--------------------------------------------------------------------------*/
