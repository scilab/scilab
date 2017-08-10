/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
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
#include <stdio.h>
#include <string.h>

extern "C"
{
#include "createGraphicObject.h"
#include "deleteGraphicObject.h"
#include "returnType.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

    int createObject3dData(int obj, int newObj, int type);

}
/*
 * Create the data object for the newObj
 * and copy the data from obj to newObj.
 */
int createObject3dData(int obj, int newObj, int type)
{
    double *pvecx, *pvecy, *pvecz;
    int dataObj = 0;
    int numElementsArray[3];
    BOOL result;
    int *ptr;

    dataObj = createDataObject(newObj, type);

    if (dataObj == 0)
    {
        return 0;
    }


    if (type == __GO_PLOT3D__ || type == __GO_GRAYPLOT__)
    {
        ptr = &numElementsArray[0];
        getGraphicObjectProperty(obj, __GO_DATA_MODEL_NUM_X__, jni_int, (void**)&ptr);
        ptr = &numElementsArray[1];
        getGraphicObjectProperty(obj, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**)&ptr);
        ptr = &numElementsArray[2];
        getGraphicObjectProperty(obj, __GO_DATA_MODEL_NUM_Z__, jni_int, (void**)&ptr);

        int grid[] = {1, numElementsArray[0], 1, numElementsArray[1]};
        result = setGraphicObjectProperty(newObj, __GO_DATA_MODEL_GRID_SIZE__, grid, jni_int_vector, 4);
    }
    else if (type == __GO_FAC3D__)
    {
        int ng = 0, nvg = 0;
        int *png = &ng, *pnvg = &nvg;
        int numColors;
        int *pNumColors = &numColors;
        double *Colors;
        getGraphicObjectProperty(obj, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &png);
        getGraphicObjectProperty(obj, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &pnvg);
        getGraphicObjectProperty(obj, __GO_DATA_MODEL_NUM_COLORS__, jni_int, (void**)&pNumColors);
        getGraphicObjectProperty(obj, __GO_DATA_MODEL_COLORS__, jni_double_vector, (void**)&Colors);

        numElementsArray[0] = numElementsArray[1] = numElementsArray[2] = ng * nvg;

        int elements[] = {ng, nvg, numColors};
        result = setGraphicObjectProperty(newObj, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, &elements, jni_int_vector, 3);
        setGraphicObjectProperty(newObj, __GO_DATA_MODEL_COLORS__, Colors, jni_double_vector, numColors);
    }
    else
    {
        return 0;
    }


    if (result == 0)
    {
        deleteDataObject(dataObj);
        return 0;
    }

    getGraphicObjectProperty(obj, __GO_DATA_MODEL_X__, jni_double_vector, (void**)&pvecx);
    getGraphicObjectProperty(obj, __GO_DATA_MODEL_Y__, jni_double_vector, (void**)&pvecy);
    getGraphicObjectProperty(obj, __GO_DATA_MODEL_Z__, jni_double_vector, (void**)&pvecz);

    setGraphicObjectProperty(newObj, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, numElementsArray[0]);
    setGraphicObjectProperty(newObj, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, numElementsArray[1]);
    setGraphicObjectProperty(newObj, __GO_DATA_MODEL_Z__, pvecz, jni_double_vector, numElementsArray[2]);

    return dataObj;
}
