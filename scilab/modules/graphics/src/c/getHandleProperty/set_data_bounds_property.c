/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_data_bounds_property.c                                       */
/* desc : function to modify in Scilab the data_bounds field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"
#include "axesScale.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int getdDataBoundsFromStack( size_t  stackPointer, int nbRow, int nbCol,
                             double * xMin, double * xMax,
                             double * yMin, double * yMax,
                             double * zMin, double * zMax );
/*------------------------------------------------------------------------*/
/**
 * fill bounds (xMin, xMax, yMin,... ) from the assigned value in the stack
 * beacause it might have several possible size.
 */
int getdDataBoundsFromStack( size_t  stackPointer, int nbRow, int nbCol,
                             double * xMin, double * xMax,
                             double * yMin, double * yMax,
                             double * zMin, double * zMax )
{
    double * bounds = stk( stackPointer ) ;

    /* initialize zMin and zMax to avoid checking between 2D and 3D */
    *zMin = 1.0 ;
    *zMax = 2.0 ;

    if ( nbRow == 3 ) /* Remove the 3x2 case */
    {
        Scierror(999, _("Wrong size for '%s' property: Must be in the set {%s}.\n"), "data_bounds", "1x4, 1x6, 2x2, 2x3, 4x1, 6x1");
        return SET_PROPERTY_ERROR ;
    }

    switch ( nbRow * nbCol )
    {
        case 4 : /* 2D case */
            *xMin = bounds[0] ;
            *xMax = bounds[1] ;
            *yMin = bounds[2] ;
            *yMax = bounds[3] ;
            break;

        case 6 : /* 3D case */
            *xMin = bounds[0] ;
            *xMax = bounds[1] ;
            *yMin = bounds[2] ;
            *yMax = bounds[3] ;
            *zMin = bounds[4] ;
            *zMax = bounds[5] ;
            break ;
        default:
            Scierror(999, _("Wrong size for '%s' property: Must be in the set {%s}.\n"), "data_bounds", "1x4, 1x6, 2x2, 2x3, 4x1, 6x1");
            return SET_PROPERTY_ERROR ;
    }
    return SET_PROPERTY_SUCCEED ;
}

/*------------------------------------------------------------------------*/
int set_data_bounds_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status = FALSE;

    /* JB Silvy 09/11/05 */
    double   xMin = 0. ;
    double   xMax = 0. ;
    double   yMin = 0. ;
    double   yMax = 0. ;
    double   zMin = 0. ;
    double   zMax = 0. ;
    int firstPlot = 0;

    if ( !( valueType == sci_matrix ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "data_bounds");
        return SET_PROPERTY_ERROR;
    }

    /* get the bounds */
    if ( getdDataBoundsFromStack( stackPointer, nbRow, nbCol, &xMin, &xMax, &yMin, &yMax, &zMin, &zMax ) == SET_PROPERTY_ERROR )
    {
        return SET_PROPERTY_ERROR ;
    }

    /* To be implemented within the MVC */
    if (!checkDataBounds(pobjUID, xMin, xMax, yMin, yMax, zMin, zMax))
    {
        return SET_PROPERTY_ERROR;
    }

    /* copy the values in the axis */
    if ( nbRow * nbCol == 4 )
    {
        /* 2D */
        double bounds[6];
        double* tmpBounds;

        /* To get the Z coordinates */
        getGraphicObjectProperty(pobjUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&tmpBounds);

        if (tmpBounds == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "data_bounds");
            return SET_PROPERTY_ERROR;
        }

        bounds[0] = xMin;
        bounds[1] = xMax;
        bounds[2] = yMin;
        bounds[3] = yMax;
        bounds[4] = tmpBounds[4];
        bounds[5] = tmpBounds[5];

        status = setGraphicObjectProperty(pobjUID, __GO_DATA_BOUNDS__, bounds, jni_double_vector, 6);
    }
    else
    {
        /* 3D */
        double bounds[6] = {xMin, xMax, yMin, yMax, zMin, zMax} ;

        status = setGraphicObjectProperty(pobjUID, __GO_DATA_BOUNDS__, bounds, jni_double_vector, 6);
    }

    setGraphicObjectProperty(pobjUID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "data_bounds");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
