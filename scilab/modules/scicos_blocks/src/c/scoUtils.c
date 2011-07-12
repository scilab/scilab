/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - Scilab Enterprises - Clément DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "machine.h"
#include "MALLOC.h"
#include "BOOL.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

#include "scoUtils.h"
#include "scicos_block4.h"
#include "scicos.h"

#include <string.h>

char *findChildWithKindAt(char *parent, const char *type, int position)
{
    char *child = NULL;

    int childrenCount;
    char **children;

    int i;
    char *childType;
    int typeCount;

    int *pChildrenCount = &childrenCount;

    getGraphicObjectProperty(parent, __GO_CHILDREN_COUNT__, jni_int, (void **)&pChildrenCount);
    getGraphicObjectProperty(parent, __GO_CHILDREN__, jni_string_vector, (void **)&children);

    for (typeCount = 0, i = 0; i < childrenCount; i++)
    {
        getGraphicObjectProperty(children[i], __GO_TYPE__, jni_string, (void **)&childType);

        if (strcmp(childType, type) == 0)
        {
            typeCount++;
        }
//      Commented due to the C++ allocation
//      see http://bugzilla.scilab.org/show_bug.cgi?id=9747
//      FREE(childType);

        if (typeCount == (position + 1))
        {
            child = strdup(children[i]);
            break;
        }
    }

//  Commented due to the C++ allocation
//  see http://bugzilla.scilab.org/show_bug.cgi?id=9747
//  for (; i>=0; i--) {
//      FREE(children[i]);
//  }
//  FREE(children);

    return child;
};

BOOL setLabel(char *pAxeUID, const char *_pstName, const char *label)
{
    char *pLabelUID;
    char *rwLabel = strdup(label);
    int dimensions[2];

    BOOL result;

    getGraphicObjectProperty(pAxeUID, _pstName, jni_string, (void **)&pLabelUID);

    if (pLabelUID != NULL)
    {
        dimensions[0] = 1;
        dimensions[1] = 1;

        result = setGraphicObjectProperty(pLabelUID, __GO_TEXT_ARRAY_DIMENSIONS__, &dimensions, jni_int_vector, 2);
    }

    if (pLabelUID != NULL && result == TRUE)
    {
        result = setGraphicObjectProperty(pLabelUID, __GO_TEXT_STRINGS__, &rwLabel, jni_string_vector, 1);
    }

    FREE(rwLabel);
//  Commented due to the C++ allocation
//  see http://bugzilla.scilab.org/show_bug.cgi?id=9747
//  FREE(pLabelUID);

    return result && pLabelUID != NULL;
}
