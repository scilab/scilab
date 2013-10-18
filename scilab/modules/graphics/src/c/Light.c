/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "createGraphicObject.h"
#include "deleteGraphicObject.h"
#include "returnType.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "HandleManagement.h"
#include "CurrentSubwin.h"
#include "Scierror.h"
#include "localization.h"
#include "BuildObjects.h"

BOOL isValidType(int type)
{
    return type >= 0 && type <= 1;
}

BOOL isValidColor(double * color)
{
    if (color != NULL)
    {
        return (color[0] >= 0.0 && color[0] <= 1.0) &&
               (color[1] >= 0.0 && color[1] <= 1.0) &&
               (color[2] >= 0.0 && color[2] <= 1.0);
    }
    return FALSE;
}

BOOL createLight(char* fname, long long axes_handle, int type, BOOL visible, double * position, double * direction, double * ambient_color, double * diffuse_color, double * specular_color, long long * pLightHandle)
{
    int iAxes = 0;
    int iLight = 0;
    int * piType = &type;
    int hType = 0;
    int * pihType = &hType;
    int * piVisible = &visible;

    if (pLightHandle == NULL)
    {
        return FALSE;
    }

    iAxes = getObjectFromHandle((long)axes_handle);
    if (iAxes == 0)
    {
        iAxes = getOrCreateDefaultSubwin();
        if (iAxes == 0)
        {
            Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
            return FALSE;
        }
    }

    //check handle type
    getGraphicObjectProperty(iAxes, __GO_TYPE__, jni_int, (void **)&pihType);
    if (hType != __GO_AXES__)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return FALSE;
    }

    iLight = createGraphicObject(__GO_LIGHT__);
    if (iLight == 0)
    {
        return FALSE;
    }

    setGraphicObjectProperty(iLight, __GO_VISIBLE__, piVisible, jni_bool, 1);

    if (isValidType(type))
    {
        setGraphicObjectProperty(iLight, __GO_LIGHT_TYPE__, piType, jni_int, 1);
    }

    if (position)
    {
        setGraphicObjectProperty(iLight, __GO_POSITION__, position, jni_double_vector, 3);
    }

    if (direction)
    {
        setGraphicObjectProperty(iLight, __GO_DIRECTION__, direction, jni_double_vector, 3);
    }

    if (isValidColor(ambient_color))
    {
        setGraphicObjectProperty(iLight, __GO_AMBIENTCOLOR__, ambient_color, jni_double_vector, 3);
    }

    if (isValidColor(diffuse_color))
    {
        setGraphicObjectProperty(iLight, __GO_DIFFUSECOLOR__, diffuse_color, jni_double_vector, 3);
    }

    if (isValidColor(specular_color))
    {
        setGraphicObjectProperty(iLight, __GO_SPECULARCOLOR__, specular_color, jni_double_vector, 3);
    }

    //return handle
    *pLightHandle = getHandle(iLight);

    //set light as child of axes
    setGraphicObjectRelationship(iAxes, iLight);
    return TRUE;
}
