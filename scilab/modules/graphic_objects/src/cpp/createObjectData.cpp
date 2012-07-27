/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "createGraphicObject.h"
#include "deleteGraphicObject.h"
#include "returnType.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

    char * createObject3dData(char *obj, char *newObj, char *type);

}

#include <stdio.h>
#include <string.h>

/*
 * Create the data object for the newObj
 * and copy the data from obj to newObj.
 */
char * createObject3dData(char *obj, char *newObj, char *type)
{
    double *pvecx, *pvecy, *pvecz;
    char *dataObj = NULL;
    int numElementsArray[3];
    BOOL result;
    int *ptr;

    /*not implemented for others types yet*/
    if (strcmp(type, __GO_PLOT3D__) != 0) {
        return NULL;
    }

    ptr = &numElementsArray[0];
    getGraphicObjectProperty(obj, __GO_DATA_MODEL_NUM_X__, jni_int, (void**)&ptr);
    ptr = &numElementsArray[1];
    getGraphicObjectProperty(obj, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**)&ptr);
    ptr = &numElementsArray[2];
    getGraphicObjectProperty(obj, __GO_DATA_MODEL_NUM_Z__, jni_int, (void**)&ptr);

    dataObj = (char*)createDataObject(newObj, type);


    if (dataObj == NULL)
    {
        return NULL;
    }

    int grid[] = {1, numElementsArray[0], 1, numElementsArray[1]};
    result = setGraphicObjectProperty(newObj, __GO_DATA_MODEL_GRID_SIZE__, grid, jni_int_vector, 4);



    if (result == 0)
    {
        deleteDataObject(dataObj);
        return NULL;
    }

    getGraphicObjectProperty(obj, __GO_DATA_MODEL_X__, jni_double_vector, (void**)&pvecx);
    getGraphicObjectProperty(obj, __GO_DATA_MODEL_Y__, jni_double_vector, (void**)&pvecy);
    getGraphicObjectProperty(obj, __GO_DATA_MODEL_Z__, jni_double_vector, (void**)&pvecz);

    setGraphicObjectProperty(newObj, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, numElementsArray[0]);
    setGraphicObjectProperty(newObj, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, numElementsArray[1]);
    setGraphicObjectProperty(newObj, __GO_DATA_MODEL_Z__, pvecz, jni_double_vector, numElementsArray[2]);

    return dataObj;
}
