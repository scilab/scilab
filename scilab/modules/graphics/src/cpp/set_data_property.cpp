/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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

/*------------------------------------------------------------------------*/
/* file: set_data_property.c                                              */
/* desc : function to modify in Scilab the data field of                  */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "types.hxx"
#include "double.hxx"
#include "int.hxx"

extern "C"
{
#include "set_data_property.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "SetPropertyStatus.h"
#include "SetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "Matplot.h"
#include "setHandleProperty.h"
#include "BasicAlgos.h"
}

int set_data_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int type = -1;
    int *piType = &type;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (type == __GO_CHAMP__)
    {
        AssignedList* tlist = NULL;
        int status = -1;

        if (((types::InternalType*)_pvData)->isTList() == false)
        {
            Scierror(999, _("Wrong type for input argument: A tlist expected.\n"));
            return SET_PROPERTY_ERROR;
        }

        /* we should have 4 properties in the tlist */
        tlist = createAssignedList(_pvCtx, 3, 4);
        if (tlist == NULL)
        {
            return SET_PROPERTY_ERROR;
        }

        status = setchampdata(_pvCtx, iObjUID, tlist);
        destroyAssignedList(tlist);
        return status;
    }
    else if (type == __GO_GRAYPLOT__)
    {
        AssignedList * tlist = NULL;
        int status = -1;

        if (((types::InternalType*)_pvData)->isTList() == false)
        {
            Scierror(999, _("Wrong type for input argument: A tlist expected.\n"));
            return SET_PROPERTY_ERROR;
        }

        /* we should have 3 properties in the tlist */
        tlist = createAssignedList(_pvCtx, 3, 3);
        if (tlist == NULL)
        {
            return SET_PROPERTY_ERROR;
        }

        status = setgrayplotdata(_pvCtx, iObjUID, tlist);
        destroyAssignedList(tlist);
        return status;
    }
    else if (type == __GO_FAC3D__ || type == __GO_PLOT3D__)
    {
        AssignedList * tlist = NULL;
        int status = -1;
        int listSize = 0;

        if (((types::InternalType*)_pvData)->isTList() == false)
        {
            Scierror(999, _("Wrong type for input argument: A tlist expected.\n"));
            return SET_PROPERTY_ERROR;
        }

        listSize = getStackListNbElement(_pvCtx, 3);

        if (listSize == 3)
        {
            tlist = createAssignedList(_pvCtx, 3, 3);
        }
        else if (listSize == 4)
        {
            tlist = createAssignedList(_pvCtx, 3, 4);
        }
        else
        {
            Scierror(999, _("Wrong size for input argument: %d or %d expected.\n"), 4, 5);
            return SET_PROPERTY_ERROR;
        }

        if (tlist == NULL)
        {
            return SET_PROPERTY_ERROR;
        }

        status = set3ddata(_pvCtx, iObjUID, tlist);
        destroyAssignedList(tlist);
        return status;

    }
    else if (type == __GO_MATPLOT__)
    {
        int datatype = 0;
        int * piDataType = &datatype;
        int imagetype = 0;
        int * piImageType = &imagetype;
        int dataorder = 0;
        int * piDataOrder = &dataorder;
        int plottype = -1;
        int gridSize[4];
        int numX = 0;
        int *piNumX = &numX;
        int numY = 0;
        int *piNumY = &numY;

        bool bRGB = false;

        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void **)&piNumX);
        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void **)&piNumY);
        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_DATA_TYPE__, jni_int, (void **)&piDataType);
        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_IMAGE_TYPE__, jni_int, (void **)&piImageType);
        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_DATA_ORDER__, jni_int, (void **)&piDataOrder);

        gridSize[1] = 1;
        gridSize[3] = 1;

        types::InternalType* pIT = (types::InternalType*)_pvData;
        if (pIT->isGenericType() == false)
        {
            Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
            return 1;
        }

        types::GenericType* pGT = pIT->getAs<types::GenericType>();
        int iDims = pGT->getDims();
        int* piDimsArray = pGT->getDimsArray();

        int iRows = piDimsArray[0];
        int iCols = piDimsArray[1];

        void* pvData = NULL;
        if (iDims != 2 && (iDims != 3 || (piDimsArray[2] != 1 && piDimsArray[2] != 3 && piDimsArray[2] != 4)))
        {
            Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
            return 1;
        }

        switch (pGT->getType())
        {
            case types::InternalType::ScilabInt8:
            {
                if (iDims == 2 && (DataType)datatype != MATPLOT_Char)
                {
                    plottype = buildMatplotType(MATPLOT_Char, (DataOrder)dataorder, (ImageType)imagetype);
                    bRGB = true;
                }
                else if (piDimsArray[2] != 1 && (DataType)datatype != MATPLOT_HM1_Char)
                {
                    plottype = buildMatplotType(MATPLOT_HM1_Char, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else if (piDimsArray[2] != 3 && (DataType)datatype != MATPLOT_HM3_Char)
                {
                    plottype = buildMatplotType(MATPLOT_HM3_Char, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else if (piDimsArray[2] != 4 && (DataType)datatype != MATPLOT_HM4_Char)
                {
                    plottype = buildMatplotType(MATPLOT_HM4_Char, (DataOrder)dataorder, (ImageType)imagetype);
                }

                pvData = (void*)pGT->getAs<types::Int8>()->get();
                break;
            }
            case types::InternalType::ScilabUInt8:
            {
                if (iDims == 2 && (DataType)datatype != MATPLOT_UChar)
                {
                    plottype = buildMatplotType(MATPLOT_UChar, (DataOrder)dataorder, (ImageType)imagetype);
                    bRGB = true;
                }
                else if ((DataType)datatype != MATPLOT_HM1_Char)
                {
                    plottype = buildMatplotType(MATPLOT_HM1_UChar, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else if (piDimsArray[2] != 3 && (DataType)datatype != MATPLOT_HM3_UChar)
                {
                    plottype = buildMatplotType(MATPLOT_HM3_UChar, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else if (piDimsArray[2] != 4 && (DataType)datatype != MATPLOT_HM4_UChar)
                {
                    plottype = buildMatplotType(MATPLOT_HM4_UChar, (DataOrder)dataorder, (ImageType)imagetype);
                }

                pvData = (void*)pGT->getAs<types::UInt8>()->get();
                break;
            }
            case types::InternalType::ScilabInt16:
            {
                if (iDims == 2 && (DataType)datatype != MATPLOT_Short)
                {
                    plottype = buildMatplotType(MATPLOT_Short, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else
                {
                    Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
                    return 1;
                }
                break;
            }
            case types::InternalType::ScilabUInt16:
            {
                if (iDims == 2 && (DataType)datatype != MATPLOT_UShort)
                {
                    plottype = buildMatplotType(MATPLOT_UShort, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else
                {
                    Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
                    return 1;
                }
                break;
            }
            case types::InternalType::ScilabInt32:
            {
                if (iDims == 2 && (DataType)datatype != MATPLOT_Int)
                {
                    plottype = buildMatplotType(MATPLOT_Int, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else
                {
                    Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
                    return 1;
                }
                break;
            }
            case types::InternalType::ScilabUInt32:
            {
                if (iDims == 2 && (DataType)datatype != MATPLOT_UInt)
                {
                    plottype = buildMatplotType(MATPLOT_UInt, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else
                {
                    Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
                    return 1;
                }
                break;
            }
            case types::InternalType::ScilabDouble:
            {
                if (iDims == 2 && (DataType)datatype != MATPLOT_Double)
                {
                    plottype = buildMatplotType(MATPLOT_Double, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else if ((DataType)datatype != MATPLOT_HM1_Double)
                {
                    plottype = buildMatplotType(MATPLOT_HM1_Double, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else if (piDimsArray[2] != 3 && (DataType)datatype != MATPLOT_HM3_Double)
                {
                    plottype = buildMatplotType(MATPLOT_HM3_Double, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else if (piDimsArray[2] != 4 && (DataType)datatype != MATPLOT_HM4_Double)
                {
                    plottype = buildMatplotType(MATPLOT_HM4_Double, (DataOrder)dataorder, (ImageType)imagetype);
                }

                pvData = (void*)pGT->getAs<types::Double>()->get();
                break;
            }
            default:
            {
                Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
                return 1;
            }
        }

        if (bRGB)
        {
            if ((ImageType)imagetype == MATPLOT_RGB)
            {
                const int mb3 = iRows / 3;
                if (mb3 * 3 == iRows)
                {
                    iRows = mb3;
                }
                else
                {
                    Scierror(202, _("%s: Wrong size for input argument #%d: The number of rows must be a multiple of 3.\n"), "data", 1);
                    return 1;
                }
            }
            else if ((ImageType)imagetype == MATPLOT_RGBA)
            {
                const int mb4 = iRows / 4;
                if (mb4 * 4 == iRows)
                {
                    iRows = mb4;
                }
                else
                {
                    Scierror(202, _("%s: Wrong size for input argument #%d: The number of rows must be a multiple of 4.\n"), "data", 1);
                    return 1;
                }
            }
        }

        if (numX != iCols + 1 || numY != iRows + 1)
        {
            gridSize[0] = iCols + 1;
            gridSize[2] = iRows + 1;
            setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);
        }

        if (plottype != -1)
        {
            setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_MATPLOT_DATA_INFOS__, &plottype, jni_int, 1);
        }

        setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_IMAGE_DATA__, pvData, jni_double_vector, iRows * iCols);
    }
    else  /* F.Leray 02.05.05 : "data" case for others (using sciGetPoint routine inside GetProperty.c) */
    {
        if (((types::InternalType*)_pvData)->isDouble() == false)
        {
            Scierror(999, _("Incompatible type for property ''%s''.\n"), "data");
            return SET_PROPERTY_ERROR;
        }

        nbRow = ((types::GenericType*)_pvData)->getRows();
        nbCol = ((types::GenericType*)_pvData)->getCols();

        return sciSetPoint(iObjUID, ((types::Double*)_pvData)->get(), &nbRow, &nbCol);
    }
    return SET_PROPERTY_ERROR;
}
/*------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the champ data is now set as a tlist (like for surface objects) */
/* setchampdata(pobj,cstk(l2), &l3, &numrow3, &numcol3, fname) */
int setchampdata(void* _pvCtx, int iObjUID, AssignedList * tlist)
{
    int nbRow[4];
    int nbCol[4];

    int numberArrows = 0;
    int dimensions[2];

    double* vx = NULL;
    double* vy = NULL;
    double* vfx = NULL;
    double* vfy = NULL;

    /* get parameters */
    vx = getCurrentDoubleMatrixFromList(_pvCtx, tlist, &nbRow[0], &nbCol[0]);
    vy = getCurrentDoubleMatrixFromList(_pvCtx, tlist, &nbRow[1], &nbCol[1]);
    vfx = getCurrentDoubleMatrixFromList(_pvCtx, tlist, &nbRow[2], &nbCol[2]);
    vfy = getCurrentDoubleMatrixFromList(_pvCtx, tlist, &nbRow[3], &nbCol[3]);

    /* check dim */
    if (nbCol[0] != 1 || nbCol[1] != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Columns vectors expected.\n"), "Tlist", 1);
        return SET_PROPERTY_ERROR;
    }

    numberArrows = nbRow[0] * nbRow[1];
    dimensions[0] = nbRow[0];
    dimensions[1] = nbRow[1];

    if (nbRow[2] != nbRow[0] || nbCol[2] != nbRow[1] || nbRow[3] != nbRow[2] || nbCol[3] != nbCol[2])
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Incompatible lengths.\n"), "Tlist", 3, 4);
        return SET_PROPERTY_ERROR;
    }

    if (nbRow[0] * nbCol[0] == 0 || nbRow[1] * nbCol[1] == 0 || nbRow[2] * nbCol[2] == 0 || nbRow[3] * nbCol[3] == 0)
    {
        return SET_PROPERTY_SUCCEED;//sciReturnEmptyMatrix();
    }

    /* Update the champ's number of arrows and dimensions then set the coordinates */
    setGraphicObjectProperty(iObjUID, __GO_NUMBER_ARROWS__, &numberArrows, jni_int, 1);
    setGraphicObjectProperty(iObjUID, __GO_CHAMP_DIMENSIONS__, dimensions, jni_int_vector, 2);

    setGraphicObjectProperty(iObjUID, __GO_BASE_X__, vx, jni_double_vector, dimensions[0]);
    setGraphicObjectProperty(iObjUID, __GO_BASE_Y__, vy, jni_double_vector, dimensions[1]);
    setGraphicObjectProperty(iObjUID, __GO_DIRECTION_X__, vfx, jni_double_vector, dimensions[0] * dimensions[1]);
    setGraphicObjectProperty(iObjUID, __GO_DIRECTION_Y__, vfy, jni_double_vector, dimensions[0] * dimensions[1]);

    return SET_PROPERTY_SUCCEED;
}

/*--------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the grayplot data is now set as a tlist (like for surface and champ objects) */
/* setgrayplot(pobj,cstk(l2), &l3, &numrow3, &numcol3, fname) */
int setgrayplotdata(void* _pvCtx, int iObjUID, AssignedList * tlist)
{
    BOOL result;

    int nbRow[3];
    int nbCol[3];
    int gridSize[4];

    double* pvecx = NULL;
    double* pvecy = NULL;
    double* pvecz = NULL;

    pvecx = getDoubleMatrixFromList(_pvCtx, tlist, 2, &nbRow[0], &nbCol[0]);
    pvecy = getDoubleMatrixFromList(_pvCtx, tlist, 3, &nbRow[1], &nbCol[1]);
    pvecz = getDoubleMatrixFromList(_pvCtx, tlist, 4, &nbRow[2], &nbCol[2]);

    if (nbCol[0] != 1 || nbCol[1] != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Column vector expected.\n"), "Tlist", 1);
        return SET_PROPERTY_ERROR;
    }

    if (nbRow[2] != nbRow[0] || nbCol[2] != nbRow[1])
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Incompatible length.\n"), "Tlist", 3);
        /* Was previously: */
#if 0
        Scierror(999, _("%s: Wrong size for input argument #%d: Incompatible length.\n"), "Tlist", "Tlist", 3);
#endif
        return 0;
    }

    if (nbRow[0] * nbCol[0] == 0 || nbRow[1] * nbCol[1] == 0 || nbRow[2] * nbCol[2] == 0)
    {
        return SET_PROPERTY_ERROR; //sciReturnEmptyMatrix()
    }

    /*
    * Update the x and y vectors dimensions
    * These vectors are column ones
    */
    gridSize[0] = nbRow[0];
    gridSize[1] = 1;
    gridSize[2] = nbRow[1];
    gridSize[3] = 1;

    /* Resizes the coordinates arrays if required */
    result = setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);

    if (result == FALSE)
    {
        Scierror(999, _("%s: No more memory.\n"), "setgrayplotdata");
        return SET_PROPERTY_ERROR;
    }

    setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, nbRow[0]);
    setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, nbRow[1]);
    setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_Z__, pvecz, jni_double_vector, nbRow[2] * nbCol[2]);

    return SET_PROPERTY_SUCCEED;
}
/*--------------------------------------------------------------------------*/
/* set3ddata(pobj,cstk(l2), &l3, &numrow3, &numcol3) */
int set3ddata(void* _pvCtx, int iObjUID, AssignedList * tlist)
{
    int type = -1;
    int *piType = &type;

    int m1, n1, m2, n2, m3, n3;
    int m3n, n3n;
    int isFac3d;

    double* pvecx = NULL;
    double* pvecy = NULL;
    double* pvecz = NULL;
    int dimvectx = 0;
    int dimvecty = 0;

    double* inputColors;
    int nbInputColors;

    // number of specified colors
    int nc = 0;

    int izcol;

    /* no copy now we just perform tests on the matrices */
    pvecx = getCurrentDoubleMatrixFromList(_pvCtx, tlist, &m1, &n1);
    pvecy = getCurrentDoubleMatrixFromList(_pvCtx, tlist, &m2, &n2);
    pvecz = getCurrentDoubleMatrixFromList(_pvCtx, tlist, &m3, &n3);

    if (m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1)
    {
        if (!(m1 == m2 && m2 == m3 && n1 == n2 && n2 == n3))
        {
            Scierror(999, _("%s: Wrong size for input arguments #%d, #%d and #%d: Incompatible lengths.\n"), "Tlist", 1, 2, 3);
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        if (m2 * n2 != n3)
        {
            Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Incompatible lengths.\n"), "Tlist", 2, 3);
            return SET_PROPERTY_ERROR;
        }
        if (m1 * n1 != m3)
        {
            Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Incompatible lengths.\n"), "Tlist", 1, 3);
            return SET_PROPERTY_ERROR;
        }
        if (m1 * n1 <= 1 || m2 * n2 <= 1)
        {
            Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Should be >= %d.\n"), "Tlist", 1, 2, 2);
            return SET_PROPERTY_ERROR;
        }
    }

    if (m1 * n1 == 0 || m2 * n2 == 0 || m3 * n3 == 0)
    {
        return SET_PROPERTY_ERROR;//sciReturnEmptyMatrix();
    }

    /* get color size if exists */
    if (getAssignedListNbElement(tlist) == 4)
    {
        getCurrentDoubleMatrixFromList(_pvCtx, tlist, &m3n, &n3n);
        if (m3n * n3n == m3 * n3)
        {
            /* the color is a matrix, with same size as Z */
            izcol = 2;
        }
        else if (m3n * n3n == n3 && (m3n == 1 || n3n == 1))
        {
            /* a vector with as many colors as facets */
            izcol = 1;
        }
        else
        {
            Scierror(999, _("Wrong size for %s element: A %d-by-%d matrix or a vector of size %d expected.\n"), "color", m3, n3, n3);
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        m3n = 0;
        n3n = 0;
        izcol = 0;
    }

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (type == __GO_FAC3D__)
    {
        isFac3d = 1;
    }
    else
    {
        isFac3d = 0;
    }

    if (m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1) /* NG beg */
    {
        /* case isfac=1;*/
        if (isFac3d == 0)
        {
            Scierror(999, _("Cannot change the %s of graphic object: its type is %s.\n"), "typeof3d", "SCI_PLOT3D");
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        /* case isfac=0;*/
        if (isFac3d == 1)
        {
            Scierror(999, _("Cannot change the %s of graphic object: its type is %s.\n"), "typeof3d", "SCI_FAC3D");
            return SET_PROPERTY_ERROR;
        }
    }

    /* check the monotony on x and y */

    if (isFac3d == 1)
    {
        /* x is considered as a matrix */
        dimvectx = -1;
    }
    else if (m1 == 1) /* x is a row vector */
    {
        dimvectx = n1;
    }
    else if (n1 == 1) /* x is a column vector */
    {
        dimvectx = m1;
    }
    else /* x is a matrix */
    {
        dimvectx = -1;
    }

    if (dimvectx > 1)
    {
        int monotony = checkMonotony(pvecx, dimvectx);
        if (monotony == 0)
        {
            Scierror(999, _("%s: Wrong value: Vector is not monotonous.\n"), "Objplot3d");
            return SET_PROPERTY_ERROR;
        }

        /* To be implemented within the MVC */
#if 0
        psurf->flag_x = monotony;
#endif
    }

    if (isFac3d == 1)
    {
        /* x is considered as a matrix */
        dimvecty = -1;
    }
    else if (m2 == 1) /* y is a row vector */
    {
        dimvecty = n2;
    }
    else if (n2 == 1) /* y is a column vector */
    {
        dimvecty = m2;
    }
    else /* y is a matrix */
    {
        dimvecty = -1;
    }

    if (dimvecty > 1)
    {
        int monotony = checkMonotony(pvecy, dimvecty);
        if (monotony == 0)
        {
            Scierror(999, _("%s: Wrong value: Vector is not monotonous.\n"), "Objplot3d");
            return SET_PROPERTY_ERROR;
        }

        /* To be implemented within the MVC */
#if 0
        psurf->flag_y = monotony;
#endif
    }

    /* get the values now */
    rewindAssignedList(tlist);

    pvecx = getCurrentDoubleMatrixFromList(_pvCtx, tlist, &m1, &n1);
    pvecy = getCurrentDoubleMatrixFromList(_pvCtx, tlist, &m2, &n2);
    pvecz = getCurrentDoubleMatrixFromList(_pvCtx, tlist, &m3, &n3);

    if (isFac3d == 1)
    {
        int numElementsArray[3];
        int result;

        numElementsArray[0] = n1;
        numElementsArray[1] = m1;
        numElementsArray[2] = m3n * n3n;

        result = setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 3);

        if (result == 0)
        {
            Scierror(999, _("%s: No more memory.\n"), "set3ddata");
            return SET_PROPERTY_ERROR;
        }
    }
    else if (isFac3d == 0)
    {
        int gridSize[4];
        int result;

        gridSize[0] = m1;
        gridSize[1] = n1;
        gridSize[2] = m2;
        gridSize[3] = n2;

        result = setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);

        if (result == 0)
        {
            Scierror(999, _("%s: No more memory.\n"), "set3ddata");
            return SET_PROPERTY_ERROR;
        }
    }

    setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, m1 * n1);
    setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, m2 * n2);

    /*
    * Plot 3d case not treated for now
    * To be implemented
    */
    if (isFac3d == 1)
    {
        if (getAssignedListNbElement(tlist) == 4) /* F.Leray There is a color matrix */
        {
            inputColors = getCurrentDoubleMatrixFromList(_pvCtx, tlist, &m3n, &n3n);
            nbInputColors = m3n * n3n;
        }
        else
        {
            inputColors = NULL;
            nbInputColors = 0;
        }

        setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_Z__, pvecz, jni_double_vector, m3 * n3);
        setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_COLORS__, inputColors, jni_double_vector, nbInputColors);
    }
    else
    {
        setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_Z__, pvecz, jni_double_vector, m3 * n3);
    }

    /* Color vector/matrix dimensions: to be checked for MVC implementation */
#if 0
    psurf->m3n = m3n; /* If m3n and n3n are 0, then it means that no color matrix/vector was in input*/
    psurf->n3n = n3n;
#endif

    return SET_PROPERTY_SUCCEED;
}
