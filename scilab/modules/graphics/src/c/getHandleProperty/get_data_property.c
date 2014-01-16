/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_data_property.c                                              */
/* desc : function to retrieve in Scilab the data field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "get_data_property.h"
#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnPropertyList.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "SetPropertyStatus.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "Matplot.h"
#include "returnProperty.h"
/*--------------------------------------------------------------------------*/
/* the matplot data can have several type */
int getmatplotdata(void * _pvCtx, int iObjUID)
{
    int datatype = 0;
    int * piDataType = &datatype;
    int numX = 0;
    int *piNumX = &numX;
    int numY = 0;
    int *piNumY = &numY;
    void * data = NULL;
    int imagetype = 0;
    int * piImagetype = &imagetype;
    int gltype = 0;
    int * piGltype = &gltype;
    int status = SET_PROPERTY_ERROR;
    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void **)&piNumX);
    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void **)&piNumY);
    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_DATA_TYPE__, jni_int, (void **)&piDataType);
    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_Z__, jni_double_vector, &data);
    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_IMAGE_TYPE__, jni_int, (void **)&piImagetype);
    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_GL_TYPE__, jni_int, (void **)&piGltype);

    if (!data)
    {
        return sciReturnEmptyMatrix(_pvCtx);
    }

    switch ((DataType)datatype)
    {
        case MATPLOT_HM1_Char :
        {
            int dims[3] = {numY - 1, numX - 1, 1};
            status = sciReturnHypermatOfInteger8(_pvCtx, dims, 3, (char*)data);
            break;
        }
        case MATPLOT_HM1_UChar :
        {
            int dims[3] = {numY - 1, numX - 1, 1};
            status = sciReturnHypermatOfUnsignedInteger8(_pvCtx, dims, 3, (unsigned char*)data);
            break;
        }
        case MATPLOT_HM3_Char :
        {
            int dims[3] = {numY - 1, numX - 1, 3};
            status = sciReturnHypermatOfInteger8(_pvCtx, dims, 3, (char*)data);
            break;
        }
        case MATPLOT_HM3_UChar :
        {
            int dims[3] = {numY - 1, numX - 1, 3};
            status = sciReturnHypermatOfUnsignedInteger8(_pvCtx, dims, 3, (unsigned char*)data);
            break;
        }
        case MATPLOT_HM1_Double :
        {
            int dims[3] = {numY - 1, numX - 1, 1};
            status = sciReturnHypermatOfDouble(_pvCtx, dims, 3, (double*)data);
            break;
        }
        case MATPLOT_HM3_Double :
        {
            int dims[3] = {numY - 1, numX - 1, 3};
            status = sciReturnHypermatOfDouble(_pvCtx, dims, 3, (double*)data);
            break;
        }
        case MATPLOT_HM4_Char :
        {
            int dims[3] = {numY - 1, numX - 1, 4};
            status = sciReturnHypermatOfInteger8(_pvCtx, dims, 3, (char*)data);
            break;
        }
        case MATPLOT_HM4_UChar :
        {
            int dims[3] = {numY - 1, numX - 1, 4};
            status = sciReturnHypermatOfUnsignedInteger8(_pvCtx, dims, 3, (unsigned char*)data);
            break;
        }
        case MATPLOT_HM4_Double :
        {
            int dims[3] = {numY - 1, numX - 1, 4};
            status = sciReturnHypermatOfDouble(_pvCtx, dims, 3, (double*)data);
            break;
        }
        case MATPLOT_Char :
        {
            status = sciReturnMatrixOfInteger8(_pvCtx, (char*)data, numY - 1, numX - 1);
            break;
        }
        case MATPLOT_UChar :
        {

            if ((ImageType)imagetype == MATPLOT_RGB)
            {
                status = sciReturnMatrixOfUnsignedInteger8(_pvCtx, (unsigned char*)data, 3 * (numY - 1), numX - 1);
            }
            else if ((ImageType)imagetype == MATPLOT_GL_RGBA)
            {
                status = sciReturnMatrixOfUnsignedInteger8(_pvCtx, (unsigned char*)data, 4 * (numY - 1), numX - 1);
            }
            else
            {
                status = sciReturnMatrixOfUnsignedInteger8(_pvCtx, (unsigned char*)data, numY - 1, numX - 1);
            }
            break;
        }
        case MATPLOT_Short :
        {
            status = sciReturnMatrixOfInteger16(_pvCtx, (short*)data, numY - 1, numX - 1);
            break;
        }
        case MATPLOT_UShort :
        {
            status = sciReturnMatrixOfUnsignedInteger16(_pvCtx, (unsigned short*)data, numY - 1, numX - 1);
            break;
        }
        case MATPLOT_Int :
        {
            status = sciReturnMatrixOfInteger32(_pvCtx, (int*)data, numY - 1, numX - 1);
            break;
        }
        case MATPLOT_UInt :
        {
            status = sciReturnMatrixOfUnsignedInteger32(_pvCtx, (unsigned int*)data, numY - 1, numX - 1);
            break;
        }
        case MATPLOT_Double :
        {
            status = sciReturnMatrix(_pvCtx, (double*)data, numY - 1, numX - 1);
            break;
        }
    }

    return status;
}
/*--------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the grayplot data is now given as a tlist (like for surface and champ objects) */
int getgrayplotdata(int iObjUID)
{
    char * variable_tlist[] = {"grayplotdata", "x", "y", "z"};
    int numX = 0;
    int *piNumX = &numX;
    int numY = 0;
    int *piNumY = &numY;
    double* dataX = NULL;
    double* dataY = NULL;
    double* dataZ = NULL;

    /* Add 'variable' tlist items to stack */
    returnedList * tList = createReturnedList(3, variable_tlist);

    if (tList == NULL)
    {
        return -1;
    }

    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void **)&piNumX);
    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void **)&piNumY);

    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);
    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&dataZ);

    addColVectorToReturnedList(tList, dataX, numX);
    addColVectorToReturnedList(tList, dataY, numY);
    addMatrixToReturnedList(tList, dataZ, numX, numY);

    destroyReturnedList(tList);

    return 0;
}
/*--------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the champ data is now given as a tlist (like for surface objects) */
int getchampdata(int iObjUID)
{
    char * variable_tlist[] = {"champdata", "x", "y", "fx", "fy"};
    int* dimensions = NULL;
    double* arrowBasesX = NULL;
    double* arrowBasesY = NULL;
    double* arrowDirectionsX = NULL;
    double* arrowDirectionsY = NULL;

    /* Add 'variable' tlist items to stack */

    returnedList * tList = createReturnedList(4, variable_tlist);

    if (tList == NULL)
    {
        return -1;
    }

    getGraphicObjectProperty(iObjUID, __GO_CHAMP_DIMENSIONS__, jni_int_vector, (void **)&dimensions);

    getGraphicObjectProperty(iObjUID, __GO_BASE_X__, jni_double_vector, (void **)&arrowBasesX);
    getGraphicObjectProperty(iObjUID, __GO_BASE_Y__, jni_double_vector, (void **)&arrowBasesY);
    getGraphicObjectProperty(iObjUID, __GO_DIRECTION_X__, jni_double_vector, (void **)&arrowDirectionsX);
    getGraphicObjectProperty(iObjUID, __GO_DIRECTION_Y__, jni_double_vector, (void **)&arrowDirectionsY);

    addColVectorToReturnedList(tList, arrowBasesX, dimensions[0]);
    addColVectorToReturnedList(tList, arrowBasesY, dimensions[1]);
    addMatrixToReturnedList(tList, arrowDirectionsX, dimensions[0], dimensions[1]);
    addMatrixToReturnedList(tList, arrowDirectionsY, dimensions[0], dimensions[1]);

    destroyReturnedList(tList);

    return 0;
}
/*--------------------------------------------------------------------------*/
int get3ddata(int iObjUID)
{
    char *variable_tlist_color[] = {"3d", "x", "y", "z", "color"};
    char *variable_tlist[] = {"3d", "x", "y", "z"};
    int type = -1;
    int *piType = &type;
    double* colors = NULL;
    double* dataX = NULL;
    double* dataY = NULL;
    double* dataZ = NULL;
    int nbRow = 0;
    int *piNbRow = &nbRow;
    int nbCol = 0;
    int *piNbCol = &nbCol;

    returnedList * tList = NULL;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);
    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&dataZ);

    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_COLORS__, jni_double_vector, (void **)&colors);

    if (colors != NULL)
    {
        /* Add 'variable' tlist items to stack */
        tList = createReturnedList(4, variable_tlist_color);

        if (type == __GO_FAC3D__)
        {
            int numColors = 0;
            int *piNumColors = &numColors;

            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void **)&piNbRow);
            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void **)&piNbCol);

            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_COLORS__, jni_int, (void **)&piNumColors);

            addMatrixToReturnedList(tList, dataX, nbRow, nbCol);
            addMatrixToReturnedList(tList, dataY, nbRow, nbCol);
            addMatrixToReturnedList(tList, dataZ, nbRow, nbCol);

            /*
             * With per-facet colors, the data vector might be either a column one or a row one,
             * only the row vector case is managed at the moment.
             */
            if (numColors == nbCol)
            {
                addMatrixToReturnedList(tList, colors, 1, nbCol);
            }
            else
            {
                addMatrixToReturnedList(tList, colors, nbRow, nbCol);
            }

        }
        destroyReturnedList(tList);
    }
    else /* no color provided in input*/
    {
        /* Add 'variable' tlist items to stack */
        tList = createReturnedList(3, variable_tlist);

        if (type == __GO_FAC3D__)
        {
            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void **)&piNbRow);
            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void **)&piNbCol);

            addMatrixToReturnedList(tList, dataX, nbRow, nbCol);
            addMatrixToReturnedList(tList, dataY, nbRow, nbCol);
            addMatrixToReturnedList(tList, dataZ, nbRow, nbCol);
        }
        else if (type == __GO_PLOT3D__)
        {
            int* xDimensions;
            int* yDimensions;

            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void **)&piNbRow);
            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void **)&piNbCol);

            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_X_DIMENSIONS__, jni_int_vector, (void **)&xDimensions);
            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_Y_DIMENSIONS__, jni_int_vector, (void **)&yDimensions);

            addMatrixToReturnedList(tList, dataX, xDimensions[0], xDimensions[1]);
            addMatrixToReturnedList(tList, dataY, yDimensions[0], yDimensions[1]);
            addMatrixToReturnedList(tList, dataZ, nbRow, nbCol);
        }

        destroyReturnedList(tList);
    }

    return 0;
}
/*------------------------------------------------------------------------*/
int get_tip_data_property(void* _pvCtx, int iObjUID)
{
    double *tip_data = NULL;

    getGraphicObjectProperty(iObjUID, __GO_DATATIP_DATA__, jni_double_vector, (void **)&tip_data);

    if (tip_data == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "data");
        return -1;
    }

    return sciReturnRowVector(_pvCtx, tip_data, 3);
}
/*------------------------------------------------------------------------*/
int get_data_property(void* _pvCtx, int iObjUID)
{
    int type = -1;
    int *piType = &type;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);
    if (piType == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "type");
        return -1;
    }

    switch (type)
    {
        case __GO_FAC3D__ :
        case __GO_PLOT3D__ :
            return get3ddata(iObjUID);
        case __GO_CHAMP__ :
            return getchampdata(iObjUID);
        case __GO_GRAYPLOT__ :
            return getgrayplotdata(iObjUID);
        case __GO_MATPLOT__ :
            return getmatplotdata(_pvCtx, iObjUID);
        case __GO_DATATIP__ :
            return get_tip_data_property(_pvCtx, iObjUID);
        default :
            /* F.Leray 02.05.05 : "data" case for others (using sciGetPoint routine inside GetProperty.c) */
        {
            int nbRow  =  0;
            int nbCol  =  0;
            int status = SET_PROPERTY_ERROR;
            /* Warning the following function allocates data */
            double* data = sciGetPoint(iObjUID, &nbRow, &nbCol);

            if (data == NULL && nbRow == 0 && nbCol == 0)
            {
                /* Empty data */
                sciReturnEmptyMatrix(_pvCtx);
                status = SET_PROPERTY_SUCCEED;
            }
            else if (data == NULL && nbRow == -1 && nbCol == -1)
            {
                /* data allocation failed */
                Scierror(999, _("%s: No more memory."), "get_data_property");
                status = SET_PROPERTY_ERROR;
            }
            else if (data == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "data");
                return -1;
            }
            else
            {
                status = sciReturnMatrix(_pvCtx, data, nbRow, nbCol);
                FREE(data);
            }

            return status;
        }
    }
}
/*------------------------------------------------------------------------*/
