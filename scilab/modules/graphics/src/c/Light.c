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

BOOL createLight(long long axes_handle, int type, BOOL visible, double * position, double * direction, double * ambient_color, double * diffuse_color, double * specular_color, long long * pLightHandle)
{
    const char * axes;
    char * light;
    int * piType = &type;
    int * piVisible = &visible;

    if (pLightHandle == NULL) return FALSE;

    axes = getObjectFromHandle(axes_handle);
    if (axes == NULL)
    {
        axes = getCurrentSubWin();
        if (axes == NULL) return FALSE;
    }

    light = createGraphicObject(__GO_LIGHT__);

    if (light == NULL) return FALSE;

    setGraphicObjectProperty(light, __GO_VISIBLE__, piVisible, jni_bool, 1);

    if (isValidType(type))
    {
        setGraphicObjectProperty(light, __GO_LIGHT_TYPE__, piType, jni_int, 1);
    }

    if (position)
    {
        setGraphicObjectProperty(light, __GO_LIGHT_POSITION__, position, jni_double_vector, 3);
    }

    if (direction)
    {
        setGraphicObjectProperty(light, __GO_LIGHT_DIRECTION__, direction, jni_double_vector, 3);
    }

    if (isValidColor(ambient_color))
    {
        setGraphicObjectProperty(light, __GO_AMBIENTCOLOR__, ambient_color, jni_double_vector, 3);
    }

    if (isValidColor(diffuse_color))
    {
        setGraphicObjectProperty(light, __GO_DIFFUSECOLOR__, diffuse_color, jni_double_vector, 3);
    }

    if (isValidColor(specular_color))
    {
        setGraphicObjectProperty(light, __GO_SPECULARCOLOR__, specular_color, jni_double_vector, 3);
    }
    
    *pLightHandle = getHandle(light);

    setGraphicObjectRelationship(axes, light);
    releaseGraphicObjectProperty(__GO_PARENT__, light, jni_string, 1);

    return TRUE;
}


BOOL deleteLight(long long light_handle)
{
    int iType;
    int *piType = &iType;
    const char * uid = NULL;

    uid = getObjectFromHandle(light_handle);
    if (uid == NULL) return FALSE;

    getGraphicObjectProperty(uid, __GO_TYPE__, jni_int, (void **)&piType);

    if (iType != __GO_LIGHT__) return FALSE;

    deleteGraphicObject((char*)uid);

    return TRUE;
}
